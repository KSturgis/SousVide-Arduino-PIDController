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
#include "UISystem.h"
#include "NokiaLCD.h"

#define DEBUG_PARAMETER_UPDATE
#define DEBUG_PROCESS_UPDATE
//#define DEBUG_TEMP_UPDATE

#define DEBUG1_TXT_COLOR	0xFFF
#define DEBUG1_COLOR		0x5A4
#define DEBUG2_TXT_COLOR	0x320
#define DEBUG2_COLOR		0x38D
#define DEBUG3_TXT_COLOR	0x000
#define DEBUG3_COLOR		0xBA5
//Temp offset to be considered SET
#define MAX_TEMP_VARIANCE	0.75f


#define ICON_BOX				96,98,128,130
#define ICON_CIRCLE_CENTER		112,114
#define ICON_COLOR_LOW			0x40D
#define ICON_COLOR_NEAR_LOW		0x388
#define ICON_COLOR_SET			0x4E2
#define ICON_COLOR_NEAR_HIGH	0x883
#define ICON_COLOR_HIGH			0xFC4

/**
	UI System for the Nokia LCD screen 
	
	available from Spark Fun http://www.sparkfun.com/products/9363

	The Shield has a color LCD screen as well as three buttons.
	A menu system allows the user to change all the process parameters
	as well as outputs the current state of the system (temperature, 
	output, etc..)
**/
class ColorLCD : public UISystem {
public:
	ColorLCD();
	virtual void Initialize();

	void SetupMenu();

	virtual void onProcessIDChange(bool idCalcEnabled); 
	virtual void onParameterUpdate(float setPoint, float kP, float kI, float kD);
	virtual void onProcessUpdate(float param);
	virtual void onTempUpdate(float tempF);

private:
	NokiaLCD* lcd;

	void drawError();
	void drawTempLow();
	void drawTempHigh();
	void drawTempNearLow();
	void drawTempNearHigh();
	void drawTempSet();

	char lcdBuf[24];
};