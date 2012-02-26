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
	Class represents a page in the menu system.

	Menu pages contain a series of menu_items.  The Up/Down buttons navigate
	selecting each menu item.  enter clicks are passed to the menu item.

	The menu item can be set to capture up down events, allowing the system to
	change patameters.

	Menu Pages are modal and kept in a stack.  Activating a menu item may add
	a new page to the menu stack (i.e. open a settings page) popping the new
	page off the stack will return the menu system to the previos page.
**/
class MenuPage {
public:
	MenuPage();
		
	MenuItem* Items[MAX_NUM_MENU_ITEMS];
	
	/**
		Return the currently selected menu item
	**/
	MenuItem* get_SelectedItem();

	/**
		Set the currently selected menu item
		**/
	void set_SelectedItem(MenuItem* item);

	/**
	Returns the idx of the currently selected item
	**/
	int8_t get_SelectedIdx();

	/**
	Changes the menu selection to the selected idx
	**/
	void set_SelectedIdx(int8_t idx);

	/**
		Called after redrawing the menu
	**/
	virtual void DrawPage() { };

	/**
	Retuns the idx if the given menu item
	-1 if there is no such item in the page
	**/
	int8_t GetIdx(MenuItem* item);
	

	/**
		Return the next valid menu item after the currently
		selected menu item
		**/
	MenuItem* NextItem();

	/**
		Returns the previous menu item before the currently
		selected menu item
		**/
	MenuItem* PrevItem();

private:
	int8_t selectedIdx;

};