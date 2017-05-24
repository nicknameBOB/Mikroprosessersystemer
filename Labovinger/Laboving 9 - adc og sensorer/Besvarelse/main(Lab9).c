#define F_CPU 16000000UL
#define USART_BAUDRATE 9600 // desired baud rate
#define UBRR_VALUE (((F_CPU / (USART_BAUDRATE * 16UL))) - 1) // UBRR value
/******************************|Includes|********************************/
#include <avr/interrupt.h>
#include <stdio.h>
/***********************************************************************/
/************************|function prototypes|**************************/
void usart_putchar( char data );
void USART_init(uint16_t ubrr_value);
/***********************************************************************/
static FILE uart_str = FDEV_SETUP_STREAM(usart_putchar, NULL, _FDEV_SETUP_WRITE); // for printf to work

int FLAG = 0;

void INTERRUPT_INIT()
{
	OCR1A = 15625; // set Timer count.
	TCCR1B = (1<<WGM12)|(1<<CS12); // Set prescaler to 256, set CTC-mode.
}

void ADC_INIT ()
{
	ADMUX = (1<<REFS0) ; // Set Vcc as ref voltage.
	ADCSRA = (1<<ADEN) | (1<<ADPS0) | (1<<ADPS1) | (1<<ADPS2); // Enable converter, and set prescale.
}


int main(void)
{
	USART_init(UBRR_VALUE);
	

	DDRC = 0x00;
	// port c is now input, pinc0 is adc0
	stdout = &uart_str;
	// to make printf work
	ADC_INIT ();
	INTERRUPT_INIT();
	while (1)
	{
		if(TIFR1 & (1<<OCF1A)) // look for flag
		{
			TIFR1 |= (1<<OCF1A); // reset flag.
			printf("ADC data = %4d\r",ADC); // print data.
			ADCSRA |= (1<<ADSC); //Start ADC converting.
			
		}
		
	}
}
/***************************************************************************/
void usart_putchar(char data) {
	while ((UCSR0A & (1 << UDRE0)) == 0) {};
	// Wait for empty transmit buffer
	UDR0 = data;
	// Start transmission
}
/********************************|usart init|*******************************/
void USART_init(uint16_t ubrr_value)
{
	UBRR0 = ubrr_value;
	// set baud rate to 9600
	UCSR0C = ((1<<UMSEL01)|(3<<UCSZ00));
	// 8 bit 1 parity
	UCSR0B = ((1<<TXEN0)|(1<<RXEN0));
	// enable transmitter, reciever and interrupt
}