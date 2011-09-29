#include "init.h"
#include <avr/io.h>

void port_init(void)
{
	// set portb/d.0 to portb/d.3 as output
	//also set as high to turn off h-bridge
	PORTB = 0x0F;
	PORTD = 0x0F;
	DDRB |= 0x0F;
	DDRD |= 0x0F;
	
	DDRD |= 0x4F;
	DDRB |= 0x10;
}

void timer_init(void)
{
	//set up the timer0
	TCNT0 = 0x00;		// counter value
	SREG |= 0x80;		// enable global interrupt bit
	TIMSK = 0x02;		// enable timer0
	TCCR0 = 0x02;		// load prescaler to run counter at clk/8 -> 0.8us resolution
	/*
	// set up counter1
	TCNT1H = 0x03;
	TCNT1L = 0x00;
	OCR1AH = 0x02;
	OCR1AL = 0x00;
	TCCR1B = 0x0D;
	
	TIMSK = 0x42;		// enable tcnt1 in compare mode, and tcnt0 in free running mode
	*/
}
