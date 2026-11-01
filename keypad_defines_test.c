#include<LPC21xx.h>
#include "keypad.h"
#include "delay.h"
#include "lcd.h"

#define ROW1 (1<<16)
#define ROW2 (1<<17)
#define ROW3 (1<<18)
#define ROW4 (1<<19)

#define COL1 (1<<20)
#define COL2 (1<<21)
#define COL3 (1<<22)
#define COL4 (1<<23)

#define ROW_MASK (ROW1|ROW2|ROW3|ROW4)
#define COL_MASK (COL1|COL2|COL3|COL4)

	
const char keymap[4][4] = 
{
	{'1','2','3','A'},
	{'4','5','6','B'},
	{'7','8','9','C'},
	{'*','0','#','D'}
};

void Keypad_Init(void)
{
	PINSEL2 &= ~(0xFFFF);

	IODIR1 |= ROW_MASK;
	IODIR1 &= ~COL_MASK;
	IOSET1 = ROW_MASK;
}

char Keypad_GetKey(void)
 
{
	unsigned char row;
	unsigned int col_pin;
	unsigned int row_pin;



		for(row=0;row<4;row++)
		{
		IOSET1=ROW_MASK;
		if(row==0)
			row_pin =ROW1;

		else if(row==1)
		row_pin=ROW2;
		else if(row==2)
		row_pin=ROW3;
		else
		row_pin=ROW4;
		  IOCLR1 = row_pin;
		delay_ms(1);

	
		if((IOPIN1 & COL1)== 0)
		{
			col_pin = 0;
		}
			else if((IOPIN1 & COL2)== 0)
			{
				col_pin=1;
			}
			else if((IOPIN1 & COL3)== 0)
			{
				col_pin=2;
			}
			else if((IOPIN1 & COL4)== 0)
			{
				col_pin=3;
			}
			else
			{
				continue;
			}

			while((IOPIN1&COL_MASK)!=COL_MASK);
			IOSET1=ROW_MASK;

			return keymap[row][col_pin];
			}
			IOSET1=ROW_MASK;

		return '\0';
}


unsigned int Keypad_GetNumber(unsigned char digits)
{
    char key;
    unsigned int number = 0;
    unsigned int digit_count = 0;
    unsigned int i;
    unsigned int divisor = 1;

    LCD_Goto(1,0);
    LCD_string("                ");
    LCD_Goto(1,0);

    while(1)
    {
        key = Keypad_GetKey();

        if(key >= '0' && key <= '9')
        {
            if(digit_count < digits)
            {
                number = (number * 10) + (key - '0');
                digit_count++;
            }

            while(Keypad_GetKey() != '\0');
        }
        else if(key == 'B')
        {
            if(digit_count > 0)
            {
                number = number / 10;
                digit_count--;
            }

            while(Keypad_GetKey() != '\0');
        }
        else if(key == '*')
        {
            number = 0;
            digit_count = 0;

            while(Keypad_GetKey() != '\0');
        }
        else if(key == '#')
        {
            if(digit_count > 0)
            {
                while(Keypad_GetKey() != '\0');
                return number;
            }

            while(Keypad_GetKey() != '\0');
        }

        LCD_Goto(1,0);

        for(i = 0; i < 16; i++)
            LCD_data(' ');

        LCD_Goto(1,0);

        divisor = 1;

        for(i = 1; i < digit_count; i++)
            divisor *= 10;

        if(digit_count > 0)
        {
            for(i = 0; i < digit_count; i++)
            {
                LCD_data(((number / divisor) % 10) + '0');

                if(divisor > 1)
                    divisor /= 10;
            }
        }
    }
}
