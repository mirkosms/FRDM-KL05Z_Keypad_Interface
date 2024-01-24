#include "romanDigit.h"
#include <string.h>
#include "lcd1602.h"

// Bufory na znaki rzymskie i cyfry
char romanString[MAX_ROMAN_STRING_LENGTH] = {0};
char digitString[MAX_DIGIT_STRING_LENGTH] = {0};
int romanModeInComputerEnabled = 0;

/**
 * Dodaje cyfrę do bufora cyfr.
 * @param digit Cyfra do dodania.
 */
void addDigitToBuffer(char digit) {
    size_t len = strlen(digitString);
    if (len < MAX_DIGIT_STRING_LENGTH - 1) {
        digitString[len] = digit;
        digitString[len + 1] = '\0';
    }
}

/**
 * Resetuje bufor cyfr.
 */
void resetDigitBuffer(void) {
    memset(digitString, 0, MAX_DIGIT_STRING_LENGTH);
}

/**
 * Obsługuje wprowadzenie cyfry.
 * Wyświetla wprowadzoną cyfrę na LCD.
 * @param key Naciśnięty klawisz reprezentujący cyfrę.
 */
void handleDigitInput(char key) {
    if (key >= '0' && key <= '9') {
        addDigitToBuffer(key);
        LCD1602_ClearAll();
        LCD1602_Print(digitString);
    }
}

/**
 * Konwertuje naciśnięty klawisz na odpowiadający mu znak rzymski.
 * @param key Naciśnięty klawisz.
 * @return Znak rzymski odpowiadający naciśniętemu klawiszowi.
 */
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

/**
 * Resetuje bufor znaków rzymskich.
 */
void resetRomanBuffer(void) {
    memset(romanString, 0, MAX_ROMAN_STRING_LENGTH);
}

/**
 * Dodaje znak rzymski do bufora.
 * @param romanChar Znak rzymski do dodania.
 */
void addRomanCharToBuffer(char romanChar) {
    size_t len = strlen(romanString);
    if (len < MAX_ROMAN_STRING_LENGTH - 1) {
        romanString[len] = romanChar;
        romanString[len + 1] = '\0';
    }
}
