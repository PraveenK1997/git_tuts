/*
 * challenge6b_display_analog_values.c
 *
 * Created: 10/20/2022 23:08:10
 * Author : prave
 */ 

#include <avr/io.h>
#define F_CPU 16000000L
#include <util/delay.h>

#define rs_high() PORTD |=(1<<0);
#define rs_low() PORTD &= ~(1<<0);

#define en_high() PORTD |= (1<<1);
#define en_low() PORTD &= ~(1<<1);

#define lcdport PORTB

void lcd_command(unsigned char cmd);

void lcd_data(unsigned char data);

void lcd_init();

void cursorpos(char x, char y);

void lcd_print(char row, char column, unsigned int value, int digits);

void lcd_out(char x, char y, char *str);

int adc_read(char channel);

int main(void)
{
	
	unsigned int volt;
	unsigned int temp;
	lcd_init();
	while(1)
	{
		
		volt = adc_read(6);
		lcd_out(1,1,"Volt: ");
		volt = (volt * 4.88);
		lcd_print(1,7,volt,4);
		lcd_out(1,11,"mV");
		temp = (volt/10);
		lcd_out(2,1,"Temp: ");
		lcd_print(2,7,temp,4);
		lcd_out(2,11,"C");
		_delay_ms(1000);
		
	}
	
}

void lcd_command(unsigned char cmd)
{
	
	lcdport = cmd;
	rs_low();
	en_high();
	_delay_ms(1);
	en_low();
}

void lcd_data(unsigned char data)
{
	
	lcdport = data;
	rs_high();
	en_high();
	_delay_ms(1);
	en_low();
}

void lcd_init()
{
	
	lcd_command(0x38);_delay_ms(500);
	lcd_command(0x02);_delay_ms(500);
	lcd_command(0x01);_delay_ms(5);
	lcd_command(0x0c);_delay_ms(5);
	lcd_command(0x06);_delay_ms(5);
	lcd_command(0x80);_delay_ms(5);
	
}

void cursorpos(char x, char y)
{
	
	if( (x < 1 || x > 2)  || (y < 1 || y > 16) )
	{
		x = 1;
		y = 1;
	}
	if(x == 1)
	lcd_command(0x80+y);
	else
	lcd_command(0xC0+y);
	
}

void lcd_print(char row, char coloumn, unsigned int value, int digits)
{
	unsigned int temp;
	unsigned int unit;
	unsigned int tens;
	unsigned int hundred;
	unsigned int thousand;
	unsigned int million;
	
	unsigned char flag=0;
	
	if(row==0||coloumn==0)
	{
		cursorpos(1,1);
	}
	else
	{
		cursorpos(row,coloumn);
		
	}
	




	if(digits==5|| flag==1)
	{
		
		million=value/10000+48;
		lcd_data(million);
		flag=1;
	}
	if(digits==4 || flag==1)
	{
		
		temp = value/1000;
		thousand = temp%10 +48;
		lcd_data(thousand);
		flag=1;
	}
	if(digits==3 || flag==1)
	{
		
		temp = value/100;
		hundred = temp%10 +48;
		lcd_data(hundred);
		flag=1;
	}
	if(digits==2 || flag==1)
	{
		
		temp = value/10;
		tens= temp%10 +48;
		lcd_data(tens);
		flag=1;
	}
	if(digits==1 || flag==1)
	{
		
		unit = value%10 + 48;
		lcd_data(unit);
	}
	if(digits>5)
	{
		lcd_data('e');
	}
}
void lcd_out(char x,char y,char *str)
{
	
	cursorpos(x,y);
	while(*str)
	{
		lcd_data(*str);
		str++;
	}
}

int adc_read(char channel)
{
	unsigned int a,result;
	ADMUX = channel; // selecting the adc channel
	ADCSRA = 0x80;  //enabling the ADC
	ADCH = 0x00;  //initialising the ADC registers to hold higher byte
	ADCL = 0x00;  // initialising the ADC registers to hold lower byte
	ADCSRA |=(1 << 6); //enable the ADC start conversion bit
	
	// wait till the conversion get finished
	while( ( ADCSRA & 0X40) != 0);
	a = ADCL; //store lower byte data
	result = ADCH; // store higher byte data
	result = result << 8; //combine both register using bitwise left shift operator
	result = result | a;
	return result;
	
}

