#include <avr/io.h>
#define F_CPU 1000000UL
#include <util/delay.h>

void one_pulse(void);

int main(void)
{
  // configure ports
  DDRD = 0xf0;
  DDRA |= 0x07;

}
#include <avr/io.h>
#define F_CPU 1000000
#include <util/delay.h>

void one_pulse(void);
void search (void);
int r1=0, x=0, c1=0 , output;
char database[4][4]= {1, 2, 3, 'A', 4, 5, 6, 'B', 7, 8, 9, 'C', '*', 0, '#', 'D'};


int main(void)
{
	
	// configure ports
	DDRD = 0xf0;
	DDRA |= 0x07;
	PORTD =0X0f;
	

	
while(1)
{
		
		while (1)
		{
			x=PIND;
			if (x!=0x0f)
			{
				break;
			}
		}
		PORTD = 0xef;
		x=PIND;
		if (x!=0xef)
		{
			r1=0;
			search();
		}
		PORTD = 0xdf;
		x=PIND;
		if (x!=0xdf)
		{
			r1=1;
			search();
		}
		PORTD = 0xbf;
		x=PIND;
		if (x!=0xbf)
		{
			r1=2;
			search();
		}
		PORTD = 0x7f;
		x=PIND;
		if (x!=0x7f)
		{
			r1=3;
			search();
		}
}
	
}

void one_pulse(void)
{
	PORTA |= 0x05;
	_delay_ms(1000);
	PORTA = 0x00;
}
void search (void)
{
	x = x & 0X0f;
	
	if (x==0X0e)
	{
		output = database[r1][0];
		
	}
	if (x==0X0d)
	{
		output = database[r1][1];
	}
	if (x==0X0b)
	{
		output = database[r1][2];
	}
	if (x==0X07)
	{
		output = database[r1][3];
	}

	
}
void one_pulse(void)
{
  PORTA |= 0x05;
  _delay_ms(1000);
  PORTA = 0x00;
}
