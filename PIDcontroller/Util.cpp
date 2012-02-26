#include "Util.h"

char numPlace[16];
char format[10];

const char* formatFloat(float f, uint8_t iDigits, uint8_t fDigits) {
	
	double fp,ip;
	bool neg;
	char* s;
	float mult;

	s = numPlace;
	neg = f<0;
	f= abs(f);
	
	
	if(neg) {
		//print -
		s[0]='-';
		s++;
		iDigits--;
	}

	
	ip = modf(f, &fp);
	
	//Print the integer part
	sprintf(format, "%%%dd", iDigits);
	sprintf(s,format,(int)fp);
	s+= strlen(s);

	if(fDigits>0) {
		//Print the fractional part
		sprintf(format, ".%%0%dd", fDigits);
		mult=10;
		for(uint8_t i=1;i<fDigits;i++) {
			mult*=10;
		}
		sprintf(s,format,(int)(ip*mult));
	} 

	return numPlace;
}


//
//extern void __bss_end;
extern void *__brkval;

int get_free_memory()
{
  int free_memory;
  
  /*if((int)__brkval == 0)
    free_memory = ((int)&free_memory) - ((int)&__bss_end);
  else*/
    free_memory = ((int)&free_memory) - ((int)__brkval);

  return free_memory;
}