/*
 * laboving1.c
 *
 * Created: 12.01.2016 10:13:36
 *  Author: rolfkrs
 */ 


/* Defines */
#define F_CPU 16000000UL            // Tell delay.h the cpu frequency 16MHz

/* Includes */
#include <avr/io.h>                 // Defines pins, ports, etc
#include <util/delay.h>             // Functions to waste time


/*	Blinking led program for use on ATmega328P in Arduino UNO.
	Will blink yellow led or external led connected to PB5 pin. */
int main(void)
{
	// Inititalization
	DDRB = 1<<DDB5;                // 0b00100000 - PB5 output - other PB pins inputs
	PORTB = 1<<PORTB5;             // PB5 high - turn on led

	// Main loop
	do {
		PORTB ^= 1<<PORTB5;       // Toggle LED pin/bit 5 in PORTB register
		_delay_ms(1000);            // wait 1 sec
	} while (1);                    // End main loop

//	return (0);            // Never reached/not needed
}

