#include<avr/io.h>
#define	F_CPU	1000000
#include<util/delay.h>
#include<string.h>


#define		KEYPAD_PORT		PORTC
#define		KEYPAD_PIN		PINC

#define		LCD_DATA_PORT		PORTB
#define 	LCD_CONT_PORT		PORTD
#define 	LCD_RS 		PD0
#define 	LCD_RW 		PD1
#define 	LCD_EN 		PD2


/*4X4 Keypad Function Declarations*/
unsigned char read_keypad(void);

/* One pulse on buzzer and yellow LED */
void one_pulse(void);

/*Alphanumeric LCD Function Declarations*/
void lcd_data_write(char data);
void lcd_command_write( char command);
void lcd_init();
void lcd_string_write( char *string);


int main(void)
{
	DDRA=0x0f;
	/* Yellow LED, Green LED, Buzzer and relay is set as outputs */

	DDRB=0xff;
	/*All the 8 pins of PortB are declared output (data pins of LCD are connected)*/

	DDRD=0x07;
	/*PD0, PD1 and PD2 pins of PortD are declared output (control pins of LCD are connected)*/

	DDRC=0x0f;
	/*PortC's upper 4 bits are declared input and lower 4 bits are declared output(4x4 Keypad is connected)*/

	PORTC=0xff;
	/*PortC's lower 4 bits are given high value and pull-up are enabled for upper 4 bits*/

	unsigned char keypad_value, count=0, password_status=0, attempts = 0;
	char password_set[6], password_entered[6];
  /*Variable declarations*/

	password_set[0] = 49;
	password_set[1] = 49;
	password_set[2] = 49;
	password_set[3] = 49;
	password_set[4] = 63;
	password_set[5] = 0;
	/* ASCII Values */
	/* Password = 1111D */

	lcd_init();
	lcd_string_write("Gwargy on fire");

  lcd_command_write(0xc0);
	/*Cursor moves to 2nd row 1st column of LCD*/

	lcd_string_write(";)");
	_delay_ms(1000);

	while (1)
	{
		lcd_command_write(0x01);
		/*Clear screen*/

		lcd_string_write("Enter Password");

		/*While loop for password entry and checking*/
		while(password_status==0x00 && attempts != 3)
		{
			lcd_command_write(0xc0);
			/*Cursor moves to 2nd row 1st column of LCD*/

			/*While loop for 4 digit password entry*/
			while(count<5)
			{
				keypad_value=read_keypad();
				/*Scan's 4X4 keypad and returns pressed key value or default value*/

				/*Checking if any key is pressed or not*/
				if(keypad_value != 0xff)
				{
					password_entered[count]=keypad_value+48;
					/*Storing the pressed key value of 4X4 keypad in ASCII format*/

					count++;

					lcd_data_write('*');
					/* Star(*) is displayed in 2nd row of LCD*/
				}

				_delay_ms(300);
				/*300ms delay as guard time between two consecutive pressing of key*/
			}

      count=0;
			/*Counter reset to 0*/

			password_entered[5]=0;
			/*Null character at the last of array(password_entered) to convert it to a string*/

			lcd_command_write(0x01);
			/*Clear screen*/

			/*Password Comparision*/
			if(!(strcmp(password_set,password_entered)))
			{
				lcd_string_write("Correct Password");
				/*String display in 1st row of LCD*/

				password_status=1;
				/*Changing the Password Status to Correct Password*/
			}
			else
			{
				attempts = attempts + 1;
				/* increment wrong attempts value by one */

				lcd_string_write("Wrong Password");
				/*String display in 1st row of LCD*/

        /* Five pulses on buzzer and yellow LED for wrong attempting */
        for (int i = 0; i < 5; i++)
				{
					one_pulse();
				}

				if (attempts < 3)
				{
					lcd_command_write(0x01);
					/*Clear screen*/

					lcd_string_write("Reenter Password");
					/*String display in 1st row of LCD*/
				}
			}
		}

		lcd_command_write(0x01);
		/*Clear Screen*/

		if (password_status == 0)
		{
			lcd_command_write(0x01);
			/*Clear Screen*/

			lcd_command_write(0x80);
			/*Cursor moves to 1st row 1st column of LCD*/

			lcd_string_write("GET LOST");
			/*String display in 1st row of LCD*/

			_delay_ms(5000);

			count=0;
			password_entered[5]=0;
			password_status = 0;
			attempts = 0;
			/* reset */
		}

		if (password_status == 1)
		{
			lcd_command_write(0x80);
			/*Cursor moves to 1st row 1st column of LCD*/

			lcd_string_write("Unlocked");
			/*String display in 1st row of LCD*/

			one_pulse();
			one_pulse();
			PORTA = 0x0a;

			_delay_ms(5000);

			PORTA = 0x00;

			lcd_command_write(0x01);
			/*Clear Screen*/

			lcd_command_write(0x80);
			/*Cursor moves to 1st row 1st column of LCD*/

			lcd_string_write("Locked");
			/*String display in 1st row of LCD*/

			_delay_ms(1000);

			count=0;
			password_entered[5]=0;
			password_status = 0;
			attempts = 0;
			/* reset */
		}
	}
}

