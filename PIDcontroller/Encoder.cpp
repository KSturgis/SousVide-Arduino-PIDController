#include "Encoder.h"


uint8_t		numBreaks = 12;
float		breakPoint[] = { 0.0000f, 0.010f, 0.050f, 0.10f, 0.50f, 1.00f, 5.0f, 10.0f, 50.0f, 100.0f, 500.0f, 1000.0f };
float		breakDelta[] = { 0.0005f, 0.001f, 0.005f, 0.01f, 0.05f, 0.10f, 0.5f,  1.0f,  5.0f,  10.0f,  50.0f,  100.0f };
uint8_t		breakDigits[]= {       4,      3,      3,     2,     2,     1,    1,     0,     0,      0,      0,       0 };

		

float Encoder_itof(encoder_t d) {
	if (d < 0) {
		return -1.0f * Encoder_itof(-1*d);
	}

	encoder_t accum = 0;
	uint8_t n;
	for (n=0; n < numBreaks-1; n++) {
		//increment accum by the number of integers between breakPoint[n+1] and breakPoint[n]
		encoder_t delta = (encoder_t)((breakPoint[n + 1] - breakPoint[n]) / breakDelta[n] + 0.5f);
		if (d < accum+delta) {
			break;
		}

		accum += delta;
	}
			
	//At point n in the break calculation 
	// calulate output 
	return breakPoint[n] + (d-accum) * breakDelta[n];
}

encoder_t Encoder_ftoi(float f) {
	if (f < 0) {
		return -1 * Encoder_ftoi(-1.0f * f);
	}

	encoder_t accum = 0;
	uint8_t n;
	for (n = 0; n < numBreaks-1; n++) {
		if (f < breakPoint[n+1]) {
			break;
		}
		//increment accum by the number of integers between breakPoint[n+1] and breakPoint[n]
		accum += (encoder_t)((breakPoint[n + 1] - breakPoint[n]) / breakDelta[n] + 0.5f);
	}


	return accum + (encoder_t)((f - breakPoint[n]) / breakDelta[n]+0.5f);
}


uint8_t Encoder_digits(float f) {
	if (f < 0) {
		return Encoder_digits(-1.0f * f);
	}

	uint8_t n;
	for (n = 0; n < numBreaks - 1; n++) {
		if (f < breakPoint[n + 1]) {
			break;
		}
	}
	return breakDigits[n];
}