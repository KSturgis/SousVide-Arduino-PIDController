#include "ColorLCD.h"


#include <Arduino.h>
#include "BtnManager.h"
#include "ColorMenu.h"
#include "MenuSystem.h"
#include "PIDMenu.h"
#include "SetPointMenu.h"
#include "TaskQueue.h"
#include "TempSensor.h"
#include "Util.h"

void loopInput();
void menu_up();
void menu_down();
void menu_enter();

#define SPLIT F("\t")

ColorLCD::ColorLCD() {
	lcd = NokiaLCD::get_Instance();
}


 void ColorLCD::Initialize() {
	if(lcd) {
		Serial.println(F("Begin LCD setup"));
		lcd->Init();	    //Initialize the LCD
		lcd->Contrast(40);
		lcd->Clear(WHITE);    // Clear LCD to a solid color 
		lcd->DrawLine(0,120,131,120,RED);
		lcd->DrawRect(0,0,131,17,true,BLACK);
		lcd->WriteStrF(PSTR(" Sous Vide"), 1,1, CYAN, BLACK);

		lcd->DrawRect(0,MENU_ROWY(4),131,MENU_ROWY(5)-1, true, DEBUG1_COLOR);
		lcd->DrawRect(0,MENU_ROWY(5),131,MENU_ROWY(6)-1, true, DEBUG2_COLOR);
		lcd->DrawRect(0,MENU_ROWY(6),131,131,true,DEBUG3_COLOR);

		drawError();

		Serial.print(("LCD Inited..\n"));
	}

	SetupMenu();

	TaskQueue* Tasks = TaskQueue::get_Instance();
	Tasks->EnqueueTask( loopInput, 40, 40 );
}


void ColorLCD::SetupMenu() {
	Serial.print(F("Setup Menu\n"));
	MenuSystem* menu = MenuSystem::get_Instance();

	//note: memory leak
	
	MenuPage* settingsPage = new MenuPage();
	settingsPage->Items[0] = new GotoPage("Color", &ColorMenu1);
	settingsPage->Items[1] = new GotoPage("PID", PIDPage::get_Instance());
	settingsPage->Items[2] = Menu_Back::get_Instance();

	MenuPage* basePage = new MenuPage();
	basePage->Items[0] = SetPointMenu::get_Instance();
	basePage->Items[1] = new GotoPage("Settings",settingsPage);


	menu->AddPage(basePage);

	//Add the button listeners to the setupMenu
	BtnManager* btn=BtnManager::get_Instance();
	btn->btnClickListners[0] = menu_up;
	btn->btnClickListners[1] = menu_enter;
	btn->btnClickListners[2] = menu_down;
}

void ColorLCD::onProcessIDChange(bool idCalcEnabled) {
	Serial.print(F("ID_CHANGE("));
	Serial.print(idCalcEnabled);
	Serial.println(F(")"));
}


void ColorLCD::onParameterUpdate(float setPoint, float kP, float kI, float kD) {
#ifdef DEBUG_PARAMETER_UPDATE	
	PIDMachine* pid = PIDMachine::get_Instance();
	Serial.print(F("paramUpdate:"));
	Serial.print(SPLIT);
	Serial.print(setPoint);
	Serial.print(SPLIT);
	Serial.print(kP);
	Serial.print(SPLIT);
	Serial.print(kI);
	Serial.print(SPLIT);
	Serial.println(kD);
#endif
}

