#ifndef KEYPAD_H
#define KEYPAD_H

void Keypad_Init(void);
char Keypad_GetKey(void);
unsigned int Keypad_GetNumber(unsigned char digits);

#endif

