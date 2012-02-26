#include "MenuSystem.h"
#include "MenuPage.h"
#include <Arduino.h>


MenuSystem::MenuSystem() {
	currentDepth=0;
}

MenuSystem* MenuSystem::_instance = NULL;

MenuSystem* MenuSystem::get_Instance() {
	if(_instance==NULL) {
		_instance = new MenuSystem();
	}
	return _instance;
}

MenuPage* MenuSystem::get_CurrentPage() {
	if(currentDepth>0) {
		return Pages[currentDepth-1];
	}
	return NULL;
}

MenuItem* MenuSystem::get_SelectedItem() {
	MenuPage* page = get_CurrentPage();
	
	if(page) {
		return page->get_SelectedItem();
	}
	return NULL;
}

void MenuSystem::AddPage(MenuPage* page) {
	Pages[currentDepth++]=page;
	Redraw();
}

void MenuSystem::PopPage() {
	//Serial.print(F("PopPage "));
	/*if(this->get_SelectedItem()) {
		Serial.println(get_CurrentPage()->get_SelectedItem()->get_MenuTitle());
	}*/
	Pages[--currentDepth]=NULL;
	Redraw();
}


void MenuSystem::Redraw() {
	MenuPage* page=get_CurrentPage();
	
	if( page==NULL ) {
		for(uint8_t idx=0;idx<MAX_NUM_MENU_ITEMS;idx++) {
			DrawEmptyItem(idx);
		}
	} else {
		for(uint8_t idx=0;idx<MAX_NUM_MENU_ITEMS;idx++) {
			MenuItem* item = page->Items[idx];
			if(item) {
				DrawMenuItem(item, item==page->get_SelectedItem());
			} else {
				DrawEmptyItem(idx);
			}
		}
		page->DrawPage();
	}
}

void MenuSystem::DrawMenuItem(MenuItem* item, bool selected) {
	MenuPage* page = get_CurrentPage();
	if(!page) return;

	int8_t pos = page->GetIdx(item);
	int bkgColor = selected ?  MENU_BACKGROUND_SELECTED:MENU_BACKGROUND;
	int Color = selected ? MENU_TEXT_SELECTED:MENU_TEXT;

	NokiaLCD* lcd = NokiaLCD::get_Instance();
	lcd->DrawRect(MENU_MIN_X, MENU_ROWY(pos), MENU_MAX_X, MENU_ROWY(pos+1),
		true,
		bkgColor);

	lcd->WriteStr(item->get_MenuTitle(), MENU_TEXT_X, MENU_ROWY(pos), Color, bkgColor);
}

void MenuSystem::DrawEmptyItem(uint8_t pos) {
	NokiaLCD* lcd = NokiaLCD::get_Instance();

	lcd->DrawRect(MENU_MIN_X, MENU_ROWY(pos), MENU_MAX_X, MENU_ROWY(pos+1)-1,
		true,
		MENU_BACKGROUND);
}


void MenuSystem::MenuEnter() {
	MenuItem* item = get_SelectedItem();
	if(item) {
		item->MenuEnter();
	}
}

void MenuSystem::MenuUp() {
	//Select the next item in the page
	MenuPage* page = get_CurrentPage();
	if(page==NULL) return;

	MenuItem* item = get_SelectedItem();
	if(item) {
		if( item->get_CaptureUpDown() ) {
			item->MenuUp();
			return;
		}
		DrawMenuItem(item, false);
	} 

	item = page->PrevItem();
	page->set_SelectedItem(item);
	DrawMenuItem(item, true);
}


void MenuSystem::MenuDown() {
	//Select the next item in the page
	MenuPage* page = get_CurrentPage();
	if(page==NULL) return;

	MenuItem* item = get_SelectedItem();
	if(item) {
		if( item->get_CaptureUpDown() ) {
			item->MenuDown();
			return;
		}
		DrawMenuItem(item, false);
	} 

	item = page->NextItem();

	page->set_SelectedItem(item);
	DrawMenuItem(item, true);
}


Menu_Back::Menu_Back() {
}

Menu_Back* Menu_Back::_instance = NULL;
Menu_Back* Menu_Back::get_Instance() {
	if(_instance==NULL) {
		_instance = new Menu_Back();
	}
	return _instance;
}


void Menu_Back::MenuEnter() {
	MenuSystem::get_Instance()->PopPage();
}

GotoPage::GotoPage(const char* name, MenuPage* page) {
	Name = name;
	Page = page;
}

void GotoPage::MenuEnter() {
	MenuSystem::get_Instance()->AddPage(Page);
}