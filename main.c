#include <avr/io.h>
#include <util/delay.h>

#define F_CPU 8000000UL

void one_pulse(void);
void two_pulses(void);
void multiple_pulses(void);




int main(void)
{
  // configure ports
  DDRD = 0xf0;
  DDRA |= 0x07;

  while(1)
  {
    PORTA = 0x01;
  }
}

void one_pulse(void)
{

}

void two_pulses(void)
{

}

void multiple_pulses(void)
{

}
