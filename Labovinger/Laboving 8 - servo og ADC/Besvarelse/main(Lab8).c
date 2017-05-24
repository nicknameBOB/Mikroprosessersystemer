/*
 * GccApplication18.c
 *
 * Created: 15.03.2017 12.56.51
 * Author : Mikolaj Jaworski
 */ 
#define F_CPU 16e6
#include <avr/io.h>

#include <util/delay.h>

uint16_t Scaler(void)
{
	uint16_t result;
	
	result = (((uint32_t)ADCH*435)/255)+115 ; //Scale ADCH to match servo pulses from 0-180 deg(115-550).
	
	return result;	// return the maped ADCH value.
}




int main(void)
{
	TCCR1A |= (1<<COM1A1)|(1<<COM1B1)|(1<<WGM11); // non inverting PWM
	TCCR1B |= (1<<WGM13)|(1<<WGM12)|(1<<CS11)|(1<<CS10);
	
	ADMUX = 0b01100010; //Set reference voltage to be AVCC, enable ADLAR, and set ADC2 as input.
	ADCSRA = 0b10000111; // Enable ADC, and set prescaler to 128.
	
	ICR1 = 4999; // Set the input capture register to 50 Hz.
	DDRB = (1<<PB1); // Set PB1 as output
	
    
    while (1) 
    {
		ADCSRA |= (1<<ADSC); // Start conversion.
		while (ADCSRA & (1<<ADSC)){} // Wait until conversion is done.
		
		OCR1A = Scaler();	// Set OCR1A to Scaler value.
	
		
    }
}

