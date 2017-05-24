//oppgave 1 

#define F_CPU 16000000UL // 16 MHz clock speed
#include <avr/io.h>
#include <util/delay.h>


int main(void)
{
	DDRD = 0b00001111;	  /* define the led pins*/
	ACSR = (1<<ACBG);     /* initialize reference comparator on our input pin */
	
	while (1)
	{
		if (ACSR & (1<<ACO))
		PORTD = 0b00001111;
		else {
			PORTD = 0b00000000;
		}
	}
}
