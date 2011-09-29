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
	
	unsigned char prevm2dir,prevm1dir;

	/*
	m1speed=0x10;
	m2speed=0x10;
	prevm1dir=m1dir=FORWARD;
	prevm2dir=m2dir=FORWARD;
	*/
	
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
						sbi(PORTD,6);
					}
				
					else if(start_flag==0)
					{
						start_flag=1;
						cbi(PORTD,6);
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
		
		if(address_b==0x39 && stop_bit==1)			
		{
/*			outb(PORTD, ((inb(PORTD)&0xF0)|(0x0F&data1_a)));
			outb(PORTB, ((inb(PORTB)&0xF0)|(0x0F&(data1_a>>4))));*/

			SREG &= 0x7F;
			
			m1speed = (data1_a & 0x7F)>>2;
			m2speed = (data2_a & 0x7F)>>2;
			
			if((data1_a & 0x80) == 0)
				m1dir = FORWARD;
			else
				m1dir = REVERSE;
			
			if((data2_a & 0x80) == 0)
				m2dir = FORWARD;
			else
				m2dir = REVERSE;
			
			if (prevm1dir!=m1dir)
			{
				outb(PORTD, (inb(PORTD)|STOP));
			}
			if (prevm2dir!=m2dir)
			{
				outb(PORTB, (inb(PORTB)|STOP));				
			}
			prevm1dir=m1dir;
			prevm2dir=m2dir;

			SREG |= 0x80;
			stop_bit = 0;
		}
	}
	return 0;
}
