#include "MKL05Z4.h"
#include "frdm_bsp.h"
#include "lcd1602.h"
#include "klaw4x4.h"
#include "TPM.h"
#include "tsi.h"
#include "joystick.h"
#include "buzzer.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


// Prototypy funkcji
static void doubleToStr(double num, char* str);
static void processKey(char key);

typedef enum { NONE, ADD, SUBTRACT, MULTIPLY, DIVIDE } Operation;

#define true 1
#define false 0
#define DEBOUNCE_COUNT 5

static volatile double currentNumber = 0.0;
static volatile double storedNumber = 0.0;
static volatile double decimalMultiplier = 0.1;
static volatile Operation currentOperation = NONE;
static volatile int decimalEntered = false;
static volatile int isNegative = false; // Zmienna do śledzenia liczby ujemnej
static volatile int buzzerEnabled = 1; // Zmienna do kontroli stanu buzzera

int main(void) {
    // Inicjalizacja urządzeń
    Klaw_Init();
    LCD1602_Init();
    LCD1602_ClearAll();
    TSI_Init();
    initBuzzerTPM();
    Joystick_Init();
    SysTick_Config(SystemCoreClock / 100);

    while (1) {
        // Obsługa suwaka TSI i przycisku MID joysticka
        uint8_t sliderValue = TSI_ReadSlider();
        bool midPressed = Joystick_TestPin(JOYSTICK_MID_PORT, JOYSTICK_MID_PIN);

        // Resetowanie kalkulatora, jeśli panel TSI lub przycisk MID zostanie dotknięty
        if (sliderValue > 0 || midPressed) {
            currentNumber = 0.0;
            storedNumber = 0.0;
            decimalMultiplier = 0.1;
            currentOperation = NONE;
            decimalEntered = false;
            isNegative = false;
            LCD1602_ClearAll();
            LCD1602_Print("0");
        }

        // Obsługa przycisku SET joysticka do włączania/wyłączania buzzera
        if (Joystick_TestPin(JOYSTICK_SET_PORT, JOYSTICK_SET_PIN)) {
            buzzerEnabled = !buzzerEnabled; // Przełączanie stanu buzzera
            LCD1602_ClearAll();
            LCD1602_Print(buzzerEnabled ? "Buzzer ON" : "Buzzer OFF");
            DELAY(500) // Krótka pauza na wyświetlenie komunikatu
        }
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
            // Odtwarzanie dźwięku tylko gdy buzzer włączony i klawisz naciśnięty
            if (buzzerEnabled) {
                playToneForKey(key);
            }
            processKey(key);
        }
    } else if (key == 0) {
        buzzerOff(); // Wyłącz buzzer, gdy żaden klawisz nie jest naciśnięty
    }
}


static void processKey(char key) {
    char buffer[16];
    char displayString[2] = {key, '\0'};
    // Odtwarzanie dźwięku tylko gdy buzzer włączony
    if (buzzerEnabled) {
        playToneForKey(key);
    }

    if (key == '-' && currentNumber == 0.0 && !decimalEntered && currentOperation == NONE) {
        isNegative = !isNegative;
        LCD1602_ClearAll();
        LCD1602_Print("-0");
    } else if (key >= '0' && key <= '9') {
        double numValue = (key - '0');
        if (!decimalEntered) {
            currentNumber = currentNumber * 10.0 + (isNegative ? -numValue : numValue);
        } else {
            currentNumber += (isNegative ? -numValue : numValue) * decimalMultiplier;
            decimalMultiplier *= 0.1;
        }
        doubleToStr(currentNumber, buffer);
        LCD1602_ClearAll();
        LCD1602_Print(buffer);
    } else if (key == '.') {
        if (!decimalEntered) {
            decimalEntered = true;
            doubleToStr(currentNumber, buffer);
            strcat(buffer, ".");
            LCD1602_ClearAll();
            LCD1602_Print(buffer);
        }
    } else if (key == '+' || key == '-' || key == '*' || key == '/') {
        storedNumber = currentNumber;
        currentNumber = 0.0; // Resetowanie bieżącej liczby
        decimalEntered = false; // Resetowanie wprowadzania części dziesiętnej
        isNegative = false; // Resetowanie znaku na dodatni dla nowej liczby
        decimalMultiplier = 0.1;
        currentOperation = (key == '+') ? ADD :
                           (key == '-') ? SUBTRACT :
                           (key == '*') ? MULTIPLY : DIVIDE;

        LCD1602_ClearAll();
        LCD1602_Print(displayString);
    } else if (key == '=') {
        if (currentOperation == DIVIDE && currentNumber == 0.0) {
            LCD1602_ClearAll();
            LCD1602_Print("N/D"); // Obsługa dzielenia przez zero
        } else {
            switch (currentOperation) {
                case ADD: currentNumber += storedNumber; break;
                case SUBTRACT: currentNumber = storedNumber - currentNumber; break;
                case MULTIPLY: currentNumber *= storedNumber; break;
                case DIVIDE: currentNumber = storedNumber / currentNumber; break;
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
}

static void doubleToStr(double num, char* str) {
    int isNegative = 0;
    if (num < 0) {
        isNegative = 1;
        num = -num;
    }

    int intPart = (int)num;
    int decimalPart = (int)((num - (double)intPart) * 100 + 0.5); // Dodanie 0.5 dla zaokrąglenia

    // Obsługa przepełnienia po zaokrągleniu
    if (decimalPart >= 100) {
        decimalPart -= 100;
        intPart += 1;
    }

    int i = 0;
    if (isNegative) {
        str[i++] = '-';
    }

    // Konwersja części całkowitej na string
    int tempIntPart = intPart;
    do {
        int digit = tempIntPart % 10;
        str[i++] = '0' + digit;
        tempIntPart /= 10;
    } while (tempIntPart > 0);

    // Odwrócenie ciągu znaków części całkowitej
    int start = isNegative ? 1 : 0;
    for (int j = start; j < (i + start) / 2; ++j) {
        char temp = str[j];
        str[j] = str[i - 1 - j + start];
        str[i - 1 - j + start] = temp;
    }

    // Dodanie części dziesiętnej tylko jeśli różna od zera
    if (decimalPart > 0) {
        str[i++] = '.';
        str[i++] = '0' + (decimalPart / 10);
        str[i++] = '0' + (decimalPart % 10);

        // Usuwanie niepotrzebnych zer na końcu
        if (str[i-1] == '0') {
            i--;
            if (str[i-1] == '0') {
                i -= 2; // Usuń także kropkę, jeśli obie cyfry dziesiętne to zera
            }
        }
    }
    str[i] = '\0';
}