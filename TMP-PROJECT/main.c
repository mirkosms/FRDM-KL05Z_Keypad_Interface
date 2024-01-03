#include "frdm_bsp.h"
#include "klaw4x4.h"
#include "lcd1602.h"

typedef enum { NONE, ADD, SUBTRACT, MULTIPLY, DIVIDE } Operation;

// Definiowanie wartości logicznych
#define true 1
#define false 0

static void floatToStr(float num, char* str);

int main(void) {
    char buffer[16];
    float currentNumber = 0.0f;
    float storedNumber = 0.0f;
    Operation currentOperation = NONE;
    int key_pressed = false;  
    int decimalEntered = false;
    float decimalMultiplier = 0.1f;  // Deklaracja zmiennej

    Klaw_Init();
    LCD1602_Init();
    LCD1602_ClearAll();

    while (1) {
        char key = Klaw_Read();

        if (key != '\0' && !key_pressed) {
            key_pressed = true;

            if (key >= '0' && key <= '9') {
                if (!decimalEntered) {
                    currentNumber = currentNumber * 10.0f + (key - '0');
                } else {
                    currentNumber += (key - '0') * decimalMultiplier;
                    decimalMultiplier *= 0.1f;
                }
                floatToStr(currentNumber, buffer);
                LCD1602_ClearAll();
                LCD1602_Print(buffer);
            } else if (key == '.') {
                decimalEntered = true;
            } else if (key == '+' || key == '-' || key == '*' || key == '/') {
                storedNumber = currentNumber;
                currentNumber = 0.0f;
                decimalEntered = false;
                decimalMultiplier = 0.1f;
                switch (key) {
                    case '+': currentOperation = ADD; break;
                    case '-': currentOperation = SUBTRACT; break;
                    case '*': currentOperation = MULTIPLY; break;
                    case '/': currentOperation = DIVIDE; break;
                }
            } else if (key == '=') {
                switch (currentOperation) {
                    case ADD: currentNumber += storedNumber; break;
                    case SUBTRACT: currentNumber = storedNumber - currentNumber; break;
                    case MULTIPLY: currentNumber *= storedNumber; break;
                    case DIVIDE:
                        if (currentNumber != 0.0f) currentNumber = storedNumber / currentNumber;
                        else LCD1602_Print("Error");
                        break;
                    default: break;
                }
                floatToStr(currentNumber, buffer);
                LCD1602_ClearAll();
                LCD1602_Print(buffer);
                currentOperation = NONE;
                decimalEntered = false;
                decimalMultiplier = 0.1f;
            } else if (key == 'C') {
                currentNumber = 0.0f;
                storedNumber = 0.0f;
                currentOperation = NONE;
                decimalEntered = false;
                decimalMultiplier = 0.1f;
                LCD1602_ClearAll();
            }
        } else if (key == '\0') {
            key_pressed = false;
        }

        for (int i = 0; i < 10000; i++) {} // Debouncing delay
    }
}

static void floatToStr(float num, char* str) {
    int intPart = (int)num;
    int decimalPart = (int)((num - (float)intPart) * 100);

    if (decimalPart < 0) {
        decimalPart = -decimalPart; // Zapewnienie dodatniości
    }

    int i = 0;
    do {
        int digit = intPart % 10;
        str[i++] = '0' + digit;
        intPart /= 10;
    } while (intPart > 0);

    // Odwracanie części całkowitej
    for (int j = 0; j < i / 2; ++j) {
        char temp = str[j];
        str[j] = str[i - 1 - j];
        str[i - 1 - j] = temp;
    }

    // Dodanie części dziesiętnej tylko wtedy, gdy jest to konieczne
    if (decimalPart > 0) {
        str[i++] = '.';
        if (decimalPart % 10 == 0) { // Jeśli tylko jedna znacząca cyfra
            str[i++] = '0' + (decimalPart / 10);
        } else { // Dwie znaczące cyfry
            str[i++] = '0' + (decimalPart / 10);
            str[i++] = '0' + (decimalPart % 10);
        }
    }

    str[i] = '\0';
}


