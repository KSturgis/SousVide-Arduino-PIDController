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
#include "MenuSystem.h"
#include "MenuPage.h"
#include "PIDMachine.h"

#define MIN_DELTA 0.1f

/**
	PIDPage
	Menu page allows editting of the PID
	paramters, each parameter is editted using
	a NumeralItem
**/
class PIDPage : public MenuPage {
public:
	static PIDPage* get_Instance();
	~PIDPage();

private:
	PIDPage();
	static PIDPage* _instance;
};


/**
	Allows editting one of the PID paramters
	paramters are stored using the Encoder system.
	This code interfaces with 
		PIDMachine::storeParameter
		PIDMachine::retrieveParameter
**/
class NumeralItem : public MenuItem {
public:
	NumeralItem(const char* name, uint8_t whichParam);

	bool Selected;
	float get_Level();


	const char* get_MenuTitle();
	bool get_CaptureUpDown();

	void MenuEnter();
	void MenuUp();
	void MenuDown();

protected:
	int16_t Retrieve_Level();
	void Store_Level();

private:
	uint8_t WhichParameter;
	int16_t Level;
	int16_t get_Delta();

	const char* Name;
	char Display[13];
	char Num_str[10];
};