/*4X4 Keypad Function Definitions*/
unsigned char read_keypad(void)
{
	unsigned char keypad_input=0xff,keypad_output=0xff;
	KEYPAD_PORT=0xfe;
	_delay_us(2);
	keypad_input=KEYPAD_PIN & 0xf0;

	if(keypad_input==0xe0)
	keypad_output=0x01;
	else if(keypad_input==0xd0)
	keypad_output=0x02;
	else if(keypad_input==0xb0)
	keypad_output=0x03;
	else if(keypad_input==0x70)
	keypad_output=0x0c;
	else
	;

	KEYPAD_PORT=0xfd;
	_delay_us(2);
	keypad_input=KEYPAD_PIN & 0xf0;

	if(keypad_input==0xe0)
	keypad_output=0x04;
	else if(keypad_input==0xd0)
	keypad_output=0x05;
	else if(keypad_input==0xb0)
	keypad_output=0x06;
	else if(keypad_input==0x70)
	keypad_output=0x0d;
	else
	;

	KEYPAD_PORT=0xfb;
	_delay_us(2);
	keypad_input=KEYPAD_PIN & 0xf0;

	if(keypad_input==0xe0)
	keypad_output=0x07;
	else if(keypad_input==0xd0)
	keypad_output=0x08;
	else if(keypad_input==0xb0)
	keypad_output=0x09;
	else if(keypad_input==0x70)
	keypad_output=0x0e;
	else
	;

	KEYPAD_PORT=0xf7;
	_delay_us(2);
	keypad_input=KEYPAD_PIN & 0xf0;

	if(keypad_input==0xe0)
	keypad_output=0x0a;
	else if(keypad_input==0xd0)
	keypad_output=0x00;
	else if(keypad_input==0xb0)
	keypad_output=0x0b;
	else if(keypad_input==0x70)
	keypad_output=0x0f;
	else
	;

	return keypad_output;
}

void one_pulse(void)
{
	PORTA |= 0x05;
	_delay_ms(100);
	PORTA = 0x00;
	_delay_ms(100);
}

/*Alphanumeric LCD Function Definitions*/
void lcd_data_write(char data)
{
	LCD_CONT_PORT=_BV(LCD_EN)|_BV(LCD_RS);
	LCD_DATA_PORT=data;
	_delay_ms(1);
	LCD_CONT_PORT=_BV(LCD_RS);
	_delay_ms(1);
}

void lcd_command_write(char command)
{
	LCD_CONT_PORT=_BV(LCD_EN);
	LCD_DATA_PORT=command;
	_delay_ms(1);
	LCD_CONT_PORT=0x00;
	_delay_ms(1);
}

void lcd_init()
{
	lcd_command_write(0x38);
	lcd_command_write(0x01);
	lcd_command_write(0x06);
	lcd_command_write(0x0e);
}

void lcd_string_write(char *string)
{
	while (*string)
	lcd_data_write(*string++);
}
