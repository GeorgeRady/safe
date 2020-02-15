#include<avr/io.h>
/*Includes io.h header file where all the Input/Output Registers and its Bits are defined for all AVR microcontrollers*/

#define	F_CPU	1000000
/*Defines a macro for the delay.h header file. F_CPU is the microcontroller frequency value for the delay.h header file. Default value of F_CPU in delay.h header file is 1000000(1MHz)*/

#include<util/delay.h>
/*Includes delay.h header file which defines two functions, _delay_ms (millisecond delay) and _delay_us (microsecond delay)*/

#define		KEYPAD_PORT		PORTC
/*KEYPAD_PORT is the microcontroller PORT Register to which 4X4 keypad is connected. Here it is connected to PORTC*/

#define		KEYPAD_PIN		PINC
/*KEYPAD_PIN is the microcontroller PIN Register to which 4X4 keypad is connected. Here it is connected to PINC*/

#define		LCD_DATA_PORT		PORTB
/*LCD_DATA_PORT is the microcontroller PORT Register to which the data pins of the LCD are connected. Here it is connected to PORTB*/

#define 	LCD_CONT_PORT		PORTD
/*LCD_CONT_PORT is the microcontroller PORT Register to which the control pins of the LCD are connected. Here it is connected to PORTD*/

#define 	LCD_RS 		PD0
/*LCD_RS is the microcontroller Port pin to which the RS pin of the LCD is connected. Here it is connected to PD0*/

#define 	LCD_RW 		PD1
/*LCD_RW is the microcontroller Port pin to which the RW pin of the LCD is connected. Here it is connected to PD1*/

#define 	LCD_EN 		PD2
/*LCD_EN is the microcontroller Port pin to which the EN pin of the LCD is connected. Here it is connected to PD2*/

