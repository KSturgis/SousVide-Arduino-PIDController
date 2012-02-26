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
#include <stddef.h>
#include <inttypes.h>
#include "Encoder.h"
#include "RollingAverageF.h"

/** The maxium amount that the Ki parameter can be mulitplied with

The PID controll accumulates the total error over the period the system is running.  
The system can become less stable if disruptions to the system cause the accumulated
error to get to large. 

The accumulated error is bonded by
 [-MAX_ACCUM_ERROR, MAX_ACCUM_ERROR]
 **/
#define MAX_ACCUM_ERROR		1000.0F

/** When the process is set to a new set temperature, initially the system only 
operates in P mode.  This allows the system get towards the set point without
generating an excessive amoutt of accumulated error.

Once Kp*(current_error) < ID_ENABLE_POINT && Kp*(current_error) > 0
ID calculations will be enabled.
**/
#define ID_ENABLE_POINT		200

//Number of samples overwhich to calculate the change in
// temperature
#define NUM_TEMP_SAMPLES_DERIVATIVE	15

#define SET_PARAM_P 0
#define SET_PARAM_I 1 
#define SET_PARAM_D 2

/**
	PIDMachine controls the PID Tempterature controller
	for the sous vide system.  

	Handles updating the output to the heating system.
		Calling Update() causes the system to update the output value
	Stores and retieve process parameters from the eeprom along with eePromLib
	*/
class PIDMachine {
public:
	/** Retuns singleton reference to the PIDMachine **/
	static PIDMachine* get_Instance();

	/** Updates the output paramter.
	
		Called from Server with most current temperature, This function
		is called every timeInterval.
	**/
	void Update(float current_temp, float timeInterval);

	/** Returns the current set temperature in F **/
	float get_SetPoint();

	/** Changes the temperatute set point.   
	
	This does not store the new value
		resetting the controller will revert the setting to the 
		last setting **/
	void set_SetPoint(float);

	

	/** Returns the current value of the P parameter for the 
	    PID Controller **/
	float get_Kp();

	/** Changes the current value of the P parameter for the 
	    PID Controller 

		This does not store the new value
		resetting the controller will revert the setting to the 
		last setting.  See storeParameter to change the parameter
		and save parameter in the eeprom.
	**/
	void set_Kp(float);

	/** Returns the current value of the I parameter for the 
	    PID Controller **/
	float get_Ki();

	/** Changes the current value of the I parameter for the 
	    PID Controller 

		This does not store the new value
		resetting the controller will revert the setting to the 
		last setting.  See storeParameter to change the parameter
		and save parameter in the eeprom.
	**/
	void set_Ki(float);

	/** Returns the current value of the D parameter for the 
	    PID Controller **/
	float get_Kd();

	/** Changes the current value of the D parameter for the 
	    PID Controller 

		This does not store the new value
		resetting the controller will revert the setting to the 
		last setting.  See storeParameter to change the parameter
		and save parameter in the eeprom.
	**/
	void set_Kd(float);

	/** Retrieves the set temperature form the eeprom in 
		full degrees Farenheight.  
	**/
	uint8_t retrieveSetTemp();

	/** Stores and updates the set temperature of the PIDMachine
	    The set temperature is an integer.  See TempSensor for
		more information on temperature limitations **/
	void storeSetTemp(uint8_t);

	/** Retrieve one of the parameters of the PICMachine from the eeprom
		(SET_PARAM_P, SET_PARAM_I, SET_PARAM_D) 
		See Encoder for more information on the parameter
		value. **/
	int16_t retrieveParameter(uint8_t which);

	/** Store one of the parameters of the PICMachine in the eeprom
		(SET_PARAM_P, SET_PARAM_I, SET_PARAM_D) 
		See Encoder for more information on the parameter
		value. **/
	void storeParameter(int16_t value, uint8_t which);

	/** Current value of the output	calculated at the last output

		<=0 Heater completly off
		>=100 Heater is completly on
	**/
	float get_CurrentOutput();

	/** Returns the Process error @ the last Update()
	**/
	float get_Error() { return error; }

	/** Returns the Integral (accumulated error) @ the last 
		Update()
	**/
	float get_Integral() { return integral; }

	/** Returns the Derivative (change in error) @ the last
		Update().  The derivative is average over 
		NUM_TEMP_SAMPLES_DERIVATIVE calls to Update() to reduce
		digital error from the temperature sensor.
	**/
	float get_Derivative()  { return derivative; }

	/** Is the system currently running ID calculations.

	When the temperature is first set the system will initially
	only perform "P" calculations until the process temperature
	approaches the set temperature.  This reduces the accumulated
	error and overshoot past the set point
	**/
	bool get_IDCalulationEnabled() { return enableID; }

	/** Returns true if the accumulated error in the PID Machine
	has reached MAX_ACCUM_ERROR and is being limited.

	This is generally an indication that the process variables have
	caused the system to oscillate rather than converge at the set 
	point.
	**/
	bool get_IntegralOverflowError();

	/** Updates server with the current value of the process
	parameters.
	**/
	void UpdatedParameters();

protected:
	/** Loads the process parameters (i.e. Set temperature, kP, kI, kD)
	from the eeProm **/
	void RetrieveSettings();
	

private:
	static PIDMachine* _instance;
	PIDMachine();

	bool enableID;
	bool loopZero;

	float setPoint;
	float error, previous_error;
	float derivative, integral;
	float currentOutput;

	/** calculate the derivate over a number of cycles
	 to reduce transient  error from the temperature probe **/
	RollingAverageF *derivativeCalculator;

	encoder_t KpInt, KiInt, KdInt;
};
