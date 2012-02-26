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

/**
	Provides an interface between the server and the User Interface.

	The system is designed to allow the PID controller to be more modular
	and allow the system to be adapted easily to new hardware configurations
	such as new display systems **/
class UISystem {
public:
	/** Setup the UISystem **/
	virtual void Initialize()=0;

	/** Called from the server whenever the process
		enables or disables ID calculation **/
	virtual void onProcessIDChange(bool idCalcEnabled)=0; 

	/** Called from the server whenever any of the parameters 
		of the pid controller are changed.

		New values are passed to this function **/
	virtual void onParameterUpdate(float setPoint, float kP, float kI, float kD)=0;

	/** Called from the server whenever the PIDMachine is
		updated with the current temperature and the output parameter
		id updated.

		@param param output parameter [0-100] **/
	virtual void onProcessUpdate(float param)=0;

	/** Called from the server when the system has a new temperature
		reading from the temperature sensor **/
	virtual void onTempUpdate(float tempF)=0;
};


