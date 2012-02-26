#include "RollingAverageF.h"
#include <stdlib.h>

RollingAverageF::RollingAverageF(int16_t numItemsAverage) {
	numItems = numItemsAverage;
	pos = 0;
	filled = false;
	data = (float*)malloc(sizeof(float)*numItems);
}

RollingAverageF::~RollingAverageF() {
	free(data);
}

void RollingAverageF::Add(float datum) {
	data[pos] = datum;
	pos = (pos+1)%numItems;
	if(pos==0) filled = true;
}

float RollingAverageF::get_Average() {
	int16_t count = filled ? numItems : pos;
	float total=0;
	for(int16_t idx=0;idx<count;idx++) {
		total += data[idx];
	}
	return (float)(total/count);
}