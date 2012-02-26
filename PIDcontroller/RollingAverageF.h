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

/**
	RollingAverageF

	Calculates a rolling average of numbers.  The last numItemsAverage
	numbers sent to Add() are averaged together and returned whe
	get_Average() is called
**/
class RollingAverageF {
public:
	RollingAverageF(int16_t numItemsAverage);
	~RollingAverageF();

	void Add(float datum);

	float get_Average();
private:
	int16_t pos;
	int16_t numItems;
	bool filled;
	float* data;
};