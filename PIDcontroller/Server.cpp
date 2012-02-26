#include "Server.h"
#include <Arduino.h>
#include "PIDMachine.h"

//These are helper functions passed the the
// Tasks system and btnManager
void loopTemp();
void loopPIDUpdate();




Server* Server::_instance = NULL;
Server* Server::get_Instance() {
	if(_instance==NULL) {
		_instance = new Server();
	}
	return _instance;
}

Server::Server() {
}

void Server::Initialize() {
	//Create the PID system
	PIDMachine* machine = PIDMachine::get_Instance();
	
	//Initalize the user interface
	View->Initialize();

	//Start the task scheduler
	TaskQueue* Tasks = TaskQueue::get_Instance();
	//Update the temperature reading.  Temperature
	// readings are average over a number of readings
	// to reduce the effect of digital noise
	Tasks->EnqueueTask( loopTemp, 1000, 500 );

	//Update the PIDMachine.  The Update calculates a 
	// new process value.  The PMW updates every 4000ms
	// the process update will occure about 500 ms before
	// then.  The PMW will only change output at the
	// beginning of each 4 second window.  So we run the update
	// 500ms before then.  A better system would be to use an
	// intereupt on PWM1 to garentee that the the update always
	// happens before the update.  In practice the task system
	// doesn't drift and the task always fires before the PWM
	// update
	Tasks->EnqueueTask( loopPIDUpdate, 3500, 4000 );
	
	//Startup the ouput device.  The PMW varies the output
	// to the heating system
	SetupPWM();
	//Turn off the output
	SetDuty(0.0f);

	//Update with initial parameters
	machine->UpdatedParameters();
}

void Server::SetProcessValue(float outputParameter) {
	float param = PEG(outputParameter,0,100);
	SetDuty(param/100);

	View->onProcessUpdate(param);
}





void Server::SetupPWM() {
	ICR1 = CYCLE_PERIOD;	//4 second period
	OCR1B = 0;				//Turn off output for now

	TCCR1A = 0;				//clear control register A 
	DDRB |= _BV(PORTB2);	//Set Port B2 as an output
	TCCR1A = 0x20;			//set output B to, PWM Phase and Frequency correct, TOP set in ICR1
	TCCR1B = 0x15;			//PWM phase and frequency mode, Clock scale (N) 1024
	TCCR1C = 0x00;			//Clear
	TIMSK1 = 0x00;			// Disable all interrupts		
	
	//TIMSK1 = 0x20			//Enable ICIE1 interrupt (180 deg out of pahse with
	//						// PWM value change,  interrupt on TOP change on BOTTOM)
}

void Server::Loop() {
	TaskQueue* Tasks = TaskQueue::get_Instance();
	int sleep = Tasks->Loop();

	if(sleep>MAX_SLEEP) {
		sleep = MAX_SLEEP;
		Serial.println(F("Max Sleep"));
	}

	if(sleep>0) {
		delay(sleep);
	}
}


void Server::SetDuty(float d) {
	d = PEG(d,0.0F,1.0F);
	uint16_t  duty= (uint16_t)(CYCLE_PERIOD*d);
	//Here we write the 16bit PWM duty to the settings
	// but we have to disable interrupts to prevent an interrupt
	// while the two bytes are written.
	//Disble interrupts
	uint8_t sBack = SREG;
	cli();
	//Set OCR1
	OCR1B = duty;
	//Enable interrupts again
	SREG = sBack;
}

void loopTemp() {
	TempSensor1.Update();
	float avgTemp = TempSensor1.get_AvgTempF();

	Server::get_Instance()->View->onTempUpdate(avgTemp);
}

void loopPIDUpdate() {
	float tempF = TempSensor1.get_AvgTempF();

	PIDMachine::get_Instance()->Update(
		tempF,
		4);
}


