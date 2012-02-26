#include "PIDMachine.h"
#include <avr/eeprom.h>
#include "EEPromLib.h"
#include "Server.h"

PIDMachine::PIDMachine() {
	loopZero = true;
	enableID = false;
	setPoint=error=previous_error=currentOutput=0;
	derivative=integral=0;
	derivativeCalculator = new RollingAverageF(NUM_TEMP_SAMPLES_DERIVATIVE);

	RetrieveSettings();
}

PIDMachine* PIDMachine::_instance;
PIDMachine* PIDMachine::get_Instance() {
	if(_instance==NULL) {
		_instance = new PIDMachine();
	}
	return _instance;
}

void PIDMachine::Update(float current_temp, float timeInterval) {
	error = setPoint - current_temp;
	if(loopZero) {
		previous_error = error;
		loopZero=false;
	}

	//calculate the change int error.  Use dderivativeCalculator
	// to average change over NUM_TEMP_SAMPLES_DERIVATIVE
	// samples. record even if ID is disabled so the rolling
	// average machine will be ready to go when ID is enabled
	float currentDerivative = (error-previous_error)/timeInterval;
		derivativeCalculator->Add(currentDerivative);


	if(!enableID) {
		//ID calc disabled set params to 0
		integral = 0;
		derivative = derivativeCalculator->get_Average();

		currentOutput = get_Kp()*error;
	} else {
		//update ID values
		integral += error*timeInterval;

		//limit the accumulated integral error
		if(integral>MAX_ACCUM_ERROR) integral = MAX_ACCUM_ERROR;
		if(integral<-MAX_ACCUM_ERROR) integral = -MAX_ACCUM_ERROR;

		derivative = derivativeCalculator->get_Average();

		currentOutput = get_Kp()*error
						+get_Ki()*integral
						+get_Kd()*derivative;
	}

	//Auto start/stop ID calculation check if we should enable ID calcs
	if(!enableID && currentOutput>0 && currentOutput<ID_ENABLE_POINT) {
		enableID=true;
		Server::get_Instance()->View->onProcessIDChange(true);
	}

	//Change the output
	Server::get_Instance()->SetProcessValue(currentOutput);

	//Store previous error
	previous_error = error;
}

float PIDMachine::get_SetPoint() {
	return setPoint;
}

void PIDMachine::set_SetPoint(float value) {
	enableID = false;
	setPoint = value;
	Server::get_Instance()->View->onProcessIDChange(false);
	UpdatedParameters();
}

float PIDMachine::get_CurrentOutput() {
	return currentOutput;
}

float PIDMachine::get_Kp() {
	return Encoder_itof(KpInt);
}

float PIDMachine::get_Ki() {
	if(enableID) {
		return Encoder_itof(KiInt);
	}
	return 0.0f;
}

float PIDMachine::get_Kd() {
	if(enableID) {
		return Encoder_itof(KdInt);
	}
	return 0.0f;
}

void PIDMachine::set_Kp(float value) {
	KpInt=Encoder_ftoi(value);
	UpdatedParameters();
}

void PIDMachine::set_Ki(float value) {
	KiInt=Encoder_ftoi(value);
	UpdatedParameters();
}

void PIDMachine::set_Kd(float value) {
	KdInt=Encoder_ftoi(value);
	UpdatedParameters();
}

void PIDMachine::RetrieveSettings() {
	KpInt = eeprom_read_word(ee_PID_P);
	KiInt = eeprom_read_word(ee_PID_I);
	KdInt = eeprom_read_word(ee_PID_D);

	//todo: better loading and saving
	setPoint = (float)eeprom_read_byte(ee_TEMP_SET);
}

void PIDMachine::UpdatedParameters() {
	Server::get_Instance()->View->onParameterUpdate(
		get_SetPoint(),
		Encoder_itof(KpInt),
		Encoder_itof(KiInt),
		Encoder_itof(KdInt)
	);
}

uint8_t PIDMachine::retrieveSetTemp() {
	return (uint8_t)eeprom_read_byte(ee_TEMP_SET);
}

void PIDMachine::storeSetTemp(uint8_t tempI) {
	eeprom_write_byte(ee_TEMP_SET,(uint8_t)tempI);
	set_SetPoint((float)tempI);
}

int16_t PIDMachine::retrieveParameter(uint8_t which) {
	switch(which) {
		case SET_PARAM_D:
			return KdInt;
		case SET_PARAM_I:
			return KiInt;
		default:
		case SET_PARAM_P:
			return KpInt;
	}
}

void PIDMachine::storeParameter(int16_t value, uint8_t which) {
	switch(which) {		
		default:
		case SET_PARAM_P:
			KpInt=value;
			eeprom_write_word(ee_PID_P,(uint16_t)(KpInt));
			break;
		case SET_PARAM_I:
			KiInt=value;
			eeprom_write_word(ee_PID_I,(uint16_t)(KiInt));
			break;
		case SET_PARAM_D:
			KdInt=value;
			eeprom_write_word(ee_PID_D,(uint16_t)(KdInt));
			break;
	}
	UpdatedParameters();
}

bool PIDMachine::get_IntegralOverflowError() {
	return abs(integral)==MAX_ACCUM_ERROR;
}