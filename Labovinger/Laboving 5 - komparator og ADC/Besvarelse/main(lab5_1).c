/*
 * GccApplication9.c
 *
 * Created: 15.02.2017 13.10.41
 * Author : Mikolaj Jaworski
 */ 


//Oppgave 2

#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>




int main(void)
{
	
	DDRD = 0xff; //Set "D" register as output.
	ADMUX = 0b01100010; //Set reference voltage to be AVCC,enable ADLAR, and set ADC2 as input.
	ADCSRA = 0b10000111; //Enable ADC, and set prescaler to 128.
	
    while (1) 
    {
		ADCSRA |= (1<<ADSC); //Start ADC converting.
		while  (ADCSRA & (1<<ADSC)){}//Waiting till converting is done.
		PORTD = ADCH;//Print ADCH-value to LED's.
		
		
    }
}

