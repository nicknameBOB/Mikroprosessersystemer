/*
 * GccApplication5.c
 *
 * Created: 25.01.2017 13.12.23
 * Author : Mikolaj Jaworski
 */ 
#define F_CPU 16000000UL //Set clock frequency to 16MHz

#include <avr/io.h>  //include the AVR IO library.
#include <util/delay.h> //include the delay library.
#define DELAYTIME 2  // Define the delay time to be 2 ms.

#define LED_PORT        PORTD  //Define the port "D" at the atmegaA328P to be LED port.
#define LED_DDR         DDRD // Define the "D" register as LED register.

uint8_t invaderData1[] = {
	 //This array sets LED_PORT. Displays figure one
	  0b01110000,
	  0b00011000,
	  0b11111101,
	  0b10110110,
	  0b00111100,
	  0b00111100,
	  0b00111100,
	  0b10110110,
	  0b11111101,
	  0b00011000,
	  0b01110000
	
	
};

uint8_t invaderData2[] = {
	//This array sets LED_PORT. Displays figure two
	 0b00001110,
	 0b00011000,
	 0b10111101,
	 0b01110110,
	 0b00111100,
	 0b00111100,
	 0b00111100,
	 0b01110110,
	 0b10111101,
	 0b00011000,
	 0b00001110
	 
};

//Pause function witch pauses for 2*5 ms.
void pause(void){
	uint8_t i;
	for (i=0; i<5; i++)
	{
		_delay_ms(DELAYTIME);
	}
}

//Function goes through arrays in povData. When one figure displayes it pauses for 10 ms.
void POVDisplay(uint8_t povData[], uint8_t numberRows){
	uint8_t i;
	for (i = 0; i < numberRows; ++i){
		LED_PORT = povData[i];
		_delay_ms(DELAYTIME);
		}
	pause();	
}



int main(void)
{
    /* Set all LED's to outputs */
	LED_DDR = 0xff;
	
    while (1) //main loop.
    {
		POVDisplay(invaderData1, sizeof(invaderData1));
		POVDisplay(invaderData2,sizeof(invaderData2));
}
	
return 0;
	
}

