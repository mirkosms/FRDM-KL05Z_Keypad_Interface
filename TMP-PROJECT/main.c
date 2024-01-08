#include "MKL05Z4.h"
#include "frdm_bsp.h"
#include "lcd1602.h"
#include "klaw4x4.h"  // Upewnij się, że ta biblioteka posiada odpowiednie funkcje do obsługi klawiatury
#include "TPM.h"
#include "tsi.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Definicje częstotliwości dla nut w skali C-dur
#define C4_FREQ  262
#define D4_FREQ  294
#define E4_FREQ  330
#define F4_FREQ  349
#define G4_FREQ  392
#define A4_FREQ  440
#define B4_FREQ  494
#define C5_FREQ  523  // Następna oktawa C
#define D5_FREQ  587
#define E5_FREQ  659
#define F5_FREQ  698
#define G5_FREQ  784
#define A5_FREQ  880
#define B5_FREQ  988
#define C6_FREQ  1047 // Następna oktawa C
#define D6_FREQ  1175

// Prototypy funkcji
static void setBuzzerFrequency(uint32_t frequency);
static void initBuzzerTPM(void);
static void buzzerOn(void);
static void buzzerOff(void);
static void playToneForKey(char key);
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

int main(void) {
    Klaw_Init();
    LCD1602_Init();
    LCD1602_ClearAll();
    TSI_Init();  // Inicjalizacja TSI
    initBuzzerTPM();  // Inicjalizacja TPM dla buzzera
    SysTick_Config(SystemCoreClock / 100);

    while (1) {
        // Odczyt wartości suwaka TSI
        uint8_t sliderValue = TSI_ReadSlider();

        // Resetowanie kalkulatora, jeśli panel TSI zostanie dotknięty
        if (sliderValue > 0) {
            currentNumber = 0.0;
            storedNumber = 0.0;
            decimalMultiplier = 0.1;
            currentOperation = NONE;
            decimalEntered = false;
            isNegative = false;

            LCD1602_ClearAll();
            LCD1602_Print("0"); // Wyświetlenie 0 na ekranie
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
        buzzerOff();  // Wyłącz buzzer przy zmianie klawisza
    } else if (key != 0 && debounce_counter < DEBOUNCE_COUNT) {
        debounce_counter++;
        if (debounce_counter == DEBOUNCE_COUNT) {
            playToneForKey(key);
            processKey(key);
        }
    } else if (key == 0) {
        buzzerOff();  // Wyłącz buzzer, gdy żaden klawisz nie jest naciśnięty
    }
}


static void processKey(char key) {
    char buffer[16];
    char displayString[2] = {key, '\0'};

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

static void playToneForKey(char key) {
    switch(key) {
        case '7': setBuzzerFrequency(C4_FREQ); break;
        case '8': setBuzzerFrequency(D4_FREQ); break;
        case '9': setBuzzerFrequency(E4_FREQ); break;
        case '/': setBuzzerFrequency(F4_FREQ); break;
        case '4': setBuzzerFrequency(G4_FREQ); break;
        case '5': setBuzzerFrequency(A4_FREQ); break;
        case '6': setBuzzerFrequency(B4_FREQ); break;
        case '*': setBuzzerFrequency(C5_FREQ); break;
        case '1': setBuzzerFrequency(D5_FREQ); break;
        case '2': setBuzzerFrequency(E5_FREQ); break;
        case '3': setBuzzerFrequency(F5_FREQ); break;
        case '-': setBuzzerFrequency(G5_FREQ); break;
        case '0': setBuzzerFrequency(A5_FREQ); break;
        case '.': setBuzzerFrequency(B5_FREQ); break;
        case '=': setBuzzerFrequency(C6_FREQ); break;
        case '+': setBuzzerFrequency(D6_FREQ); break;
        default: buzzerOff(); break;
    }
}


static void setBuzzerFrequency(uint32_t frequency) {
    // Oblicz wartość rejestrów MOD i CNV na podstawie częstotliwości
    TPM0->MOD = (SystemCoreClock / (64 * frequency)) - 1;
    TPM0->CONTROLS[3].CnV = (TPM0->MOD) / 2; // Ustaw wypełnienie 50%
}

static void initBuzzerTPM() {
    // Funkcja inicjalizująca TPM dla buzzera
    PWM_Init();
}

static void buzzerOn() {
    // Włączenie buzzera (sygnał PWM)
    TPM0->CONTROLS[3].CnV = TPM0->MOD / 2;  // 50% wypełnienia impulsu
}

static void buzzerOff() {
    // Wyłączenie buzzera (sygnał PWM)
    TPM0->CONTROLS[3].CnV = 0;  // Wypełnienie impulsu 0%
}