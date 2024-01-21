#include "buzzer.h"

extern Mode currentMode;

// Funkcje dotyczące buzzera
void Buzzer_Init(void) {
    PWM_Init();
    Buzzer_StopTone(); // Zatrzymaj dźwięk na początku
}

void Buzzer_PlayTone(uint32_t frequency) {
    // Ustawienie częstotliwości dla buzzera
    TPM0->MOD = (SystemCoreClock / (64 * frequency)) - 1;
    TPM0->CONTROLS[3].CnV = (TPM0->MOD) / 2; // Ustaw wypełnienie 50%
}

void Buzzer_StopTone(void) {
    // Zatrzymanie dźwięku buzzera
    TPM0->CONTROLS[3].CnV = 0;  // Wypełnienie impulsu 0%
}

void Buzzer_PlayNoteForKey(char key) {
    uint32_t frequency = 0;

    // Przypisanie dźwięku do klawisza
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
        default: frequency = 0; break;
    }
    
    if (frequency > 0) {
        Buzzer_PlayTone(frequency);
    } else {
        Buzzer_StopTone();
    }
}

