#include "mode_manager.h"
#include "lcd1602.h"
#include "buzzer.h"
#include "romanDigit.h"
#include "globals.h"
#include "uart.h"
#include "klaw4x4.h"

// Zmienne globalne (jeśli są potrzebne)
extern volatile uint32_t tickCount;
extern volatile int displayState;
extern volatile uint32_t displayTimer;
extern volatile uint32_t lastModeChangeTick;

Mode currentMode = DEFAULT;

void changeMode(Mode newMode) {
    currentMode = newMode;
    lastModeChangeTick = tickCount;  // Zapisz czas zmiany trybu

    // Wyświetlanie komunikatu o zmianie trybu
    LCD1602_ClearAll();
    if (newMode == MUSIC) {
        LCD1602_Print("Tryb MUSIC");
    } else if (newMode == COMPUTER) {
        LCD1602_Print("Tryb COMPUTER");
    } else if (newMode == ROMAN) {
        LCD1602_ClearAll();
        LCD1602_Print("Tryb ROMAN");
    } else if (newMode == DEFAULT) {
        LCD1602_Print("Tryb DEFAULT");
    }
}

void handleKeyboardInput(char key) {
    if (key != 0) {
        switch (currentMode) {
            case MUSIC:
                handleMusicMode(key);
                break;
            case DEFAULT:
                handleDefaultMode(key);
                break;
            case COMPUTER:
                handleComputerMode(key);
                break;
            case ROMAN:
                handleRomanMode(key);
                break;
        }
    } else {
        Buzzer_StopTone();
    }
}

void handleMusicMode(char key) {
    // Odtwarzanie dźwięku w trybie MUSIC
    Buzzer_PlayNoteForKey(key);
}

void handleDefaultMode(char key) {
    if (buzzerEnabled) {
        Buzzer_PlayNoteForKey(key);
    }
}

void handleRomanMode(char key) {
    char romanChar = ConvertKeyToRoman(key);
    if (romanChar != ' ') {
        addRomanCharToBuffer(romanChar);
        LCD1602_ClearAll();
        LCD1602_Print(romanString);
        uart_send(romanChar); // Wysyłanie przez UART
    }
    if (buzzerEnabled) {
        Buzzer_PlayNoteForKey(key);
    }
}

void handleComputerMode(char key) {
    if (!romanModeInComputerEnabled) {
        handleDigitInput(key);
        uart_send(key); // Wysyłanie przez UART
    } else {
        char romanChar = ConvertKeyToRoman(key);
        if (romanChar != ' ') {
            addRomanCharToBuffer(romanChar);
            LCD1602_ClearAll();
            LCD1602_Print(romanString);
            uart_send(romanChar); // Wysyłanie przez UART
        }
    }
    if (buzzerEnabled) {
        Buzzer_PlayNoteForKey(key);
    }
}

void updateDisplay(void) {
    if (displayState == 1 && (tickCount - displayTimer > 1000)) { // 1000 ticków opóźnienia
        displayState = 0; // Reset stanu wyświetlacza
        LCD1602_ClearAll();
        LCD1602_SetCursor(0, 0);
        LCD1602_Print("Tryb MUSIC");
    }
    // Możliwość dopisania koolejnych stanów
}
