#define F_CPU 1000000UL
#include <util/delay.h>
#include <stdlib.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/atomic.h>
#define keypad_PORT PORTD
#define keypad_DDR DDRD

void one_pulse(void);
ISR(INT0_vect) // the interrupt code on INT0
{
	PORTC|=0x7F;
	_delay_ms(1000);
}
int main(void)
 { // configure ports 
	DDRD = 0xF0;
	PORTD =0X0F; 
	DDRA|= 0x07; 
	int x , correct=0 , count=0;
	GICR|=0x40; // enable external Interrupt INT0
	MCUCR=0x02; // configure INT0 falling edge triggered
	sei(); // enable global interrupts by setting global interrupt enable bit in SREG=0x80;
  while(1)
  { 
	  while(1) 
	   { 
		 x= PIND;
		 if (x!=0X0F) 
		   {
			break;
		   }
	   }
	PORTD=0xBF;
	if (PIND==0xBE)
	{ 
		count=1;
		
		if (PIND==0xBD)
		{
			count=2;
			if (PIND==0xBB)
			{
				count=3;
				PORTD=0xDF;
				if (PIND==0xDE)
				{
					correct=1;
					count=4;
				}
			}
		}
		if((count==4) & (correct==1))
		{
			x=1;
		}
	}
  }
 } 
void one_pulse(void)
{
	PORTA |= 0x05;
	_delay_ms(1000);
	PORTA = 0x00;
}
