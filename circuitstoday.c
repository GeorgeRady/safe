////////////////////***Password Based Digital Locker***////////////////////////
void get_key(void);
void run_key_function(void);
void display(void);
void show_digit(char digit);
void verify_password(void);

void uart_send_string(char strng[50]);
void uart_send_char(char a);
void send_sms(void);
void gsm_initialization(void);

void eeprom_wrt(int addr,char dta);
char eeprom_rd(int addr);

char temp,column,key=0,number[4]={10,10,10,10},index,password[4],temp1,digit;
char show=1,open=0,match=0,temp2,miss_match,temp4,block=0;
char contact_number[10]={'1','2','3','4','5','6','7','8','9','0'};
unsigned int wait;

void block_time() iv IVT_ADDR_TIMER1_COMPA ics ICS_AUTO 
{
    wait++;          //Increment seconds count
    if(wait==10)     //Blocking time = 10 seconds. It can be upto 65,535 seconds
    {                //Since wait is declared as unsigned integer.
        TCCR1B=0X00; //Stop the timer
        wait=0;     //If the blocking time is completed
        block=0;    //System is unblocked or resumed
        miss_match=0;  //Reset the miss-match counting variable
        eeprom_wrt(10,block);  //Update the EEPROM registers
        eeprom_wrt(11,0);
        eeprom_wrt(12,0);
        PORTC&=0X1F;
        PORTC|=0X80;      //Ready
    }
    else    //If blocking time is not completed
    {
        eeprom_wrt(11,wait/256);    //Update the time registers in EEPROM
        eeprom_wrt(12,wait%256);
    }
}

void main()
{
    PORTA=0X00;           DDRA=0XFF;
    PORTB=0X00;           DDRB=0X0F;
    PORTC=0X00;           DDRC=0XFF;
    PORTD=0X0F;           DDRD=0XF0;
    
    OCR1AH=0X3D;           OCR1AL=0X09;    //ONE Second equivalent
    
    TIMSK|=0X10;           //Enable compare match interrupt
    SREG|=0X80;            //Enable global interrupt
    
    UBRRH=0X00;         UBRRL=0X67;    //Baud rate=9600
    UCSRB=0X98;         UCSRC=0X86;
    
    PORTC|=0X40;       //Wait
    //Delay_ms(15000);      //Waiting for GSM module start-up

    for(temp1=0;temp1<=3;temp1++)
    {
        password[temp1]=eeprom_rd(temp1);
    }
    block=eeprom_rd(10);    //Blocked information
    if(block==1)
    {
        PORTC&=0XBF;
        PORTC|=0X20;   //Blocked
        wait=eeprom_rd(11);     //Time lapse after blocking
        wait*=256;
        temp1=eeprom_rd(12);
        wait+=temp1;
        TCCR1B=0X0D;           //Start timer
    }
    else
    {
        PORTC&=0X1F;
        PORTC|=0X80;      //Ready
    }
    while(1)
    {
        if(block==0)
        {
            get_key();
            display();
        }
    }
}

void get_key(void)
{
    for(column=1,temp=1;column<=4;temp*=2,column++)
    {
        PORTB&=0XF0;    //Disable all columns without disturbing remaining pins
        PORTB|=temp;    //Enable particular column leaving remaining port pins
        switch(PINB&0XF0) //Read rows data and identify the
        {
            case 0X10:{                 //Row 1
                        key=column;
                      }break;
            case 0X20:{                 //Row 2
                        key=4+column;
                      }break;
            case 0X40:{                 //Row 3
                        key=8+column;
                      }break;
            case 0X80:{                 //Row 4
                        key=12+column;
                      }break;
        }
        if(key>=1)
        {
            run_key_function();
            do{  display();     }while((PINB&0XF0)!=0);  //Wait for key release
            key=0;          //Setting key value out of range
        }
    }
}

