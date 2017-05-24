/*************************************************************************
 *  Mikroprosessorsystemer                                               *
 *  Labøving 5.1 - Komparator                                            *
 *  NTNU IES - Rolf Kristian Snilsberg                                   *
 *                                                                       *
 *  Device: ATmega328P in Xplained Mini                                  *
 *************************************************************************/

/*	Program reads analog comparator output ands turns yellow LED on/off 
	according to output.
	Voltage on positive input is band-gap reference (1.1v) and AIN1/PD6 is
	input pin for external voltage. Test with potentiometer to 3V3.
 */


//	Includes
#include <avr/io.h>    // AVR I/O definitions. 



int main( void )
{  
	DDRB  = (1<<DDB5);      // Yellow LED on PB5 output

//	Configure comparator with internal bandgap reference (1.1v) on positive input.
	ACSR = (1<<ACBG);
	
	do {		
		if(ACSR & (1<<ACO)){
			// Turn off PB0 led if comparator output high
			PORTB &= ~(1<<PORTB5);  
		} else {
			// Turn on PB0 led if comparator output low
			PORTB |= (1<<PORTB5);
		}
	} while(1);

}

