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
    const char* noteName = "";
    uint32_t frequency = 0;

    // Przypisanie dźwięku do klawisza
    switch(key) {
        case '1': noteName = "C4"; frequency = C4_FREQ; break;
        case '2': noteName = "D4"; frequency = D4_FREQ; break;
        case '3': noteName = "E4"; frequency = E4_FREQ; break;
        case '/': noteName = "F4"; frequency = F4_FREQ; break;
        case '4': noteName = "G4"; frequency = G4_FREQ; break;
        case '5': noteName = "A4"; frequency = A4_FREQ; break;
        case '6': noteName = "B4"; frequency = B4_FREQ; break;
        case '*': noteName = "C5"; frequency = C5_FREQ; break;
        case '7': noteName = "D5"; frequency = D5_FREQ; break;
        case '8': noteName = "E5"; frequency = E5_FREQ; break;
        case '9': noteName = "F5"; frequency = F5_FREQ; break;
        case '-': noteName = "G5"; frequency = G5_FREQ; break;
        case '.': noteName = "A5"; frequency = A5_FREQ; break;
        case '0': noteName = "B5"; frequency = B5_FREQ; break;
        case '=': noteName = "C6"; frequency = C6_FREQ; break;
        case '+': noteName = "D6"; frequency = D6_FREQ; break;
        default: Buzzer_StopTone(); return;
    }

    if (key != 0) {
        Buzzer_PlayTone(frequency);
        if (currentMode == MUSIC) {
            LCD1602_ClearAll();
            LCD1602_Print(noteName);
        }
    } else {
        Buzzer_StopTone();
    }
}

