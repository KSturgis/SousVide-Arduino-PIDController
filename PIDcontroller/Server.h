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
#include "RollingAverage.h"
#include "TaskQueue.h"
#include "TempSensor.h"
#include "Util.h"
#include "UISystem.h"


/** Sets the Cycle preiod of the PWM timer to 4 seconds **/
#define CYCLE_PERIOD		31250
/** Max sleep.  In practice this isn't used this
    just sets an maximum amount of ms the loop will
	sleep **/
#define MAX_SLEEP			1000

/**
	The Server is the center of the system

	It handles interfacing between the PIDMachine which does 
	all the calculation, the View which handles the user
	interface, and the PWM which controls the output to the
	heater
**/
class Server {
public:
	/**
		Returns the singleton instance of the server
	**/
	static Server* get_Instance();

	/**
		Called from ArduinoOS setup()

		Sets up the whole system and gets it ready to start 
		running
	**/
	void Initialize();

	/**
		View handles the User Iterface of the system.

		The View allows the pid system to adapt to different
		hardware configurations easily.

		The system could be adapted to a new display or 
		user interface just by setting the View
	**/
	UISystem* View;

	/**
		Called from the PIDMachine every time the output
		process is updated (output to the heater)
	**/
	void SetProcessValue(float outputParameter);

	/**
		Call from the ArduinoOS Loop.

		this basically gets called infintly repeated when
		the system is running
	**/
	void Loop();

protected:
	void SetupLCD();
	void SetupMenu();
	void SetupPWM();
	void SetDuty(float d);


private:
	static Server* _instance;
	Server();
};