void run_key_function(void)
{
    switch(key)
    {
        case 1:{                  //KEY 1
                    number[index]=1;
                    index++;
                }break;
        case 2:{                  //KEY 2
                    number[index]=2;
                    index++;
                }break;
        case 3:{                  //KEY 3
                    number[index]=3;
                    index++;
                }break;
        case 4:{                  //KEY 4
                    index--;
                    number[index]=10;
                    match=0;
                }break;
        case 5:{                  //KEY 5
                    number[index]=4;
                    index++;
                }break;
        case 6:{                  //KEY 6
                    number[index]=5;
                    index++;
                }break;
        case 7:{                  //KEY 7
                    number[index]=6;
                    index++;
                }break;
        case 8:{                  //KEY 8
                    if(match==1)
                    {
                        for(temp2=0;temp2<=3;temp2++)
                        {
                            number[temp2]=10;
                        }
                    }
               }break;
        case 9:{                  //KEY 9
                    number[index]=7;
                    index++;
                }break;
        case 10:{                 //KEY 10
                    number[index]=8;
                    index++;
                }break;
        case 11:{                 //KEY 11
                    number[index]=9;
                    index++;
                }break;
        case 12:{                 //KEY 12
                    if(match==1)
                    {
                        for(temp2=0;temp2<=3;temp2++)
                        {
                            password[temp2]=number[temp2];
                        }
                        for(temp2=0;temp2<=3;temp2++)
                        {
                            eeprom_wrt(temp2,password[temp2]);
                        }
                    }
                }break;
        case 13:{                 //KEY 13
                    index=0;
                    for(temp2=0;temp2<=3;temp2++)
                    {
                        number[temp2]=10;
                    }
                    PORTC&=0XFD;
                }break;
        case 14:{                 //KEY 14
                    number[index]=0;
                    index++;
                }break;
        case 15:{                 //KEY 15
                    show++;
                    show%=2;
                }break;
        case 16:{                 //KEY 16
                    verify_password();   //Goto verification function
                    if(match==1)    //If password matches
                    {
                        if(open==0) //If the door is closed
                        {
                            open=1;  //Update door status
                            PORTC|=0X01; //Activate relay
                        }
                        else       //If the door is opened
                        {
                            open=0;   //Update door status
                            PORTC&=0XFE; //Turn Off the relay
                            for(temp2=0;temp2<=3;temp2++)
                            { //Update the number array with default values
                                number[temp2]=10;
                            }
                            match=0;   //Update the match variable
                        }    //Else the door will open if open key is pressed
                        miss_match=0;//If there are any missmatches before opening
                    }
                    else
                    {
                        miss_match++;
                        for(temp2=0;temp2<=3;temp2++)
                        {
                            number[temp2]=10;
                        }
                        display();
                        if(miss_match==3)
                        {
                            block=1;
                            PORTC|=0X02;     //Buzzer ON
                            PORTC&=0X1F;
                            PORTC|=0X20;     //Blocked Indicator
                            send_sms();      //Send an SMS
                            eeprom_wrt(10,block); //Update the EEPROM register
                            Delay_ms(3000);       //about blocked condition
                            PORTC&=0XFD;  //Turn Off the buzzer after 3 seconds
                            TCCR1B=0X0D;  //Start timer
                        }
                        else    //If wrong attempts is less than 3 times
                        {
                            PORTC|=0X02;    //Turn On the buzzer
                            PORTC&=0X1F;
                            PORTC|=0X40;    //Wait Indicator
                            Delay_ms(2000);
                            PORTC&=0XFD;    //Turn Off the buzzer
                            PORTC&=0X1F;
                            PORTC|=0X80;    //Ready Indicator
                        }
                    }
                    index=0;
                }break;
     }
}

void verify_password(void)
{
    match=1;       //Assume password matches
    for(temp2=0;temp2<=3;temp2++)
    {//Compare the data of each address location of entered number and password
        if(number[temp2]!=password[temp2])
        {    //If there is a miss-match at any address location
            match=0;      //Password doesnot match.
            temp2=10;     //Exit from for loop
        }
    }
}

void display(void)
{
    for(digit=4,temp1=1;digit>=1;digit--)
    {
        if(number[digit-1]!=10)
        {
            PORTD&=0X0F;
            PORTD|=~temp1*16;
            if(show==1)
            {
                show_digit(number[digit-1]);
            }
            else
            {
                PORTA=0X40;
            }
            Delay_us(5000);
            PORTA=0X00;
        }
        temp1*=2;
    }
}
void show_digit(char digit)
{
    switch(digit)
    {
        case 1:{     PORTA=PORTA|0X06;          }break;
        case 2:{     PORTA=PORTA|0X5B;          }break;
        case 3:{     PORTA=PORTA|0X4F;          }break;
        case 4:{     PORTA=PORTA|0X66;          }break;
        case 5:{     PORTA=PORTA|0X6D;          }break;
        case 6:{     PORTA=PORTA|0X7D;          }break;
        case 7:{     PORTA=PORTA|0X07;          }break;
        case 8:{     PORTA=PORTA|0X7F;          }break;
        case 9:{     PORTA=PORTA|0X6F;          }break;
        case 0:{     PORTA=PORTA|0X3F;          }break;
    }
}

void eeprom_wrt(int addr,char dta)
{
    do{}while(EEWE_BIT==1);
    EEARH=addr/256;
    EEARL=addr%256;
    EEDR=dta;
    EEMWE_BIT=1;
    EEWE_BIT=1;
}

char eeprom_rd(int addr)
{
    int dt;
    do{}while(EEWE_BIT==1);
    EEARH=addr/256;
    EEARL=addr%256;
    EERE_BIT=1;
    dt=EEDR;
    return dt;
}

void gsm_initialization(void)
{
    uart_send_string("ATE0\r\n"); //Disable echoing of commands
    Delay_ms(500);
    uart_send_string("AT+CMGF=1\r\n"); //Message format = text mode
    Delay_ms(500);
    uart_send_string("AT+CMGD=1,4\r\n"); //Delete all the messages
    Delay_ms(500);
    uart_send_string("AT+GSMBUSY=1\r\n");
    Delay_ms(500);       //Busy mode enabled to reject incoming calls
}

void send_sms(void)
{
    gsm_initialization();
    uart_send_string("AT+CMGS=");  //Command to send SMS
    uart_send_char('"');           //Command format
    for(temp4=0;temp4<=9;temp4++)
    {                              //Send contact number
            uart_send_char(contact_number[temp4]);
    }
    uart_send_char('"');           //Command format
    uart_send_char(13);           //Command format

    Delay_ms(300);                //Wait before sending the text
    uart_send_string("Alert:");   //Sent the text
    uart_send_char(13);           //New line indication=carriage return
    uart_send_string("Wrong password is entered for 3 times.");//Tetx
    uart_send_char(13);            //New line indication=carriage return
    uart_send_string("System is blocked for one hour.");
    uart_send_char(26);    //Command format indicating end of text and sens SMS
    uart_send_char(13);    //Command ending format
    uart_send_char(10);
}

void uart_send_string(char strng[50])
{
    for(temp4=0;strng[temp4]!=0;temp4++)
    {
        uart_send_char(strng[temp4]);
    }
}

void uart_send_char(char a)
{
    do{}while((UCSRA&0X20)==0);
    UCSRA|=0X40;
    UDR=a;
    do{}while((UCSRA&0X40)==0);
}
