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

#ifndef MENU_H
#define MENU_H
#include "NokiaLCD.h"

#define MAX_NUM_MENU_ITEMS	4
#define MAX_MENU_DEPTH	3
#define MENU_MINY		17
#define MENU_HEIGHT		16
#define MENU_MIN_X		0
#define MENU_MAX_X		131
#define MENU_TEXT_X		4
#define MENU_ROWY(n)	(n)*MENU_HEIGHT+MENU_MINY


#define MENU_BACKGROUND				0xfff
#define MENU_BACKGROUND_SELECTED	0x6ff
#define MENU_TEXT					0x000
#define MENU_TEXT_SELECTED			0x553

//defined in MenuPage.h
class MenuPage;

/**
	MenuItem abstract class represents all items displayed on 
	a menu page.
**/
class MenuItem {
public:
	virtual const char* get_MenuTitle()=0;
	virtual bool get_CaptureUpDown()=0;

	virtual void MenuEnter()=0;
	virtual void MenuUp()=0;
	virtual void MenuDown()=0;
};


/**
	MenuSystem

	The menu system is the heart of the UI for the ColorLCD system.
	the user interface is a menu system with three buttons (up, down, enter)
	The system has a active page which has a number of menu items.
	The user can scroll through the menu items. Enter clicks are passed
	to the current menu item.  The menu item can set to capture the up/down
	clicks preventing scrolling through the menu, but allowing the user
	to change settings.

	The page system is a modal stack.  There is a current page that is displayed
	to the user.  A new page to could be added to the stack and it will be 
	displayed.  When the page is done the system can remove the page revealing
	the original page.
**/
class MenuSystem {
public:
	/** Singleton initializer **/
	static MenuSystem* get_Instance();

	/** Current Page **/
	MenuPage* get_CurrentPage();
	
	/** Selected Item from current page **/
	MenuItem* get_SelectedItem();

	/** Adds a page to the current page stack **/
	void AddPage(MenuPage* page);

	/** Remove the top page for the stack **/
	void PopPage();

	/** Redraw the whole current menu **/
	void Redraw();

	/** Draw only one menu item **/
	void DrawMenuItem(MenuItem* item, bool selected);

	/** Function called menu on enter button **/
	void MenuEnter();

	/** Function called when user selects menu up **/
	void MenuUp();

	/** Function called when user selects menu down **/
	void MenuDown();
private:

	MenuSystem();
	/** Singleton instance **/
	static MenuSystem* _instance;

protected:
	void DrawEmptyItem(uint8_t pos);
	int currentDepth;
	MenuPage* Pages[MAX_MENU_DEPTH];
};


class Menu_Back : public MenuItem {
public:
	static Menu_Back* get_Instance();

	const char* get_MenuTitle() { return "BACK"; }
	bool get_CaptureUpDown() { return false; }

	void MenuEnter();
	void MenuUp() { }
	void MenuDown() { }

private:
	Menu_Back();
	static Menu_Back* _instance;
};

class GotoPage : public MenuItem {
public:
	GotoPage(const char* name, MenuPage* page);

	const char* get_MenuTitle() { return Name; }
	bool get_CaptureUpDown() { return false; }

	void MenuEnter();
	void MenuUp() { }
	void MenuDown() { }
private:
	const char* Name;
	MenuPage* Page;
};


#endif