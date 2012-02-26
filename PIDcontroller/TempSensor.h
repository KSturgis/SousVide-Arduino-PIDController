/*****************************************************************************
    SousVide PIDController
    Copyright ©2012 Ken Sturgis

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
******************************************************************************/
#pragma once
#include <Arduino.h>
#include "RollingAverageF.h"

#define NUM_TEMP_SAMPLES_TO_AVERAGE	8

/**
	Reads temperature from the ADC controller.

**/
class TempSensor {
public:
	/** Create a temperature sensor that reads from pin **/
	TempSensor(int8_t pin);
	~TempSensor();

	/** Return the current reading from the pin (raw ADC data) **/
	int16_t get_CurrentTemp();

	/** Gets the current temperature from the prob in degrees
	Farenheight **/
	float get_CurrentTempF();

	/** Averages the temperature output over a number of calls.
	The temperature is read every time Update() is called 
	**/
	float get_AvgTempF();

	/** Causes the system to store the current temperature in the
	rolling average temperature system used by get_AvgTempF() **/
	void Update();

protected:
	/** Store temperature data over a number of calls to Update() **/
	RollingAverageF* AvgData;

	/** Utility converts the raw ADC parameter to degrees F **/
	float convertF(int16_t);

private:
	int8_t TemperaturePin;
};

extern TempSensor TempSensor1;
