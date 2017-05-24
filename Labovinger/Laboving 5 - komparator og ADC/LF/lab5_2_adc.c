/*************************************************************************
 *  Mikroprosessorsystemer                                               *
 *  Labøving 5.2 - ADC                                                   *
 *  NTNU IES - Rolf Kristian Snilsberg                                   *
 *                                                                       *
 *  Device: ATmega328P in Xplained Mini                                  *
 *************************************************************************/

/*	Setting VTarget to 5.5V and AREF0 to 5.12V will give a step size of 
	5120mV/256 = 20mV. 
 */

// Defines

//	Includes
#include <avr/io.h>    // AVR i/o definitions.

//	Prototypes.
void ADC_Init(void);

int main( void )
{  
	// PORTD output. Leds active low
	DDRD  = 0xFF;
// 	PORTD = 0x00;  // test leds

	ADC_Init();

	do { // endless loop
		ADCSRA |= (1<<ADSC);  // Start adc conversion.
		do {} while (ADCSRA & (1<<ADSC));  // wait for it to finish
		PORTD = ~ADCH;
	} while(1);

}


/*!	ADC initialization. Prescaler 128x, resulting in 125kHz adc clock @ 16MHz.
	ADC0 input. AVCC as reference. Result left-adjusted.
 */
void ADC_Init(void)
{
//	ADCSRB = 0x00;       // Free running mode.
	ADMUX  = (1<<REFS0)|(1<<ADLAR); // ADC0 single ended input and result left adjusted.
	DIDR0 |= (1<<ADC0D); // digital input buffer disable, saves power.
	ADCSRA = (1<<ADEN)|(7<<ADPS0);
}

