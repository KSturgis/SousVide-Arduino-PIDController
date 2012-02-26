#include "TempSensor.h"

TempSensor TempSensor1 = TempSensor(A0);

TempSensor::TempSensor(int8_t pin) {
	TemperaturePin =  pin;
	pinMode(TemperaturePin, INPUT);
	//Set the analogreference to 1.1V
	analogReference(INTERNAL);
	AvgData = new RollingAverageF(NUM_TEMP_SAMPLES_TO_AVERAGE);
}

TempSensor::~TempSensor() {
	delete AvgData;
}

int16_t TempSensor::get_CurrentTemp() {
	return analogRead(TemperaturePin);
}

float TempSensor::get_CurrentTempF() {
	return convertF(get_CurrentTemp());
}

float TempSensor::get_AvgTempF() {
	return AvgData->get_Average();
}

void TempSensor::Update() {
	int16_t tempD =  get_CurrentTemp();
	float tempF = convertF(tempD);
	AvgData->Add(tempF);
}

float TempSensor::convertF(int16_t tempADC) {
	// based upon 10bit AD with 10mV/C and 1.1V VRef

	// degC (C) = tempADC(bit)*1.1V/1024(bit)/.010(V/C);
	// degF (f) = degC*9/5+32;
	return tempADC*99.0f/512.0f+32.0f;;
}