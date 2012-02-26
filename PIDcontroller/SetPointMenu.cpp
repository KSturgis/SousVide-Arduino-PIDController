#include "SetPointMenu.h"
#include <Arduino.h>
#include "PIDMachine.h"

SetPointMenu* SetPointMenu::_instance = NULL;
SetPointMenu* SetPointMenu::get_Instance() {
	if(_instance==NULL) {
		_instance = new SetPointMenu();
	}
	return _instance;
}

SetPointMenu::SetPointMenu() {
	Level = Retrieve();
	Selected = false;
}

uint8_t SetPointMenu::get_Level() {
	return Level;
}

const char* SetPointMenu::get_MenuTitle() {
	if(Selected){
		sprintf(Display, "> SET: %3dF", Level);
	} else {
		sprintf(Display, "TEMP: %3dF", Level);
	}
	return Display;
}

bool SetPointMenu::get_CaptureUpDown() {
	return Selected;
}

void SetPointMenu::MenuEnter()  {
	if(Selected) {
		//store the temperature in eeprom
		Store();
	}
	Selected = !Selected;
	MenuSystem::get_Instance()->DrawMenuItem(this,true);
}

void SetPointMenu::MenuUp() {	
	Level++;
	MenuSystem::get_Instance()->DrawMenuItem(this,true);
}

void SetPointMenu::MenuDown() {
	Level--;
	MenuSystem::get_Instance()->DrawMenuItem(this,true);
}

uint8_t SetPointMenu::Retrieve() {
	return PIDMachine::get_Instance()->retrieveSetTemp();
}

void SetPointMenu::Store() {
	PIDMachine::get_Instance()->storeSetTemp(Level);
}
