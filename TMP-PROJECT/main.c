#include "frdm_bsp.h"
#include "klaw4x4.h"
#include "lcd1602.h"

typedef enum { NONE, ADD, SUBTRACT, MULTIPLY, DIVIDE } Operation;

#define true 1
#define false 0
#define DEBOUNCE_COUNT 5 // Liczba odczytów dla potwierdzenia naciśnięcia

static void doubleToStr(double num, char* str);
static void processKey(char key);

volatile char lastKey = '\0';
volatile float currentNumber = 0.0f;
volatile float storedNumber = 0.0f;
volatile Operation currentOperation = NONE;
volatile int decimalEntered = false;
volatile float decimalMultiplier = 0.1f;

int main(void) {
    Klaw_Init();
    LCD1602_Init();
    LCD1602_ClearAll();

    // Konfiguracja SysTick
    SysTick_Config(SystemCoreClock / 100); // Przykład dla przerwania co 1 ms

    while (1) {
        // Pętla może pozostać pusta lub zawierać logikę niezwiązaną z przerwaniami
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
            processKey(key); // Przetwarzanie naciśniętego klawisza po debouncingu
        }
    }
}


static void processKey(char key) {
    char buffer[16];
    char displayString[2] = {key, '\0'}; // Tworzy ciąg znaków z pojedynczego klawisza

    if (key >= '0' && key <= '9') {
        if (!decimalEntered) {
            currentNumber = currentNumber * 10.0f + (key - '0');
        } else {
            currentNumber += (key - '0') * decimalMultiplier;
            decimalMultiplier *= 0.1f;
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
        currentNumber = 0.0f;
        decimalEntered = false;
        decimalMultiplier = 0.1f;
        currentOperation = (key == '+') ? ADD :
                           (key == '-') ? SUBTRACT :
                           (key == '*') ? MULTIPLY : DIVIDE;

        LCD1602_ClearAll();
        LCD1602_Print(displayString); // Wyświetla znak operacji na wyświetlaczu
    } else if (key == '=') {
        switch (currentOperation) {
            case ADD: currentNumber += storedNumber; break;
            case SUBTRACT: currentNumber = storedNumber - currentNumber; break;
            case MULTIPLY: currentNumber *= storedNumber; break;
            case DIVIDE:
                if (currentNumber != 0.0f) 
                    currentNumber = storedNumber / currentNumber;
                else 
                    LCD1602_Print("Error");
                break;
            default: break;
        }
        doubleToStr(currentNumber, buffer);
        LCD1602_ClearAll();
        LCD1602_Print(buffer);
        currentOperation = NONE;
        decimalEntered = false;
        decimalMultiplier = 0.1f;
    }
    // else if (key == 'C') { ... } - Czeka na implementację na panelu dotykowym
}

static void doubleToStr(double num, char* str) {
    int intPart = (int)num;
    int decimalPart = (int)((num - (double)intPart) * 100);

    // Konwersja części całkowitej na string
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

    // Dodanie części dziesiętnej
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
