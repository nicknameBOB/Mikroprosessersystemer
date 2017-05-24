/*
 * GccApplication15.c
 *
 * Created: 08.03.2017 13.14.47
 * Author : Mikolaj Jaworski
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#define F_CPU 16000000UL


int main(void)
{
	DDRB = (1 << DDB1); // Set B-register as output.
	
	//TCCR1A = 0b01010000; // Toggle OC1A on compare match.
	//TCCR1B = 0b00001101; // 1024x prescaler and CTC mode with OCR1A top.
	//OCR1A = 15625; // top value to give 500ms period.
	
	TCCR1A = (1<<COM1A1)|(1<<WGM10);
	TCCR1B = (1<<WGM12) | (1<<CS11) | (1<<CS10);
	OCR1A = 0x0f;
    while (1) 
    {
    }
}

