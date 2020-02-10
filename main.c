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

void one_pulse(void)
{
  PORTA |= 0x05;
  _delay_ms(1000);
  PORTA = 0x00;
}
