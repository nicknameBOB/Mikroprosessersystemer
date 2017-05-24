/*
 * lab10_mikroprosessorsystemer.c
 *
 * Created: 29.03.2017 12:06:54
 *  Author: Haakenl
 */ 


#define F_CPU 16000000UL 
#define USART_BAUDRATE 9600 // desired baud rate 
#define UBRR_VALUE (((F_CPU / (USART_BAUDRATE * 16UL))) - 1) // UBRR value


/******************************|Includes|********************************/ 
#include <avr/interrupt.h> 
#include <stdio.h> 
/***********************************************************************/

/************************|function prototypes|**************************/ 
static int usart_putchar( char data, FILE *stream ); 
void USART_init(uint16_t ubrr_value); 
/***********************************************************************/

static FILE uart_str = FDEV_SETUP_STREAM(usart_putchar, NULL, _FDEV_SETUP_WRITE); // for printf to work

int main(void) 
{ 
	USART_init(UBRR_VALUE); 
	stdout = &uart_str; // to make printf work
	
	printf("char %d\r\n",sizeof(char));
	
	while (1) 
	{
		
	} 
}
	

/***************************************************************************/ 
static int usart_putchar(char data, FILE *stream) 
{ 
	while ((UCSR0A & (1 << UDRE0)) == 0) {}; // Wait for empty transmit buffer 
	UDR0 = data; // Start transmission 
	return 0; 
}

/********************************|usart init|*******************************/ 
void USART_init(uint16_t ubrr_value) 
{ 
	UBRR0 = ubrr_value; // set baud rate to 9600 
	UCSR0C = ((1<<UMSEL01)|(3<<UCSZ00)); // 8 bit 1 parity 
	UCSR0B = ((1<<TXEN0)|(1<<RXEN0)); // enable transmitter, reciever and interrupt 
}