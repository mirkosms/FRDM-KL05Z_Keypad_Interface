#include "buzzer.h"

// Globalna zmienna określająca, czy buzzer jest aktywny
extern Mode currentMode;
volatile int buzzerEnabled = 1; // Domyślnie buzzer jest włączony

/**
 * Inicjalizuje buzzer.
 */
void Buzzer_Init(void) {
    PWM_Init(); // Inicjalizacja modułu PWM
    Buzzer_StopTone(); // Zatrzymanie dźwięku na początku
}

/**
 * Odtwarza ton o zadanej częstotliwości.
 * @param frequency Częstotliwość tonu do odtworzenia.
 */
void Buzzer_PlayTone(uint32_t frequency) {
    // Ustawienie modułu TPM dla generowania sygnału PWM
    TPM0->MOD = (SystemCoreClock / (64 * frequency)) - 1;
    TPM0->CONTROLS[3].CnV = (TPM0->MOD) / 2; // Ustawienie wypełnienia 50%
}

/**
 * Zatrzymuje odtwarzanie tonu.
 */
void Buzzer_StopTone(void) {
    TPM0->CONTROLS[3].CnV = 0; // Ustawienie wypełnienia impulsu na 0%
}

/**
 * Odtwarza dźwięk odpowiadający naciśniętemu klawiszowi.
 * @param key Klawisz naciśnięty przez użytkownika.
 */
void Buzzer_PlayNoteForKey(char key) {
    uint32_t frequency = 0;
    // Przypisanie odpowiedniej częstotliwości do każdego klawisza
    switch(key) {
        case '1': frequency = C4_FREQ; break;
        case '2': frequency = D4_FREQ; break;
        case '3': frequency = E4_FREQ; break;
        case '/': frequency = F4_FREQ; break;
        case '4': frequency = G4_FREQ; break;
        case '5': frequency = A4_FREQ; break;
        case '6': frequency = B4_FREQ; break;
        case '*': frequency = C5_FREQ; break;
        case '7': frequency = D5_FREQ; break;
        case '8': frequency = E5_FREQ; break;
        case '9': frequency = F5_FREQ; break;
        case '-': frequency = G5_FREQ; break;
        case '.': frequency = A5_FREQ; break;
        case '0': frequency = B5_FREQ; break;
        case '=': frequency = C6_FREQ; break;
        case '+': frequency = D6_FREQ; break;
        default: frequency = 0; break;  // Brak dźwięku dla innych klawiszy
    }

    // Odtwarzanie dźwięku o określonej częstotliwości
    if (frequency > 0) {
        Buzzer_PlayTone(frequency);
    } else {
        Buzzer_StopTone(); // Zatrzymanie dźwięku, jeśli nie ma przypisanego tonu
    }
}

