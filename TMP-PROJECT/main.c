#include "frdm_bsp.h"
#include "klaw4x4.h"
#include "lcd1602.h"

typedef enum { NONE, ADD, SUBTRACT, MULTIPLY, DIVIDE } Operation;

// Definiowanie true i false dla kompatybilności
#define true 1
#define false 0

// Prototyp funkcji konwersji liczby na ciąg znaków
static void intToStr(int num, char* str);

int main(void) {
    char buffer[16];
    int currentNumber = 0;
    int storedNumber = 0;
    Operation currentOperation = NONE;
    int key_pressed = false; // Używamy int zamiast bool

    Klaw_Init();
    LCD1602_Init();
    LCD1602_ClearAll();

    while (1) {
        char key = Klaw_Read();

        if (key != '\0' && !key_pressed) {
            key_pressed = true;

            if (key >= '0' && key <= '9') {
                currentNumber = currentNumber * 10 + (key - '0');
                intToStr(currentNumber, buffer);
                LCD1602_ClearAll();
                LCD1602_Print(buffer);
            } else if (key == '+' || key == '-' || key == '*' || key == '/') {
                storedNumber = currentNumber;
                currentNumber = 0;
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
                        if (currentNumber != 0) currentNumber = storedNumber / currentNumber;
                        else LCD1602_Print("Error"); // Error handling for division by zero
                        break;
                    case NONE: break; // Obsługa przypadku NONE
                }
                intToStr(currentNumber, buffer);
                LCD1602_ClearAll();
                LCD1602_Print(buffer);
                currentOperation = NONE;
            }
            // Implementacja panelu dotykowego 'C' będzie tutaj
        } else if (key == '\0') {
            key_pressed = false;
        }

        // Krótkie opóźnienie dla debouncing
        for (int i = 0; i < 10000; i++) {
            // Proste opóźnienie; dostosuj wartość i = 10000, jeśli jest to konieczne
        }
    }
}
// Funkcja intToStr
static void intToStr(int num, char* str) {
    int i = 0;
    int isNegative = 0; // Zamiast typu bool używamy int

    if (num == 0) {
        str[i++] = '0';
        str[i] = '\0';
        return;
    }

    if (num < 0) {
        isNegative = 1; // Zamiast 'true' używamy 1
        num = -num;
    }

    while (num != 0) {
        int rem = num % 10;
        str[i++] = (char)(rem + '0'); // Dodajemy jawne rzutowanie
        num = num / 10;
    }

    if (isNegative) {
        str[i++] = '-';
    }

    str[i] = '\0';

    // Odwrócenie ciągu znaków
    int start = 0;
    int end = i - 1;
    while (start < end) {
        char temp = str[start];
        str[start] = str[end];
        str[end] = temp;
        start++;
        end--;
    }
}