void ColorLCD::onProcessUpdate(float param) {
	PIDMachine* pid = PIDMachine::get_Instance();
	float setTemp = pid->get_SetPoint();
	float currentTemp = TempSensor1.get_AvgTempF();

	

	int dP,dI,dD;
	dP=(int)(pid->get_Kp()*pid->get_Error()+0.5f);
	dI=(int)(pid->get_Ki()*pid->get_Integral()+0.5f);
	dD=(int)(pid->get_Kd()*pid->get_Derivative()+0.5f);


#ifdef DEBUG_PROCESS_UPDATE
	//Send debug information
	Serial.print("PIDUpdate:");
	Serial.print(SPLIT);
	Serial.print(currentTemp);
	Serial.print(SPLIT);
	Serial.print(pid->get_Error());
	Serial.print(SPLIT);
	Serial.print(pid->get_Integral());
	Serial.print(SPLIT);

	float derivative = pid->get_Derivative();
	Serial.print(derivative,4);
	Serial.print(SPLIT);
	Serial.print(dP);
	Serial.print(SPLIT);
	Serial.print(dI);
	Serial.print(SPLIT);
	Serial.print(dD);
	Serial.print(SPLIT);
	Serial.print((int)(param+0.5f));
	Serial.print(SPLIT);
	Serial.print((int)(millis()/1000.0f+0.5f));
	Serial.println("");
#endif 

	sprintf(lcdBuf, " O:%3d%%", (int)param);
	lcd->WriteStr(lcdBuf, 
		9*8, MENU_ROWY(4),
		DEBUG1_TXT_COLOR, DEBUG1_COLOR);

	sprintf(lcdBuf, "P%s%s",
		pid->get_IDCalulationEnabled() ? "ID": "",
		pid->get_IntegralOverflowError() ? "-E": "  ");
	lcd->WriteStr(lcdBuf, 
		0, MENU_ROWY(5),
		DEBUG2_TXT_COLOR, DEBUG2_COLOR);

	sprintf(lcdBuf, "%d/%d/%d    ", dP, dI, dD);
	lcd->WriteStr(lcdBuf, 
		0, MENU_ROWY(6),
		DEBUG3_TXT_COLOR, DEBUG3_COLOR);

	float delta = currentTemp-setTemp;
	if(delta>3*MAX_TEMP_VARIANCE) {
		drawTempHigh();
	} else if ( delta>1*MAX_TEMP_VARIANCE ) {
		drawTempNearHigh();
	} else if ( delta<-1*MAX_TEMP_VARIANCE ) {
		drawTempNearLow();
	} else if ( delta<-3*MAX_TEMP_VARIANCE ) {
		drawTempLow();
	} else {
		drawTempSet();
	}
}

void ColorLCD::onTempUpdate(float tempF) {
#ifdef DEBUG_TEMP_UPDATE
	Serial.print(F("TempUpdate:"));
	Serial.print(SPLIT);
	Serial.println(tempF);
#endif

	sprintf(lcdBuf, "T:%sF ", formatFloat(tempF,3,1));
	lcd->WriteStr(lcdBuf, 
		0, MENU_ROWY(4), 
		DEBUG1_TXT_COLOR, DEBUG1_COLOR);
}



void ColorLCD::drawError() {
	lcd->DrawRect(ICON_BOX,true,RED);
	lcd->DrawCircle(ICON_CIRCLE_CENTER,10,WHITE,FULLCIRCLE);
	lcd->DrawCircle(ICON_CIRCLE_CENTER,9,WHITE,FULLCIRCLE);
	lcd->DrawCircle(ICON_CIRCLE_CENTER,8,WHITE,FULLCIRCLE);
}

void ColorLCD::drawTempLow() {
	lcd->DrawRect(ICON_BOX,true,ICON_COLOR_LOW);
	lcd->DrawRect(ICON_BOX,false,WHITE);
}

void ColorLCD::drawTempHigh() {
	lcd->DrawRect(ICON_BOX,true,ICON_COLOR_HIGH);
	lcd->DrawRect(ICON_BOX,false,WHITE);
}

void ColorLCD::drawTempNearLow() {
	lcd->DrawRect(ICON_BOX,true,ICON_COLOR_NEAR_LOW);
	lcd->DrawRect(ICON_BOX,false,WHITE);
}

void ColorLCD::drawTempNearHigh() {
	lcd->DrawRect(ICON_BOX,true,ICON_COLOR_NEAR_HIGH);
	lcd->DrawRect(ICON_BOX,false,WHITE);
}

void ColorLCD::drawTempSet() {
	lcd->DrawRect(ICON_BOX,true,ICON_COLOR_SET);
	lcd->DrawRect(ICON_BOX,false,WHITE);
}


void loopInput() {
	BtnManager::get_Instance()->Loop();
}

void menu_up() {
	//Serial.print(F("UP\n"));
	MenuSystem* menu = MenuSystem::get_Instance();
	menu->MenuUp();
}

void menu_enter() {
	//Serial.print(F("ENTER\n"));;
	MenuSystem* menu = MenuSystem::get_Instance();
	menu->MenuEnter();
}

void menu_down() {
	//Serial.print(F("DOWN\n"));
	MenuSystem* menu = MenuSystem::get_Instance();
	menu->MenuDown();
}