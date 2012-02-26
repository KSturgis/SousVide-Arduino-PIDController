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

/**
	Allows editting the system temperature in 
	degrees farenheight

	Menu item allows to change the set temperature
	of the PIDMachine.

	Uses:
		PIDMachine::retrieveSetTemp
		PIDMachine::storeSetTemp
**/
class SetPointMenu : public MenuItem {
public:
	static SetPointMenu* get_Instance();
		
	uint8_t get_Level();

	const char* get_MenuTitle();
	bool get_CaptureUpDown();

	void MenuEnter();
	void MenuUp();
	void MenuDown();

protected:
	uint8_t Retrieve();
	void Store();

private:
	SetPointMenu();
	static SetPointMenu* _instance;

	bool Selected;
	uint8_t Level;

	const char* Name;
	char Display[13];
};

