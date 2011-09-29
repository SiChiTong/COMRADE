#include "motor.h"
#include "init.h"
#include <avr/io.h>

/*
extern unsigned char m1speed;
extern unsigned char m2speed;
extern unsigned char m1dir;
extern unsigned char m2dir;
*/

int main(void)
{
	
	unsigned char start_flag = 0, old_sda = 1, add_flag = 1, add_level = 1;
	unsigned char sda, scl,start = 1, ctr = 0, start_bit = 0, stop_bit = 0, address_b = 0x00;
	unsigned char data1_a = 0x00, data2_a = 0x00, data1_flag = 0, data2_flag = 1;
	unsigned char data1_ctr = 0, data2_ctr = 0;
	
	stepper_direction = 1;
	angle_to_rotate = 1;
	steps_per_angle = 16;
	
	port_init();
	timer_init();

	for(;;)
	{
		
		sda=(PIND & 0x10)>>4;
		scl=(PIND & 0x20)>>5;

		if(scl==0)scl=1;
			else scl=0;


		if(scl==1)
		{
			if(!start)
			{
				if(sda!=old_sda)
				{	
					if(old_sda==1)
					{
						start_bit=1;
						stop_bit=0;
			
						ctr=0;
						data1_ctr=0;
						data2_ctr=0;
						add_flag=0;
						address_b=0x00;
						data1_a=0x00;
						data2_a=0x00;
						add_level=0;
						data1_flag=1;
						data2_flag=1;
					}
			
					else if(old_sda==0)
					{
						start_bit=0;
						stop_bit=1;
						add_level=0;
					}
				
					if(start_flag==1)
					{
						start_flag=0;
						//sbi(PORTD,6);
					}
				
					else if(start_flag==0)
					{
						start_flag=1;
						//cbi(PORTD,6);
					}
				}
			
				else if((start_bit==1) && (add_flag==0) && (start==0) && (add_level==1))
				{
					address_b += (sda & 0x01)<<(7-ctr);
					ctr++;
									
					if(ctr>=8)
						{
						add_flag=1;
						}		
					add_level=0;
				}
						
				else if((ctr!=8) && (start_bit==1) && (data1_flag==0) && (start==0) && (add_level==1))
				{
					data1_a += (sda & 0x01)<<(7-data1_ctr);
					data1_ctr++;
					
					if(data1_ctr>=8)
					{
						data1_flag=1;
					}		
					add_level=0;
				}
			
				else if((data1_ctr!=8) && (start_bit==1) && (data2_flag==0) && (data1_flag==1) && (start==0) && (add_level==1))
				{
					data2_a += (sda & 0x01)<<(7-data2_ctr);
					data2_ctr++;				
					if(data2_ctr>=8)
					{
						data2_flag=1;
					}		
					add_level=0;
				}
	
				else if((ctr==8) && (start_bit==1) && (start==0) && (add_level==1))
				{	add_level=0;data1_flag=0;ctr++;	}
				else if((data1_ctr==8) && (start_bit==1) && (start==0) && (add_level==1))
				{	add_level=0;data2_flag=0;data1_ctr++;	}
			}
		}
	
		else
		{	add_level=1;	}

		start=0;
		
		old_sda=sda;
		

//		if(stop_bit==1)
		if((address_b==0x19) && (stop_bit==1))
		{
			SREG &= 0x7F;
			/*
			outb(PORTD, ((inb(PORTD)&0xF0)|(0x0F&data1_a)));
			outb(PORTB, ((inb(PORTB)&0xF0)|(0x0F&(data1_a>>4))));
			*/
			
			// check for instruction
			if((data1_a & 0x80) == 0x80)
			{
				steps_per_angle = data1_a & 0x7F;
				timer_value = 0xFF - (16 * ((data2_a&0x0F) + 1) );
			}
			else
			{
				angle_to_rotate = data1_a & 0x7F;
				if(angle_to_rotate == 0)
				{
					outb(PORTD, (inb(PORTD) | 0x0F));
					outb(PORTB, (inb(PORTB) | 0x0F));
				}
	
				if((data1_a & 0x80) == 0)
					stepper_direction = 1;
				else
					stepper_direction = -1;
				
				//run_motors();
				
				SREG |= 0x80;
				stop_bit = 0;
			}
		}
	}
	return 0;
}
