/*************************************************************************
 *  Mikroprosessorsystemer                                               *
 *  Labøving 3 - Oppgave 1 EEPROM handling                               *
 *  Høgskolen i Sør-Trøndelag - Rolf Kristian Snilsberg                  *
 *                                                                       *
 *  Device: ATmega2560							                         *
 *************************************************************************/


#define EE_START_LOCATION 0x01   // Starting position for eeprom writing
#define EE_END_LOCATION 0x10       // Ending location for eeprom writes. 


//	Includes
#include <avr/eeprom.h>  


//	Prototypes.
void EEPROM_write(uint16_t iAddress, unsigned char cValue);
unsigned char EEPROM_read(uint16_t iAddress);


/*	Main function. It will write values to locations defined above of the EEPROM
	It will first use own functions then avrlib c functions 
	It's generic for most AVRs. */
int main( void )
{
    uint8_t ee_value = 5;
	uint16_t ee_address = EE_START_LOCATION; 


	// Own functions writes
	for(ee_address=EE_START_LOCATION ; ee_address<=EE_END_LOCATION ; ee_address++){
		EEPROM_write(ee_address, ee_value );
		ee_value++;
	}

	// Avrlib C function writes
	for(ee_address=EE_START_LOCATION ; ee_address<=EE_END_LOCATION ; ee_address++){
		/* Locations in eeprom are addresses and avr gcc thus want pointers. Either 
		   we define variables in eeprom or we cast address values */
		eeprom_write_byte( (uint8_t *)ee_address, ee_value );
		ee_value++;
	}

	// Own function read
	PORTA =  EEPROM_read(0x07 );

	// Avrlib C function read
	PORTB =  eeprom_read_byte( (uint8_t *)0x0B);

    do {  // Eternal loop/Main program.
		asm("nop");  // for debugging purposes
	} while(1);
}




/*	Write one character to eeprom at given adress. Blocking, so will wait until 
	eeprom is ready for access before continuing with writing. Assumes interrupts 
	disabled. */
void EEPROM_write(uint16_t address, uint8_t data)
{
    while (EECR & (1<<EEPE));   // Wait until any previous eeprom write is finsihed
    EEAR  = address;            // Load eeprom adress
    EEDR  = data;               // Load eeprom data
    EECR |= (1<<EEMPE);         // Master write enable
    EECR |= (1<<EEPE);          // Write enable, within 4 cycles of previous bit setting.
}


/*	Read one character from at given adress. Blocking, so will wait until 
	eeprom is ready for access before continuing with reading. */
uint8_t EEPROM_read(uint16_t address)
{
    while (EECR & (1<<EEPE));   // Wait until any previous eeprom write is finsihed
    EEAR   = address;         	// Load eeprom adress
    EECR   |= (1<<EERE);         // Read enable
    return EEDR;             	// Return eeprom data
}

