//lcd.h
#ifndef LCD_H
#define LCD_H

void LCD_Init(void);
void LCD_cmd(unsigned char cmd);
void LCD_data(unsigned char data);
void LCD_string(char *str);
void LCD_Goto(unsigned char row, unsigned char column);
void LCD_Clear(void);
void LCD_Printnum(unsigned int num);
#endif

