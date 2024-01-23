#include "romanDigit.h"
#include <string.h>
#include "lcd1602.h"

char romanString[MAX_ROMAN_STRING_LENGTH] = {0}; // Bufor na znaki rzymskie
char digitString[MAX_DIGIT_STRING_LENGTH] = {0}; // Bufor na cyfry
int romanModeInComputerEnabled = 0;

void addDigitToBuffer(char digit) {
    size_t len = strlen(digitString);
    if (len < MAX_DIGIT_STRING_LENGTH - 1) {
        digitString[len] = digit;
        digitString[len + 1] = '\0';
    }
}

void resetDigitBuffer(void) {
    memset(digitString, 0, MAX_DIGIT_STRING_LENGTH);
}

void handleDigitInput(char key) {
    if (key >= '0' && key <= '9') {
        addDigitToBuffer(key);
        LCD1602_ClearAll();
        LCD1602_Print(digitString);
    }
}

char ConvertKeyToRoman(char key) {
    switch(key) {
        case '1': return 'I';
        case '2': return 'V';
        case '3': return 'X';
        case '4': return 'L';
        case '5': return 'C';
        case '6': return 'D';
        case '7': return 'M';
        default: return ' '; // Nieaktywny klawisz
    }
}

void resetRomanBuffer(void) {
    memset(romanString, 0, MAX_ROMAN_STRING_LENGTH);
}

void addRomanCharToBuffer(char romanChar) {
    size_t len = strlen(romanString);
    if (len < MAX_ROMAN_STRING_LENGTH - 1) {
        romanString[len] = romanChar;
        romanString[len + 1] = '\0';
    }
}
