#ifndef ROMANDIGIT_H
#define ROMANDIGIT_H

#include <stddef.h>

#define MAX_ROMAN_STRING_LENGTH 20  // Maksymalna długość łańcucha znaków dla cyfr rzymskich
#define MAX_DIGIT_STRING_LENGTH 20  // Maksymalna długość łańcucha znaków dla cyfr

extern char romanString[MAX_ROMAN_STRING_LENGTH]; // Bufor przechowujący bieżący ciąg cyfr rzymskich
extern char digitString[MAX_DIGIT_STRING_LENGTH]; // Bufor przechowujący bieżący ciąg cyfr
extern int romanModeInComputerEnabled;            // Flag, która określa, czy tryb Roman jest aktywny w trybie COMPUTER

// Dodaje cyfrę do bufora cyfr
void addDigitToBuffer(char digit);
// Resetuje bufor cyfr
void resetDigitBuffer(void);
// Obsługuje wprowadzenie cyfry
void handleDigitInput(char key);

// Konwertuje naciśnięty klawisz na znak rzymski
char ConvertKeyToRoman(char key);
// Resetuje bufor cyfr rzymskich
void resetRomanBuffer(void);
// Dodaje znak rzymski do bufora
void addRomanCharToBuffer(char romanChar);

#endif // ROMANDIGIT_H
