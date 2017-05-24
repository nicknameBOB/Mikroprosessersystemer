/*****************************************************************************
 *  Mikroprosessorsystemer                                                   *
 *  Øving med servo og adc                                                   *
 *  NTNU/IES - Rolf Kristian Snilsberg                                       *
 *                                                                           *
 *  Device: ATmega328P                                                       *
 *****************************************************************************
 */


//	Defines 
#define F_CPU 16000000UL     // 16MHz system clock     
#define T1_PRESCALE 8
// servo period = 20ms = 20000us
#define SERVO_PERIOD  F_CPU/1000000*20000/T1_PRESCALE 
// maximum servo should be = 2ms = 2000us, but is 2150 after testing servo 
#define SERVO_MAX     F_CPU/1000000*2150/T1_PRESCALE   
// minimum should be = 1ms = 1000us, but is actually 460 us after testing servo
#define SERVO_MIN     F_CPU/1000000*460/T1_PRESCALE

// Include definition file(s)
#include <avr/interrupt.h>

// Prototypes
void ADC_Init(void);
void Timer1_PWM_init(void);


/*	Main program. */
int main( void )
{
	ADC_Init();
	Timer1_PWM_init();


/*	Endless loop. Starts ADC measurement and waits for it to finish
	Adjusts servo signal according to adc measurement. */
	do {
		ADCSRA |= (1<<ADSC);  // Start adc conversion.
		do {} while (ADCSRA & (1<<ADSC));  // wait for it to finish
//		OCR1A = ((ADCH*12)+SERVO_MIN);  // easy aproximation of servo
		OCR1A = ADC*(SERVO_MAX-SERVO_MIN)/1024+SERVO_MIN;
 	} while(1);
}

void Timer1_PWM_init(void)
{

	DDRB  = (1<<DDB1);  // Set OC1A/PB1 as output.
	/* Fast PWM non-inverting mode. Clear OC1A on Compare Match, set at TOP=ICR1. 
	Prescale 16Mhz by 8 -> 2MHz = 0.5us/tick */
	TCCR1A  = (1<<COM1A1)|(0<<COM1A0)|(1<<WGM11)|(0<<WGM10); 
	TCCR1B  = (1<<CS11)|(1<<WGM13)|(1<<WGM12);

	ICR1 = SERVO_PERIOD;
//	OCR1A = SERVO_MIN;  // Manual setting to 0° for testing
//	OCR1A = (SERVO_MAX+SERVO_MIN)/2;  // Manual setting to 90° for testing
// 	OCR1A = SERVO_MAX;  // Manual setting to 180° for testing
}


/*!	ADC initialization. Prescaler 128x, resulting in 125kHz adc clock @ 16MHz.
	ADC0 input. AVCC as reference. Result left-adjusted.
 */
void ADC_Init(void)
{
//	ADCSRB = 0x00;       // Free running mode.
	ADMUX  = (1<<REFS0)|(0<<ADLAR); // ADC0 single ended input and result left adjusted.
	DIDR0 |= (1<<ADC0D); // digital input buffer disable, saves power.
	ADCSRA = (1<<ADEN)|(7<<ADPS0);
}

