;
; laboving_assembly.asm
;
; Created: 05.04.2017 11.28.15
; Author : rolfkrs
;

//	#include <m328pdef.inc>  // inkluderes automatisk og treng ikke lenger
	ldi r16, 99

//	Oppgave 1 - dataoverføring
	ldi r17,3      // immediate, 3 -> r17
	ldi r18,5      // immediate, 5 -> r18
	mov r0,r17     // register direct, kopierer innhold i r17 til r0

	in  r0,0x3F    // i/o direct. SREG -> r0. Tregere/større alternativ: lds r0, 0x5F
	out 0x3F,r17   // i/o direct, r16 -> SREG

	sts SRAM_START,r17  // Data direct. r17 -> første lokasjon i SRAM (0x200 for ATmega2560).
	lds r0, SRAM_START  // Data direct. Første lokasjon i SRAM -> r0

	st  X+,r16     // Lagre r16 der X peker (r28:r27= 0x0000 = r0) og postinkrementer X 
	ld  r0,X       // Last det X peker på (X = 0x0001 = r1) -> r0


//	Oppgave 2 - addisjon.
//	Oppgave a)
	ldi  r16,127  //
	ldi  r17,129  // -127 signed/two's complement
	add  r16,r17
/*	Sum er egentlig 256 desimalt/0x100 hex men får ikke plass i 8 bit så mente/
	carry blir satt. Resultatet også korrekt ved signed verdier. Resultatet er 
	null så Zero-flag blir satt. Half carry blir satt fordi det er mente fra 
	bit 3 (for BCD aritmetikk). */

//	Oppgave b)
	ldi  r16,80
	ldi  r17,48
	add  r16,r17
/*	Resultatet er 128 desimalt/0x80 hex, men kan også tolkes som -128 signed. 
	Negative flag (N) blir satt pga. at bit 7 (msb) er satt etter addisjonen.
	Two’s complement overflow flag blir satt pga at bit 7 i ingen av tallene 
	er satt fra før, dvs at resultatet har endret fortegn og er feil hvis vi 
	regner tallene som signed. S bitet sier også at resultat ikke er negativt.*/

//	Oppgave c)
	ldi  r16,128  // -128 signed
	ldi  r17,40
	add  r16,r17
/*	Resultatet er 168 desimalt/0xA8 hex, men kan også tolkes som -88 signed. 
	Negative flag (N) blir satt pga av at bit 7 (msb) er satt etter addisjonen.
	S bitet blir satt og sier at resultatet er negativt (hvis vi regner signed). 
	Resultat er korrekt uansett om vi regner tallene som signed eller unsigned.*/


//	Oppgave e)
	ldi  r16,low(8166)
	ldi  r17,low(840)
	add  r16,r17
	ldi  r17,high(8166)
	ldi  r18,high(840)
	adc  r17,r18
/*	Først adderes de lave bytene uten mente, så de høye med mente. Resultatet 
	ligger i r17:r16 og er 0x232E = 9006 som er korrekt. */



//	Oppgave 3 - løkke 4 klokkesykler som kjøres 5 ganger.
	sbi  DDRB,DDB5    // Set PB0 as output.

	ldi r16,5 // 1 klokkesyklus
delay_loop:
	dec r16 // 1 klokkesyklus
	nop // 1 klokkesyklus
	brne delay_loop // 2 klokkesykler nå hoppet tas, 1 ellers.
	nop

/*	The main loop will run continously. The inner loop takes 4 clock cycles,
	and repeating it 250 times will give a delay of 1000 clock cycles, and
	additional overhead will bring this to 1004. Repeating this 249 times
	will give a delay of 249996 cycles, and additional overhead will bring this
	to 250000. Repeating this 4 times will give a delay of one million clock 
	cycles, accounting for a 1s delay at 1MHz. All loops used decrementation 
	of registers, and checks if the result is zero to decide if to stop or 
	continue.
	An easier to tune alternative is shown below.
*/ 
main_loop:
/*	Using nested loops to make a delay. */
// Outer loop takes (249996+4)*16=16000000 clock cycles
	ldi  r18,64     // Number of times the outer loop will be run.
outer_loop:
//	Middle loop takes (1000+4)*249 = 249996 clock cycles
	ldi  r17,249   // Number of times the middle loop will be run.
middle_loop:
//	Inner loop takes 250*4=1000 clock cycles
	ldi  r16,250   // Number of times the inner loop will be run.
inner_loop:
	nop            // 1 clock cycle to get 4 total
	dec  r16       // 1 clock cycle
	brne inner_loop  // 2 clock cyles if jump taken, 1 otherwise

	nop            // 1 extra clock cycle to come close to 250 000 total
	dec  r17       // 1 clock cycle
	brne middle_loop // 2 clock cyles if jump taken, 1 otherwise

	nop            // 1 extra clock cycle to come to 250 000 total
	dec  r18       // 1 clock cycle
	brne outer_loop // 2 clock cyles if jump taken, 1 otherwise

	sbi  PINB,PINB5  // Toggle PB0
rjmp main_loop


/*	Alternative delay using several registers to hold number of loops.
	ldi takes 1*3=3 cycles. I.e. 1000000-3= 999997 subi+2*sbci takes 3 cycles. 
	brne takes 2 clock cycles when taken, 1 when not. So a total of 5 clock 
	cycles each loop, while last takes 4. 999997/5 = 199999.4, i.e loop needs 
	to be run 199999 times and missing 5*0.2+1 = 3 clock cyles.
*/
#define alt_delay 199999

alternative_main_loop:
	ldi  r16,byte1(alt_delay)  //Load immediate. 1 clock cycle 
	ldi  r17,byte2(alt_delay)  //Load immediate. 1 clock cycle 
	ldi  r18,byte3(alt_delay)  // Load immediate. 1 clock cycle 
delay_1mill_cycles:
	subi r16,0x01   // subtract 1. 1 clock cycle
	sbci r17,0x00   // subtract carry. 1 clock cycle 
	sbci r18,0x00   // subtract carry. 1 clock cycle 
	brne delay_1mill_cycles  // jump if result != 0, 2 cycles if taken, 1 otherwise
	nop
	nop
/*	The test works because Z-flag remains unchanged when the result is zero 
	from sbc/sbci. Z is cleared otherwise. All bytes are thus zero when brne is
	not taken. This loop is exactly 1 million cycles, but the toggling and rjmp 
	takes 3 cycles too. Could remove 2*nop to make it 1000001 cycles total. */
	sbi  PINB,PINB0  // Toggle PB0. 1 cycle
	rjmp alternative_main_loop  // 2 cycles. Start new delay loop.

//	Eksempel på uendelig løkke som hindrer kontroller i å gå ut i tomt programminne
loop:
//	application code
    rjmp loop
