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

#ifndef COLOR_MENU_H
#define COLOR_MENU_H

#include "NokiaLCD.h"
#include "MenuSystem.h"
#include "MenuPage.h"

/**
	Menu page allows you to edit the color of the header
	bar on the LCD.  Has no real use, but it is fun and
	allows the programmer to choose colors to use in the
	UI
**/
class ColorMenu : public MenuPage {
public:
	ColorMenu();

	~ColorMenu();

	static color_t Color;

	void DrawPage() { Draw(); }

	static void Draw();
};

extern ColorMenu ColorMenu1;

/**
	Menu item allows the user to edit one segment of the 
	color (R, G, or B)

**/
class ColorMenuItem : public MenuItem {
public:
	bool Selected;
	uint8_t Level;
	
	ColorMenuItem(const char* name, uint8_t bitOffset);


	const char* get_MenuTitle();
	bool get_CaptureUpDown();

	void MenuEnter();
	void MenuUp();
	void MenuDown();

private:
	const char* Name;
	uint8_t BitOffset;
	char Display[20];
};

#endif