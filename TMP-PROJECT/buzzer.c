#include "buzzer.h"

// Funkcje dotyczące buzzera
void initBuzzerTPM() {
    // Twoja implementacja
    PWM_Init();
}

void setBuzzerFrequency(uint32_t frequency) {
    // Twoja implementacja
    TPM0->MOD = (SystemCoreClock / (64 * frequency)) - 1;
    TPM0->CONTROLS[3].CnV = (TPM0->MOD) / 2; // Ustaw wypełnienie 50%
}

void buzzerOn() {
    // Twoja implementacja
    TPM0->CONTROLS[3].CnV = TPM0->MOD / 2;  // 50% wypełnienia impulsu
}

void buzzerOff() {
    // Twoja implementacja
    TPM0->CONTROLS[3].CnV = 0;  // Wypełnienie impulsu 0%
}

void playToneForKey(char key) {
    switch(key) {
        case '1': setBuzzerFrequency(C4_FREQ); break;
        case '2': setBuzzerFrequency(D4_FREQ); break;
        case '3': setBuzzerFrequency(E4_FREQ); break;
        case '/': setBuzzerFrequency(F4_FREQ); break;
        case '4': setBuzzerFrequency(G4_FREQ); break;
        case '5': setBuzzerFrequency(A4_FREQ); break;
        case '6': setBuzzerFrequency(B4_FREQ); break;
        case '*': setBuzzerFrequency(C5_FREQ); break;
        case '7': setBuzzerFrequency(D5_FREQ); break;
        case '8': setBuzzerFrequency(E5_FREQ); break;
        case '9': setBuzzerFrequency(F5_FREQ); break;
        case '-': setBuzzerFrequency(G5_FREQ); break;
        case '.': setBuzzerFrequency(A5_FREQ); break;
        case '0': setBuzzerFrequency(B5_FREQ); break;
        case '=': setBuzzerFrequency(C6_FREQ); break;
        case '+': setBuzzerFrequency(D6_FREQ); break;
        default: buzzerOff(); break;
    }
}
