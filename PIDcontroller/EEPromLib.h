/*****************************************************************************
    SousVide PIDController
    Copyright ©2012 Ken Sturgis

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
******************************************************************************/
#pragma once
#include <inttypes.h>


typedef uint8_t*	EEPROMLoc8_t;
typedef uint16_t*	EEPROMLoc16_t;
typedef uint32_t*	EEPROMLoc32_t;

#define ee_BASE			0x100
#define ee_DATA_VERSION	(EEPROMLoc8_t) (ee_BASE)
#define ee_TEMP_SET		(EEPROMLoc8_t) (ee_BASE+0x01)
#define ee_PID_P		(EEPROMLoc16_t)(ee_BASE+0x02)
#define ee_PID_I		(EEPROMLoc16_t)(ee_BASE+0x04)
#define ee_PID_D		(EEPROMLoc16_t)(ee_BASE+0x06)

//Changing this version will cause the system to reset to 
// factory settings (InitStoredMemory)
#define CURRENT_VERSION 0x04

/**
	Checks that ee_DATA_VERSION == CURRENT_VERSION
	(indicating the memory has been properly initialized.)

	If not the system will reset all the parameters to
	factory default.
**/
void InitStoredMemory();