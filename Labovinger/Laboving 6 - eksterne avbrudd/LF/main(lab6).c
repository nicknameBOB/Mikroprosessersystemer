/*
 * ext_pchange_interrupt.c
 *
 * Created: 01.03.2017 11.12.22
 * Author : rolfkrs
 */ 

#define F_CPU 16e6

#include <avr/interrupt.h>
#include <util/delay.h>

void INT_init(void);


int main(void)
{
	//initialization
	DDRB  = 0b00111111;  // PB0-5 outputs;
	PORTB = 0b00111111;  // Turn off active low leds
	
	INT_init();
	sei();  // enable global interrupts

	while(1)
	{
		_delay_ms(1000);
		PINB |= (1<<PINB5);
	}
}


/*	Enable INT0 on PD2 to negative flanks, and INT1 on PD3 and PCINT2/20 
	on PD4 to low level interrupts  */
void INT_init(void)
{
	PORTD = (1<<PORTD4)|(1<<PORTD3)|(1<<PORTD2); // Internal pull-up on PD2-4
	EIMSK = (1<<INT0) | (1<<INT1);  // Enable INT0 and INT1
	EICRA = (1<< ISC01);  // Negative flank trigging on INT0

//	Enable pin change interrupt on PD4
	PCICR = (1<<PCIE2);
	PCMSK2 = (1<<PCINT20);  
}


/*	External interrupt 0 on PD2. Toggles PB0*/
ISR(INT0_vect)
{
	PINB |= (1<<PINB0); // Toggle LED pin/bit 0 in PORTB register
}


/*	External interrupt 1 on PD3. Turns off PB1-4 */
ISR(INT1_vect)
{
	PORTB &= ~((1<<PORTB4)|(1<<PORTB3)|(1<<PORTB2)|(1<<PORTB1));
}


/*	Pin change interrupt 2, enabled for PCINT20 on PD4. Turns on PB1-4*/
ISR(PCINT2_vect)
{
	PORTB |= (1<<PINB4)|(1<<PINB3)|(1<<PINB2)|(1<<PINB1);
}

