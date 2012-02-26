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

#ifndef BtnManager_H
#define BtnManager_H

typedef void BtnAction(void);

#define NUM_BUTTONS 3
#define BTN_PINS  {3,4,5}
#define MIN_CYCLE_DEBOUNCE 2
#define REPEAT_CYCLE 3

/**

The BtnManager handles reading button clicks from the Sparkfun
NokiaLCD screen.

The manager
* De-bounces the button
* Repeats the click if the button is held down

The task manager should call Loop frequently, A button must be down
for MIN_CYCLE_DEBOUNCE calls of loop before the click event is called.
If the button is held down for a long time the click event will
repeat every REPEAT_CYCLE.

Click handlers are assigned by setting a void function()* to btnClickListners
**/
class BtnManager {
public:
	static BtnManager* get_Instance();
	void Loop();

	BtnAction* btnClickListners[NUM_BUTTONS];

protected:
	static int pins[];
	bool btnState[NUM_BUTTONS];
	int btnStateCnt[NUM_BUTTONS];

private:
	static BtnManager* _instance;
	BtnManager();
};

#endif