#include "motor.h"
#include <avr/io.h>
#include <inttypes.h>

#define TIMER_VALUE 175
#define TIME_PERIOD 31

volatile unsigned char m1speed, m2speed, counter = 0;
volatile unsigned char m1dir, m2dir;

unsigned char stop_state = 0x05;

void run_motors()
{

}

SIGNAL(SIG_OVERFLOW0)
{
	TCNT0 = TIMER_VALUE;
	if (counter>=TIME_PERIOD)
	{
		counter=0xFF;
		stop_state = ((stop_state^0x0F) & 0x0F);
	}
	else if ((counter==m1speed) || (counter==m2speed))
	{
		// switch off the motor
		if (counter==m1speed)
		{	outb(PORTD, (inb(PORTD) | stop_state));	}
		if (counter==m2speed)
		{	outb(PORTB, (inb(PORTB) | stop_state));	}
	}
	else if (counter==0)
	{
		// switch on the motor
		outb(PORTD, (inb(PORTD) & 0xF0) | m1dir);
		outb(PORTB, (inb(PORTB) & 0xF0) | m2dir);
	}
	counter++;
}
