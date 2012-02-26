#include "EEPromLib.h"
#include <Arduino.h>
#include <avr/eeprom.h>
#include "PIDMachine.h"
#include "Encoder.h"

void InitStoredMemory() {
	uint8_t ver = eeprom_read_byte(ee_DATA_VERSION);

	if(ver!=CURRENT_VERSION) {
		//reset eeprom data
		Serial.println("Resetting eeprom");
		eeprom_write_byte(ee_TEMP_SET,140);
		eeprom_write_word(ee_PID_P, Encoder_ftoi(10));
		eeprom_write_word(ee_PID_I, Encoder_ftoi(0));
		eeprom_write_word(ee_PID_D, Encoder_ftoi(0));

		eeprom_write_byte(ee_DATA_VERSION, CURRENT_VERSION);
	}
}