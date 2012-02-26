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

typedef int16_t encoder_t;

/**
	The encoder
	
	provides a way of storing and incrementing a floating point
	number.

	Every number encoder_t (int16_t) maps to an output float.  Incrementing
	encoder_t increaes the floating point output by 2%-10% in a plesant
	round fashion.
**/

/** Converts an encoded int16_t number
	to its floating point definition **/
float	Encoder_itof(encoder_t d);

/** Conerts a floating point number
	to its closest encoded representation **/
encoder_t Encoder_ftoi(float f);

/** Returns the number of digits past the decimel point that are 
    filled in when representing the number **/
uint8_t	Encoder_digits(float f);