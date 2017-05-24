/* This is the Fokking HEADER!
 * GccApplication12.c
 *							Labbøving 6 interøpt
 * Created: 01.03.2017 12.46.35
 * Author : Jonathan Jørgen Mikolaj
 */ 

#define F_CPU 16000000UL // 16 MHz clock speed
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

/*-----------------------------------------------------------------------------------------*/
ISR(INT0_vect){
	PORTB ^= (0b00011111);  // Toggle LED pin/bit 0-4 in PORTB register
}
ISR(INT1_vect){
	PORTB &= ~(0b00011111); // turn on LED pin/bit 0-4 in PORTB register
}
ISR(PCINT2_vect){
	if(!(PIND & (1<<PIND4))) {	// if the pinD4 is pushed
		PORTB |= (0b00011111);	// Set pin B0 and B1 low.
	}
}
/*-----------------------------------------------------------------------------------------*/
void initInterrupt0(void) {
	EIMSK |= (1 << INT0);		  // enable INT0 
	EICRA |= (1 << ISC01);		  // trigger when falling flank button
}
void initInterrupt1(void) {
	EIMSK |= (1 << INT1);		  // enable INT1 
	EICRA |= (1 << ISC11);		  // trigger when falling flank button 
}
void initInterrupt2(void){
	PCICR = (1<<PCIE2);				 // Activate Interrupt Enable on PCIE2
	PCMSK2 = (1<<PCINT20);			 // Activate pin PD4 as interrupt 
	
	//PCICR = (1<<PCIE2);				// enable interrupt PCIE2.
	//PCMSK2 = (1<<PCINT20);			// define PCINT20 (PD4 as interrupt.)
}
/*-----------------------------------------------------------------------------------------*/
int main(void)
{
	DDRD = 0x00;		// Set all pinD's as input.
	
	//initialization
	DDRB = 0b00111111;			   	 // PB0-5 outputs;
	PORTB = 0b00111111;				 // Turn off active low led's
	PORTD = (1<<PORTD2)|(1<<PORTD3)|(1<<PORTD4); // Internal pull-up on PD2, PD3 and PD4
		
	initInterrupt0();	// gets interrupt to work as intended 0
	initInterrupt1();	// gets interrupt to work as intended 1
	initInterrupt2();	// gets interrupt to work as intended 2
	sei();				// set (global) interrupt enable bit 
	
	while(1){	
		_delay_ms(500);
		PINB |= (1<<PINB5);		//  LED5 blinks 
	}
}