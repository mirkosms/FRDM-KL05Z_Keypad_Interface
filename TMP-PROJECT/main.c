#include "frdm_bsp.h"
#include "klaw4x4.h"
#include "lcd1602.h"

typedef enum { NONE, ADD, SUBTRACT, MULTIPLY, DIVIDE } Operation;

#define true 1
#define false 0
#define DEBOUNCE_COUNT 5

static void doubleToStr(double num, char* str);
static void processKey(char key);

volatile char lastKey = '\0';
volatile double currentNumber = 0.0;
volatile double storedNumber = 0.0;
volatile double decimalMultiplier = 0.1;
volatile Operation currentOperation = NONE;
volatile int decimalEntered = false;
volatile int isNegative = false; // Zmienna do śledzenia liczby ujemnej

int main(void) {
    Klaw_Init();
    LCD1602_Init();
    LCD1602_ClearAll();

    SysTick_Config(SystemCoreClock / 100);

    while (1) {
        // Pętla może pozostać pusta
    }
}

void SysTick_Handler(void) {
    static char last_key = 0;
    static int debounce_counter = 0;
    char key = Klaw_Read();

    if (key != last_key) {
        debounce_counter = 0;
        last_key = key;
    } else if (key != 0 && debounce_counter < DEBOUNCE_COUNT) {
        debounce_counter++;
        if (debounce_counter == DEBOUNCE_COUNT) {
            processKey(key);
        }
    }
}

static void processKey(char key) {
    char buffer[16];
    char displayString[2] = {key, '\0'};

    if (key == '-' && currentNumber == 0.0f && !decimalEntered && currentOperation == NONE) {
        isNegative = true;
        LCD1602_ClearAll();
        LCD1602_Print("-"); // Wyświetlenie minusa
    } else if (key >= '0' && key <= '9') {
        double numValue = (key - '0');
        if (isNegative) {
            numValue = -numValue;
            isNegative = false;
        }

        if (!decimalEntered) {
            currentNumber = currentNumber * 10.0 + numValue;
        } else {
            currentNumber += numValue * decimalMultiplier;
            decimalMultiplier *= 0.1;
        }
        doubleToStr(currentNumber, buffer);
        LCD1602_ClearAll();
        LCD1602_Print(buffer);
    } else if (key == '.') {
        decimalEntered = true;
        doubleToStr(currentNumber, buffer);
        strcat(buffer, ".");
        LCD1602_ClearAll();
        LCD1602_Print(buffer);
    } else if (key == '+' || key == '-' || key == '*' || key == '/') {
        storedNumber = currentNumber;
        currentNumber = 0.0;
        decimalEntered = false;
        decimalMultiplier = 0.1;
        currentOperation = (key == '+') ? ADD :
                           (key == '-') ? SUBTRACT :
                           (key == '*') ? MULTIPLY : DIVIDE;

        LCD1602_ClearAll();
        LCD1602_Print(displayString);
    } else if (key == '=') {
        if (currentOperation == DIVIDE && currentNumber == 0.0) {
            LCD1602_ClearAll();
            LCD1602_Print("N/D");  // Wyświetlenie komunikatu o błędzie dzielenia przez zero
        } else {
            switch (currentOperation) {
                case ADD: currentNumber += storedNumber; break;
                case SUBTRACT: currentNumber = storedNumber - currentNumber; break;
                case MULTIPLY: currentNumber *= storedNumber; break;
                case DIVIDE: 
                    currentNumber = storedNumber / currentNumber; 
                    break;
                default: break;
            }
            doubleToStr(currentNumber, buffer);
            LCD1602_ClearAll();
            LCD1602_Print(buffer);
        }
        currentOperation = NONE;
        decimalEntered = false;
        decimalMultiplier = 0.1;
    }
    // else if (key == 'C') { ... } // Czeka na implementację na panelu dotykowym
}

static void doubleToStr(double num, char* str) {
    int isNegative = 0;
    if (num < 0) {
        isNegative = 1;
        num = -num;
    }

    int intPart = (int)num;
    int decimalPart = (int)((num - (double)intPart) * 100);

    int i = 0;
    if (isNegative) {
        str[i++] = '-';
    }

    int tempIntPart = intPart;
    do {
        int digit = tempIntPart % 10;
        str[i++] = '0' + digit;
        tempIntPart /= 10;
    } while (tempIntPart > 0);

    int start = isNegative ? 1 : 0;
    for (int j = start; j < (i + start) / 2; ++j) {
        char temp = str[j];
        str[j] = str[i - 1 - j + start];
        str[i - 1 - j + start] = temp;
    }

    if (decimalPart > 0) {
        str[i++] = '.';
        int firstDecimalDigit = decimalPart / 10;
        int secondDecimalDigit = decimalPart % 10;
        
        if (firstDecimalDigit != 0 || secondDecimalDigit != 0) {
            str[i++] = '0' + firstDecimalDigit;
            if (secondDecimalDigit != 0) {
                str[i++] = '0' + secondDecimalDigit;
            }
        }
    }

    str[i] = '\0';
}
