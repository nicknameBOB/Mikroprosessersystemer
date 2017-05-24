/*
 * Laboving4pullup.c
 *
 * Created: 08.02.2017
 * Author : Jorgen
 */ 
					
#define F_CPU 16000000UL			// 16Mhz


#include <avr/io.h>				// AVR
#include <util/delay.h>			// delay
#define TRUE 1
#define FALSE 0

int8_t flag = FALSE;
uint8_t magicCounter = 0;

uint8_t lysVerdi = 0x0F;  
//en funksjon som skriver til lysene min som om det var et register

void pinWrite(void)
{
		PORTD =  (lysVerdi & ~(0x03)); // Legger verdiene på PORTD 2 til 7
		PORTB = (lysVerdi & ~(0xFC));  // Legger verdiene på PORTB 1 og 2
}
//det som skjer når du trykker på knappen
void coolstuff()
{
		

		 lysVerdi ^= 0xFF;
		 pinWrite();


}
//Hva som skjer når knappen er holdt inne
void coolerstuff()
{	
		lysVerdi = 0;
		pinWrite();
		
		for (int8_t i = 0; i < 8; i++)
			{
				lysVerdi |= (1<<i);
				pinWrite();
				_delay_ms(200);
			}
		for (int8_t i = 7; i > 3; i--)
		{
				lysVerdi &= ~(1<<i);
				pinWrite();
				_delay_ms(200);
		}
		


}
//knappfuksjonen
void button(void)
{
	
	if ((!(PINB & 0x80)))
	
	{
		if (!(flag))
		{
			coolstuff();
			flag = TRUE;
		}
		_delay_ms(200);
		magicCounter += 1; //Teller av hvor lenge kanppen er holdt inne 
	}
	else
	{
		flag = FALSE;
		magicCounter = 0;
	}
	//når knappen er holdt inne i 2 sekunder skjer dette
	if (magicCounter>9)
	{
		coolerstuff();
	}
}
int main(void)
{	
	
				DDRD = 0xFC;				// set Port D fra 2 til 7
				DDRB = 0x03;				// set port D 0 og 1
				PORTB = 0x80;				//setter ingangen til pull up
 
    while (1) 
    {
	 button(); //kaller button funksjon
	 
    }
}

