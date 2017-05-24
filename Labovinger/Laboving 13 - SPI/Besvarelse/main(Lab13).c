/*
 * SPI_Atmega328PB.c
 *
 * Created: 26.04.2017 20:47:25
 * Author : Jørgen Steen
 */ 

#include <avr/io.h>         // A part of all AVR-programming for convenient programming
#define F_CPU 16000000UL     //The clock is 16Mhz
#include <util/delay.h>     //Delay Library
#include <string.h>
#include <math.h>
#define USART_BAUDRATE 9600 // desired baud rate
#define BAUD_PRESCALE (((F_CPU / (USART_BAUDRATE * 16UL))) - 1) // UBRR value


//Defining my own boolean commands
#define TRUE 1
#define FALSE 0

//bitmask for the SPI communication
#define SS_bm (1<<2) 
#define MOSI_bm (1<<3)
#define MISO_bm (1<<4)
#define SCK_bm (1<<5)
#define SPI_receiving_flag (!(SPSR0 & (1<<SPIF)))

/* '
****** Choose if it is master or slave  ******
*/
#define Master FALSE //Is this the master device?



void UART_init(void)
{
	UBRR0 = BAUD_PRESCALE; // Set UBBR according to system clock and desired baud rate
	UCSR0B = (1 << RXEN0) | (1 << TXEN0); // Turn on the transmission and reception circuitry
}

//Initiates SPI. One thing for master or slave.
void SPI_init(void)
{
	if (Master)
	{
		DDRB |= SS_bm | MOSI_bm | SCK_bm; //Set SPI outputs as Output
		SPCR0 |=  (1<<SPR0) | (1<<MSTR) |(1<<SPE);
	}
	else
	{
			DDRB = MISO_bm;
			SPCR0 =  (1<<SPE);
	}
}

//Transmite 1 char.
void USART_Transmit(char data)
{
	// Wait for empty transmit buffer
	while (!( UCSR0A & (1<<UDRE0)));
	// Put data into buffer, sends the data
	UDR0 = data;
	//while (!( UCSR0A & (1<<TXEN0))); //Wait until data is sent
}

//SPI Transmition function
void SPI_transmit(uint8_t SPI_value)
{
	SPDR0 = SPI_value;
	USART_Transmit(SPI_value);
}

//SPI recieve function
uint8_t SPI_receive(void)
{
	while (SPI_receiving_flag);
	return SPDR0; 
}

//Uart recieve function
char USART_recieve(void)
{
	
	while (!(UCSR0A & (1<<RXC0)))	{}
	char uart_in = UDR0;
	
	return(uart_in);
}

int main(void)
{
	SPI_init(); //initiate SPI values
	UART_init(); //Initiates Uart

    while (1) 
    {
		//If master it reads UART and send with SPI, if slave it reads SPI and send with UART
		if (Master)
		{	
			char UART_in = USART_recieve();
			SPI_transmit(UART_in);
		}
		else if (!Master)
		{
			uint8_t SPI_in = SPI_receive();
			USART_Transmit(SPI_in);
		}
		else
		{
			char Not_defined[] = "?";
			USART_Transmit(*Not_defined);
			_delay_ms(100);
		}

    }
}

