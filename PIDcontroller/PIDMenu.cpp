#include "PIDMenu.h"
#include <Arduino.h>
#include "Util.h"
#include "Encoder.h"

PIDPage::PIDPage() {
	Items[0] = new NumeralItem("P",SET_PARAM_P);
	Items[1] = new NumeralItem("I",SET_PARAM_I);
	Items[2] = new NumeralItem("D",SET_PARAM_D);
	Items[3] = Menu_Back::get_Instance();
	set_SelectedItem(NULL);
}

PIDPage::~PIDPage() {
	delete Items[0];
	delete Items[1];
	delete Items[2];
}

PIDPage* PIDPage::_instance = NULL;
PIDPage* PIDPage::get_Instance() {
	if(_instance==NULL) {
		_instance = new PIDPage();
	}
	return _instance;
}

NumeralItem::NumeralItem(const char* name, uint8_t whichParam) {
	Name=name;
	Selected = false;
	WhichParameter = whichParam;
	Level = Retrieve_Level();
}

float NumeralItem::get_Level() {
	return Encoder_itof(Level);
}


const char* NumeralItem::get_MenuTitle() {
	uint8_t fDigits;
	float faLevel = abs(get_Level());
	fDigits = Encoder_digits(get_Level());

	if(Selected){
		sprintf(Display, "> %s:%s", Name, 
			formatFloat(get_Level(),3,fDigits));
	} else {
		sprintf(Display, "%s:%s", Name, 
			formatFloat(get_Level(),3,fDigits));
	}
	return Display;
}


bool NumeralItem::get_CaptureUpDown() {
	return Selected;
}

void NumeralItem::MenuEnter() {
	if(Selected) {
		//becoming deselected store the value
		Store_Level();
	}

	Selected = !Selected;
	MenuSystem::get_Instance()->DrawMenuItem(this,true);
}

int16_t NumeralItem::get_Delta() {
	return 1;
}

void NumeralItem::MenuUp() {	
	Level+=get_Delta();

	MenuSystem::get_Instance()->DrawMenuItem(this,true);
}

void NumeralItem::MenuDown() {
	Level-=get_Delta();

	MenuSystem::get_Instance()->DrawMenuItem(this,true);
}

int16_t NumeralItem::Retrieve_Level() {
	return PIDMachine::get_Instance()->retrieveParameter(WhichParameter);
}

void NumeralItem::Store_Level() {
	PIDMachine::get_Instance()->storeParameter(Level, WhichParameter);
}