/*****************************************************************************
 *  Mikroprosessorsystemer                                                   *
 *  Labøving 7                                                               *
 *  NTNU IES - Rolf Kristian Snilsberg				                         *
 *                                                                           *
 *  Device: ATmega328                                                        *
 *****************************************************************************
 */

/*	The application will first setup the 16-bit TCNT1 to give out a 1 HZ 
	heartbeat on the OC1A pin, i.e. PB1. It will then wait for a low input 
	on PD2 to change mode. The second mode is to give out a varying PWM on 
	OC1A with frequency 122Hz @ 1MHz clock. Applying this signal to an LED 
	will increase its light intensity until OCR wraps and then start from 
	0 again. It's generic for most AVRs but is run on a ATmega328P.
*/


//	Defines 
#define F_CPU 16000000UL     // 16MHz system clock     
#define TOGGLE_DELAY_MS 500 // Delay between each toggle of led in ms.
#define T1_PRESCALER_A 1024    // Timer 1 prescaler for task a)


// Include definition file(s)
#include <avr/interrupt.h>
#include <util/delay.h>



/*	Main program. */
int main( void )
{
	// Set OC1A/PB1 as output.
	DDRB  = (1<<DDB1);
	PORTD = (1<<PORTD2); // pullup on PD2

/*	Task 1
	Clear timer on compare match; top is then OCR1A. Toggle OC1A each time.
	Prescaler is 256, giving a range of 1*256/16MHz = 256us or 3.9kHz, to 
	2^16*256/1MHz = 16.8s or ~0.06Hz. 500 ms is 0.5s * 1MHz / 256 = 1953  */
	TCCR1A = (1<<COM1A0);
	TCCR1B = (1<<CS12)|(1<<CS10)|(1<<WGM12);
	OCR1A =  (((F_CPU / T1_PRESCALER_A) * TOGGLE_DELAY_MS) / 1000 ); // Order to avoid overflow in compiler calculations
	 
	// Wait for input to change to mode 2.
	do { } while ( PIND & (1 << PIND2) );    // Wait for button to be pressed.

/*	Task 2    
	8bit fast PWM inverting mode (256 steps). Set OC1A on Compare Match, clear at TOP. 
	Prescale by 8. 16MHz / (256*256)  = 244 Hz. */
	DDRB  |= (1<<DDB2);  // OC1B/PB2 pin as output also
	TCCR1A  = (1<<COM1B1)|(1<<COM1B0)|(1<<COM1A1)|(1<<COM1A0)|(1<<WGM10); 
	TCCR1B  = (1<<CS11)|(1<<WGM12);
//	Enable timer 1 overflow to increase duty cycle periodically.
// 	TIMSK1 = (1<<TOIE1);
// 	TIFR1 |= (1<<TOV1);
//	sei();


/*	Endless loop. Increases the duty cycle each PWM_DELAY_MS millisecond. 
	Delay in ms between each increase of duty cycle is 1000ms/16 = 62.5ms */
	#define PWM_DELAY_MS 63
	enum direction {DOWN=0, UP} count_direction;    
	uint8_t pwm_value[] = {0,1,2,3,5,7,10,15,22,31,44,63,90,127,180,255};
	uint8_t i = 0;
	count_direction = UP;

	do {
  		OCR1A = pwm_value[i];
//  		OCR1A++;
 		OCR1B = pwm_value[15-i];
 		_delay_ms(PWM_DELAY_MS);
		if(count_direction == UP)
		{
			i++;
			if (i >=15 ) {
				count_direction = DOWN;
			}
		} else {
			i--;
			if (i == 0 ) {
				count_direction = UP;
			}							
		}
	} while(1);
}


/*	The compare match interrupt could also be used for increasing the duty cycle
	which would not block the main loop and only be run when needed. It needs to 
	be adapted to the prescaler. This interrupt code assumes 8x prescaler, i.e
	8*256*1us ~= 2ms between each. */
ISR(TIMER1_OVF_vect)
{
	static uint8_t count=0;
	count++;
	if(count >= 4) {
		count = 0;
		OCR1A++;
		OCR1B--;
	}
}

