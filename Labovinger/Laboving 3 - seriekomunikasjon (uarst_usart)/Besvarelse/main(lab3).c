/*
 * GccApplication9.c
 *
 * Created: 01.02.2017 12.55.08
 * Author : jonat
 */ 

#define F_CPU 16000000UL // System clock
#define USART_BAUDRATE 9600 // desired baud rate
#define BAUD_PRESCALE (((F_CPU / (USART_BAUDRATE * 16UL))) - 1) // UBRR value

#include <avr/io.h>



/* Simple loopback UART program. */
int main(void)
{
	DDRD = 0b11111100;// deacler port D2-D7 as outputt
	DDRB = 0b00000011;// deacler port B0-B1 as outputt
	
	UBRR0 = BAUD_PRESCALE; // Set UBBR according to system clock and desired baud rate
	UCSR0B = (1 << RXEN0) | (1 << TXEN0); // Turn on the transmission and reception circuitry
	while(1)
	{
		
		do {} while(!(UCSR0A & (1<<RXC0))); // Wait for incoming data
		/* Next line outputs received data. This is possible due to UDR being separate
		registers for transmission and reception*/
		
		UDR0 = UDR0;
		
		PORTD = (UDR0 << 2); //display uart asxcii value as 8 bit led indicators 
		PORTB = (UDR0 >> 6); //display uart asxcii value as 8 bit led indicators
		}
}