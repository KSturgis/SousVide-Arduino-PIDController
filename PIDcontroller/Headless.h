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
#include <inttypes.h>
#include "UISystem.h"

/**
	UI system using the USB connection 

	* Recieve commands on USB Serial to change parameters
		* SetTemp(123) - Sets Temperature to 123 deg F
		* SetP(10.0)   - Sets P paramter to 10.0
		* SetI(0.0)    - Sets the I parameter to 0.0 (I calculation off)
		* SetD(50)     - Sets the D parameter to 50
	* Sends debug information on the USB
**/
class Headless : public UISystem {
public:
	static Headless* get_Instance();
	virtual void Initialize();

	virtual void onProcessIDChange(bool idCalcEnabled);
	virtual void onParameterUpdate(float setPoint, float kP, float kI, float kD);
	virtual void onProcessUpdate(float param);
	virtual void onTempUpdate(float tempF);

	/** Called when a command is recieved on the Serial bus
	**/
	virtual void RecvCmd(const char* cmd);

protected:
	void SetParameter(float a, int8_t  which);
private:
	static Headless* _instance;
	Headless();
};

