#include "MenuPage.h"
#include <Arduino.h>

MenuPage::MenuPage() {
	for(int i=0;i<MAX_NUM_MENU_ITEMS;i++) {
		Items[i]=0;
	}
	selectedIdx=-1;
}

int8_t MenuPage::GetIdx(MenuItem* item) {
	if(item!=NULL) {
		for(uint8_t i=0;i<MAX_NUM_MENU_ITEMS;i++) {
			if(Items[i]==item) return i;
		}
	}
	return -1;
}

MenuItem* MenuPage::get_SelectedItem() {
	if(selectedIdx==-1) return NULL;
	return Items[selectedIdx];
}

void MenuPage::set_SelectedItem(MenuItem* item) {
	selectedIdx = GetIdx(item);
}

int8_t MenuPage::get_SelectedIdx() {
	return selectedIdx;
}

void MenuPage::set_SelectedIdx(int8_t idx) {
	selectedIdx=idx;
}

MenuItem* MenuPage::NextItem() {
	int8_t sIdx = get_SelectedIdx();
	if(sIdx==-1) return Items[0];

	while(1) {
		sIdx = (sIdx+1)%MAX_NUM_MENU_ITEMS;
		if(Items[sIdx]!=NULL) {
			return Items[sIdx];
		}
	}
}

MenuItem* MenuPage::PrevItem() {
	int8_t sIdx = get_SelectedIdx();
	if(sIdx==-1) return Items[0];

	while(1) {
		sIdx = (sIdx-1);
		if(sIdx<0) sIdx = MAX_NUM_MENU_ITEMS-1;

		//Serial.print(" "+sIdx);
		if(Items[sIdx]!=NULL) {
			return Items[sIdx];
		}
	}
}