#include<string.h>
/*Includes string.h header file which defines different string functions*/

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

	unsigned char count=0, keypad_value, password_status=0;
	char password_set[6], password_entered[6];

	password_set[0] = 49;
	password_set[1] = 49;
	password_set[2] = 49;
	password_set[3] = 49;
	password_set[4] = 63;
	password_set[5] = 0;
	/* ASCII Values */
	/* Password = 1111D */

	/*Variable declarations*/

	lcd_init();
	/*LCD initialization*/

	lcd_string_write("Gwargy on fire");
	/*String display in 1st row of LCD*/

	lcd_command_write(0xc0);
	/*Cursor moves to 2nd row 1st column of LCD*/

	lcd_string_write(";)");
	/*String display in 2nd row of LCD*/

	_delay_ms(500);
	_delay_ms(500);
	_delay_ms(500);
	_delay_ms(500);
	/*Display stays for 2 second*/

	lcd_command_write(0x01);
	/*Clear screen*/

	lcd_string_write("Enter Password");
	/*String display in 1st row of LCD*/

	/*While loop for password entry and checking*/
	while(password_status==0x00)
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
				/*Counter increment*/

				lcd_data_write('*');
				/* Star(*) is displayed in 2nd row of LCD*/
			}
			else
			{
				;
				/*Null statement*/
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
			lcd_string_write("Wrong Password");
			/*String display in 1st row of LCD*/

			_delay_ms(500);
			_delay_ms(500);
			_delay_ms(500);
			_delay_ms(500);
			/*Display stays for 2 second*/

			lcd_command_write(0x01);
			/*Clear screen*/

			lcd_string_write("Reenter Password");
			/*String display in 1st row of LCD*/
		}
	}

	_delay_ms(500);
	_delay_ms(500);
	_delay_ms(500);
	_delay_ms(500);
	/*Display stays for 2 second*/

	lcd_command_write(0x01);
	/*Clear Screen*/

	if (password_status == 1)
	{
		lcd_command_write(0x80);
		/*Cursor moves to 1st row 1st column of LCD*/

		lcd_string_write("Openning...");
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

		lcd_string_write("Closed");
		/*String display in 1st row of LCD*/

	}

	/*Start of infinite loop*/
	// while(1)
	// {
	// 	lcd_command_write(0x80);
	// 	/*Cursor moves to 1st row 1st column of LCD*/
	//
	// 	lcd_string_write("Press any Key");
	// 	/*String display in 1st row of LCD*/
	//
	// 	lcd_command_write(0xc0);
	// 	/*Cursor moves to 2nd row 1st column of LCD*/
	//
	// 	lcd_string_write("Key Value: ");
	// 	/*String display in 1st row of LCD*/
	//
	// 	keypad_value=read_keypad();
	// 	/*Scan's 4X4 keypad and returns pressed key value or default value*/
	//
	// 	/*Checking if any key is pressed or not*/
	// 	if(keypad_value!=0xff)
	// 	{
	// 		switch(keypad_value)
	// 		{
	// 			case 0:
	// 			lcd_data_write('0');
	// 			/*Displays 0 in 2nd row of LCD*/
	//
	// 			break;
	// 			/*Break statement*/
	//
	// 			case 1:
	// 			lcd_data_write('1');
	// 			/*Displays 1 in 2nd row of LCD*/
	//
	// 			break;
	// 			/*Break statement*/
	//
	// 			case 2:
	// 			lcd_data_write('2');
	// 			/*Displays 2 in 2nd row of LCD*/
	//
	// 			break;
	// 			/*Break statement*/
	//
	// 			case 3:
	// 			lcd_data_write('3');
	// 			/*Displays 3 in 2nd row of LCD*/
	//
	// 			break;
	// 			/*Break statement*/
	//
	// 			case 4:
	// 			lcd_data_write('4');
	// 			/*Displays 4 in 2nd row of LCD*/
	//
	// 			break;
	// 			/*Break statement*/
	//
	// 			case 5:
	// 			lcd_data_write('5');
	// 			/*Displays 5 in 2nd row of LCD*/
	//
	// 			break;
	// 			/*Break statement*/
	//
	// 			case 6:
	// 			lcd_data_write('6');
	// 			/*Displays 6 in 2nd row of LCD*/
	//
	// 			break;
	// 			/*Break statement*/
	//
	// 			case 7:
	// 			lcd_data_write('7');
	// 			/*Displays 7 in 2nd row of LCD*/
	//
	// 			break;
	// 			/*Break statement*/
	//
	// 			case 8:
	// 			lcd_data_write('8');
	// 			/*Displays 8 in 2nd row of LCD*/
	//
	// 			break;
	// 			/*Break statement*/
	//
	// 			case 9:
	// 			lcd_data_write('9');
	// 			/*Displays 9 in 2nd row of LCD*/
	//
	// 			break;
	// 			/*Break statement*/
	//
	// 			case 10:
	// 			lcd_data_write('*');
	// 			/*Displays * in 2nd row of LCD*/
	//
	// 			break;
	// 			/*Break statement*/
	//
	// 			case 11:
	// 			lcd_data_write('#');
	// 			/*Displays # in 2nd row of LCD*/
	//
	// 			break;
	// 			/*Break statement*/
	//
	// 			case 12:
	// 			lcd_data_write('A');
	// 			/*Displays A in 2nd row of LCD*/
	//
	// 			break;
	// 			/*Break statement*/
	//
	// 			case 13:
	// 			lcd_data_write('B');
	// 			/*Displays B in 2nd row of LCD*/
	//
	// 			break;
	// 			/*Break statement*/
	//
	// 			case 14:
	// 			lcd_data_write('C');
	// 			/*Displays C in 2nd row of LCD*/
	//
	// 			break;
	// 			/*Break statement*/;
	//
	// 			case 15:
	// 			lcd_data_write('D');
	// 			/*Displays D in 2nd row of LCD*/
	//
	// 			break;
	// 			/*Break statement*/;
	// 		}
	// 	}
	// 	else
	// 	{
	// 		;
	// 		/*Null statement*/
	// 	}
	//
	// 	_delay_ms(300);
	// 	/*300ms delay as guard time between two consecutive pressing of key*/
	// }
}
/*End of program*/

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

