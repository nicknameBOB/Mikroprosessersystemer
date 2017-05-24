/*
 * lab10_mikroprosessorsystemer_oppg_2.c
 *
 * Created: 29.03.2017 13:54:48
 *  Author: Haakenl
 */ 


#define F_CPU 16000000UL

/******************************|Includes|********************************/
#include <avr/interrupt.h>
#include <stdio.h>
/***********************************************************************/


int main(void)
{	
	volatile int8_t int8var = -200; // overflow - warning
	volatile uint8_t uint8var = 100;
	volatile int16_t int16var = -5000;;
	volatile uint16_t uint16var = 2000;;
	volatile int32_t int32var = 2000*5000; // overflow in expression- warning
	
	
	volatile float floatvar = 3.5;
	volatile float floatvar2 = 4.7;
	
	while (1)
	{
		int32var = 2000/5000*200;
		int16var = int8var * uint8var;
		
		int8var++;
		uint16var++;
		int16var++;
		int32var++;
		
		floatvar = floatvar * uint8var;
		floatvar = floatvar/floatvar2;
	}
}