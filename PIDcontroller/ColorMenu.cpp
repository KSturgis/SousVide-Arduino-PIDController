#include "ColorMenu.h"
#include <Arduino.h>

ColorMenu::ColorMenu() {
	Items[0] = new ColorMenuItem("RED",8);
	Items[1] = new ColorMenuItem("GREEN",4);
	Items[2] = new ColorMenuItem("BLUE",0);
	Items[3] = Menu_Back::get_Instance();
	set_SelectedItem(NULL);
}

ColorMenu::~ColorMenu() {
	delete Items[0];
	delete Items[1];
	delete Items[2];
}

ColorMenu ColorMenu1 = ColorMenu();

void ColorMenu::Draw() {
	NokiaLCD* lcd = NokiaLCD::get_Instance();
	lcd->DrawRect(0,0,131,17,true,Color);
	lcd->WriteStrF(PSTR(" Sous Vide"), 1,1, CYAN, Color);
}

color_t ColorMenu::Color = 0x000;



ColorMenuItem::ColorMenuItem(const char* name, uint8_t bitOffset) {
	Name = name;
	BitOffset = bitOffset;
	Level = 0x0;
	Selected = false;
}

const char* ColorMenuItem::get_MenuTitle() {
	if(Selected){
		sprintf(Display, "> %s %x", Name, Level);
	} else {
		sprintf(Display, "%s %x", Name, Level);
	}
	return Display;
}

bool ColorMenuItem::get_CaptureUpDown() {
	return Selected;
}

void ColorMenuItem::MenuEnter() {
	Selected = !Selected;
	MenuSystem::get_Instance()->DrawMenuItem(this,true);
}

void ColorMenuItem::MenuUp() {
	if(Level<0xF) {
		Level++;
		//Set the right four bits in the Color, while
		// leaving the other bits as is
		ColorMenu::Color = (Level << BitOffset) | (~(0xF<<BitOffset) & ColorMenu::Color);
	}
	MenuSystem::get_Instance()->DrawMenuItem(this,true);
	ColorMenu::Draw();
	
}

void ColorMenuItem::MenuDown() {
	if(Level>0) {
		Level--;
		//Set the right four bits in the Color, while
		// leaving the other bits as is
		ColorMenu::Color = (Level << BitOffset) | (~(0xF<<BitOffset) & ColorMenu::Color);
	}
	MenuSystem::get_Instance()->DrawMenuItem(this,true);
	ColorMenu::Draw();
}

