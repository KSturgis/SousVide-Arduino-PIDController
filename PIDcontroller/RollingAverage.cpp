#include "RollingAverage.h"

RollingAverage::RollingAverage() {
	pos = 0;
	filled =false;
}


void RollingAverage::Add(AVG_TYPE datum) {
	data[pos] = datum;
	pos = (pos+1)%NUM_AVERAGE_ITEMS;
	if(pos==0) filled = true;
}


AVG_TYPE RollingAverage::get_Average() {
	int16_t count = filled ? NUM_AVERAGE_ITEMS : pos;
	AVG_TYPE total=0;
	for(int16_t idx=0;idx<count;idx++) {
		total += data[idx];
	}
	return (AVG_TYPE)(total/count);
}