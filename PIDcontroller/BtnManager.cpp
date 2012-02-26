#include "BtnManager.h"
#include <Arduino.h>

int BtnManager::pins[] = BTN_PINS;

BtnManager* BtnManager::_instance = NULL;
BtnManager* BtnManager::get_Instance() {
	if(_instance==NULL) {
		_instance= new BtnManager();
	}
	return _instance;
}


BtnManager::BtnManager() {
	
	for(int idx=0;idx<NUM_BUTTONS;idx++) {
		pinMode(pins[idx], INPUT);
		btnState[idx]=digitalRead(pins[idx]);
		//set button in pull up mode
		digitalWrite(pins[idx],HIGH);
		btnStateCnt[idx]=0;
	}
}

void BtnManager::Loop() {
	for(int idx=0;idx<NUM_BUTTONS;idx++) {
		bool curState = digitalRead(pins[idx]);

		if(curState==btnState[idx]) {
			//one more cycle at this state
			btnStateCnt[idx]++;

			if(btnState[idx]==false && btnStateCnt[idx]>REPEAT_CYCLE) {
				//Serial.print("Click [repeat]");
				btnStateCnt[idx]=0;
				if(btnClickListners[idx]) {
					btnClickListners[idx]();
				}
			}
		} else {
			//change in state
			if(curState==true && btnStateCnt[idx]>=MIN_CYCLE_DEBOUNCE ) {
				//click event
				//Serial.print("Click ");
				//Serial.print(pins[idx]);
				//Serial.print(" ");
				//Serial.print(btnStateCnt[idx]);
				//Serial.print("\n");

				if(btnClickListners[idx]) {
					btnClickListners[idx]();
				}
			}

			btnState[idx]=curState;
			btnStateCnt[idx]=0;
		}

		
	}
}