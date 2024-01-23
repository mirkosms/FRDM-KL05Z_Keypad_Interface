#ifndef ROMANDIGIT_H
#define ROMANDIGIT_H

#include <stddef.h>

#define MAX_ROMAN_STRING_LENGTH 20
#define MAX_DIGIT_STRING_LENGTH 20

extern char romanString[MAX_ROMAN_STRING_LENGTH];
extern char digitString[MAX_DIGIT_STRING_LENGTH];
extern int romanModeInComputerEnabled;

void addDigitToBuffer(char digit);
void resetDigitBuffer(void);
void handleDigitInput(char key);

char ConvertKeyToRoman(char key);
void resetRomanBuffer(void);
void addRomanCharToBuffer(char romanChar);

#endif // ROMANDIGIT_H
