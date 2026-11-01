//lcd_defines.c
#include<LPC21xx.h>
#include "lcd.h"
#include "delay.h"


#define DATA_MASK 0x000000FF
#define LCD_RS (1<<8)
#define LCD_EN (1<<9)



void LCD_cmd(unsigned char cmd)
{
	 IOCLR0=LCD_RS;

	IOCLR0=DATA_MASK;
	IOSET0 = ((unsigned int)cmd& DATA_MASK); 

	

	IOSET0 = LCD_EN;
	delay_ms(2);
	IOCLR0 = LCD_EN;
	delay_ms(2);
}

void LCD_data(unsigned char data)
{
	IOSET0=LCD_RS;
	
	IOCLR0 = DATA_MASK;
	IOSET0 = ((unsigned int)data & DATA_MASK);
	

	
	   
	IOSET0 = LCD_EN;
	delay_ms(2);
	IOCLR0 = LCD_EN;
	delay_ms(2);	
}

void LCD_string(char *str)
{
	while(*str)
	{
		LCD_data(*str);
		str++;
	}
}

void LCD_Goto(unsigned char row,unsigned char col)
{
	if(row==0)
	LCD_cmd(0x80+col);
	else
	LCD_cmd(0xc0+col);
}

void LCD_Init(void)
{
    /* P0.0 to P0.9 as GPIO */
    PINSEL0 &= ~(0x000FFFFF);

    /* P0.0-P0.9 as output */
    IODIR0 |= 0x000003FF;

    /* Keep LCD initially disabled */
    IOCLR0 = LCD_RS | LCD_EN;
    IOCLR0 = DATA_MASK;

    delay_ms(100);

    LCD_cmd(0x38);
    delay_ms(5);

    LCD_cmd(0x38);
    delay_ms(5);

    LCD_cmd(0x38);
    delay_ms(5);

    LCD_cmd(0x0C);
    delay_ms(5);

    LCD_cmd(0x01);
    delay_ms(10);

    LCD_cmd(0x06);
    delay_ms(5);
}

void LCD_Printnum(unsigned int num)
{
	if(num==0)
	{
		LCD_data('0');
		return ;
	}
	if(num>=100)
	LCD_data((num/100)+'0');
	if(num>=10)
	LCD_data(((num/10)%10)+'0');
	LCD_data((num%10)+'0');
}


void LCD_Clear(void)
{
	LCD_cmd(0x01);
	delay_ms(2);
}


