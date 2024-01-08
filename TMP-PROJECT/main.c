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

int main(void) {
    char key;

    Klaw_Init();
    LCD1602_Init();
    LCD1602_Backlight(TRUE);
    TSI_Init();
    initBuzzerTPM();

    while (1) {
        key = Klaw_Read(); // Odczytanie naciśniętego klawisza

        switch(key) {
            case '7': setBuzzerFrequency(C4_FREQ); break;
            case '8': setBuzzerFrequency(D4_FREQ); break;
            case '9': setBuzzerFrequency(E4_FREQ); break;
            case '/': setBuzzerFrequency(F4_FREQ); break; // '/' może być F4
            case '4': setBuzzerFrequency(G4_FREQ); break;
            case '5': setBuzzerFrequency(A4_FREQ); break;
            case '6': setBuzzerFrequency(B4_FREQ); break;
            case '*': setBuzzerFrequency(C5_FREQ); break; // '*' może być C5
            case '1': setBuzzerFrequency(D5_FREQ); break;
            case '2': setBuzzerFrequency(E5_FREQ); break;
            case '3': setBuzzerFrequency(F5_FREQ); break;
            case '-': setBuzzerFrequency(G5_FREQ); break; // '-' może być G5
            case '0': setBuzzerFrequency(A5_FREQ); break;
            case '.': setBuzzerFrequency(B5_FREQ); break; // '.' może być B5
            case '=': setBuzzerFrequency(C6_FREQ); break; // '=' może być C6
            case '+': setBuzzerFrequency(D6_FREQ); break; // '+' może być D6
            default: buzzerOff(); break; // Jeśli żaden klawisz nie jest naciśnięty, wyłącz buzzer
        }
    }

    return 0;
}
