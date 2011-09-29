#include "motor.h"
#include <avr/io.h>
#include <inttypes.h>

volatile unsigned char timer_value = 192;//135;
volatile unsigned char angle_to_rotate = 0;
volatile char stepper_direction;
volatile unsigned char steps_per_angle = 16;

volatile unsigned char steps_moved = 0;

const unsigned char stepper_coil_state[8] = {0x06, 0x06, 0x0F, 0x09, 0x09, 0x09, 0x0F, 0x06};//, 0x06, 0x06, 0x0F, 0x09, 0x09, 0x09, 0x0F, 0x06};

unsigned char coil_ptr1 = 0, coil_ptr2 = 6;

void run_motors()
{
	if(angle_to_rotate == 0)
	{
		//outb(PORTD, (inb(PORTD) | 0x0F));
		//outb(PORTB, (inb(PORTB) | 0x0F));
		//TCCR0 = 0;
		return;
	}
	
	//coil_ptr1 = (coil_ptr1 + stepper_direction)%8;
	//coil_ptr2 = (coil_ptr2 + stepper_direction)%8;
	coil_ptr1 += stepper_direction;
	coil_ptr2 += stepper_direction;
	
	if(coil_ptr1 == 8)
		coil_ptr1 = 0;
	else if(coil_ptr1 == 0xFF)
		coil_ptr1 = 7;
	
	if(coil_ptr2 == 8)
		coil_ptr2 = 0;
	else if(coil_ptr2 == 0xFF)
		coil_ptr2 = 7;	
	
	// activate coils
	if(stepper_coil_state[coil_ptr1] == 0x0F)
	{	outb(PORTD, (inb(PORTD) | 0x05));	}
	else
	{	outb(PORTD, (inb(PORTD) & 0xF0) | stepper_coil_state[coil_ptr1]);	}
	
	if(stepper_coil_state[coil_ptr2] == 0x0F)
	{	outb(PORTB, (inb(PORTB) | 0x05));	}
	else
	{	outb(PORTB, (inb(PORTB) & 0xF0) | stepper_coil_state[coil_ptr2]);	}
	
	// increment pointers
	steps_moved++;
	
	if(steps_moved >= steps_per_angle)
	{
		steps_moved = 0;
		angle_to_rotate--;
	}
	
	TCNT0 = timer_value;
	//TCCR0 = 0x03;
}

SIGNAL(SIG_OVERFLOW0)
{
	run_motors();
}
