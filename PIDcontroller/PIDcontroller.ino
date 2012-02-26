#include "PIDController.h"

//Views
#include "Headless.h"
#include "ColorLCD.h"


Server* mServer;
//uint8_t reserve[128*4];

//************************************************************************
//					Main Code
//************************************************************************
void	setup()
{
	//Setup the serial port for debug messages
	Serial.begin(115200);
	Serial.print(F("Booting..\n"));
	//Verify that the stored process variables are correct
	// or reset to factory defaults
	InitStoredMemory();
	
	//reserve some memory for last resort future use
	//reserve[0]=0;

	//Setup the server system
	mServer = Server::get_Instance();
	
	//Use this view if you have the Nokia Color LCD shield
	// From SparkFun
	mServer->View = new ColorLCD();

	//Use this view if you don't have a screen for user input
	// All updates will be sent to the USB connection and 
	// all parameters can be changed by sending commands to
	// the arduino via the USB connnection.
	//mServer->View = Headless::get_Instance();

	//Startup the PID controller
	mServer->Initialize();
	Serial.print(F("Running..\n"));
}


void loop() {
	mServer->Loop();
}



