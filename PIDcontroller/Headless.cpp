#include "Headless.h"
#include <Arduino.h>
#include "Encoder.h"
#include "PIDMachine.h"
#include "Server.h"
#include "TaskQueue.h"
#include "TempSensor.h"
#include "Util.h"

#define SPLIT F("\t")
#define MAX_RECV	36
void loopSerialInput();
char recvBuf[MAX_RECV];


Headless* Headless::_instance = NULL;
Headless* Headless::get_Instance() {
	if(_instance==NULL) {
		_instance=new Headless();
	}
	return _instance;
}

Headless::Headless() {
}

void Headless::Initialize() {
	TaskQueue* Tasks = TaskQueue::get_Instance();
	Tasks->EnqueueTask( loopSerialInput, 50, 1000 );
}

void Headless::onProcessIDChange(bool idCalcEnabled) {
	Serial.print(F("ID_CHANGE("));
	Serial.print(idCalcEnabled);
	Serial.println(F(")"));
}

void Headless::onParameterUpdate(float setPoint, float kP, float kI, float kD) {
	PIDMachine* pid = PIDMachine::get_Instance();
	Serial.print(F("paramUpdate:"));
	Serial.print(SPLIT);
	Serial.print(setPoint);
	Serial.print(SPLIT);
	Serial.print(kP);
	Serial.print(SPLIT);
	Serial.print(kI);
	Serial.print(SPLIT);
	Serial.println(kD);
}

void Headless::onProcessUpdate(float param) {
	PIDMachine* pid = PIDMachine::get_Instance();
	float setTemp = pid->get_SetPoint();
	float currentTemp = TempSensor1.get_AvgTempF();

	int dP,dI,dD;
	dP=(int)(pid->get_Kp()*pid->get_Error()+0.5f);
	dI=(int)(pid->get_Ki()*pid->get_Integral()+0.5f);
	dD=(int)(pid->get_Kd()*pid->get_Derivative()+0.5f);

	//Send debug information
	Serial.print("PIDUpdate:");
	Serial.print(SPLIT);
	Serial.print(currentTemp);
	Serial.print(SPLIT);
	Serial.print(pid->get_Error());
	Serial.print(SPLIT);
	Serial.print(pid->get_Integral());
	Serial.print(SPLIT);

	float derivative = pid->get_Derivative();
	Serial.print(derivative,4);
	Serial.print(SPLIT);
	Serial.print(dP);
	Serial.print(SPLIT);
	Serial.print(dI);
	Serial.print(SPLIT);
	Serial.print(dD);
	Serial.print(SPLIT);
	Serial.print((int)(param+0.5f));
	Serial.print(SPLIT);
	Serial.print((int)(millis()/1000.0f+0.5f));
	Serial.print(SPLIT);
	Serial.print(get_free_memory());
	Serial.println("");
}

void Headless::onTempUpdate(float tempF) {
	//Serial.print(F("TempUpdate:"));
	//Serial.print(SPLIT);
	//Serial.println(tempF);
}

void Headless::RecvCmd(const char* cmd) {
	//temperature update
	if(strncmp("SetTemp(",cmd,8)==0) {
		const char* s=cmd+8;
		int a = atoi(s);

		if(a>0 && a<255) {
			PIDMachine::get_Instance()->storeSetTemp(a);
		} else {
			Serial.println("Invalid temp");
		}
	} 

	//Set Parameter P
	else if(strncmp("SetP(",cmd,5)==0) {
		const char* s=cmd+5;
		float a = atof(s);
		SetParameter(a,SET_PARAM_P);
	} 

	//Set Parameter I
	else if(strncmp("SetI(",cmd,5)==0) {
		const char* s=cmd+5;
		float a = atof(s);
		SetParameter(a,SET_PARAM_I);
	}

	//Set Parameter D
	else if (strncmp("SetD(",cmd,5)==0) {
		const char* s=cmd+5;
		float a = atof(s);
		SetParameter(a,SET_PARAM_D);
	}

	else {
		Serial.print(F("Unkown cmd: "));
		Serial.println(recvBuf);
	}
}

void Headless::SetParameter(float a, int8_t  which) {
	encoder_t aDigit = Encoder_ftoi(a);

	Serial.print(F("Param: "));
	Serial.print(which);
	Serial.print(SPLIT);
	Serial.print(aDigit);
	Serial.print(SPLIT);
	Serial.println(Encoder_itof(aDigit));
	PIDMachine::get_Instance()->storeParameter(aDigit, which);
	
}

void loopSerialInput() {
	int idx, avail;
	// send data only when you receive data:
	avail = Serial.available();

	if(avail>0 ) {
		if(avail>MAX_RECV) { avail = MAX_RECV; }
		for(idx=0; idx<avail; idx++) {
			// read the incoming byte:
			recvBuf[idx] = Serial.read();
		}
		recvBuf[idx]=0;
		
		Headless::get_Instance()->RecvCmd(recvBuf);		
	}
}