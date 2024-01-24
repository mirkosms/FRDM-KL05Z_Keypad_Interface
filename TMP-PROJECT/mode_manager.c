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

Mode currentMode = DEFAULT; // Początkowy tryb

/**
 * Zmienia aktualny tryb działania programu.
 * @param newMode Nowy tryb do ustawienia.
 */
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

/**
 * Obsługuje wprowadzenie klawisza z klawiatury w zależności od aktywnego trybu.
 * @param key Naciśnięty klawisz.
 */
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

/**
 * Obsługuje domyślny tryb działania programu. Odtwarza dźwięk, jeśli buzzer jest włączony.
 * @param key Klawisz naciśnięty przez użytkownika.
 */
void handleDefaultMode(char key) {
    // Jeżeli buzzer jest włączony, odtwarza dźwięk związany z naciśniętym klawiszem.
    if (buzzerEnabled) {
        Buzzer_PlayNoteForKey(key);
    }
}

/**
 * Obsługuje tryb rzymski. Przetwarza naciśnięte klawisze na znaki rzymskie i wyświetla je.
 * @param key Klawisz naciśnięty przez użytkownika.
 */
void handleRomanMode(char key) {
    // Konwersja klawisza na znak rzymski.
    char romanChar = ConvertKeyToRoman(key);

    // Jeśli znak jest ważny (różny od spacji), dodaje go do bufora i wyświetla.
    if (romanChar != ' ') {
        addRomanCharToBuffer(romanChar);
        LCD1602_ClearAll();
        LCD1602_Print(romanString);
        uart_send(romanChar); // Wysyłanie znaku przez UART.
    }

    // Odtwarzanie dźwięku, jeśli buzzer jest włączony.
    if (buzzerEnabled) {
        Buzzer_PlayNoteForKey(key);
    }
}

/**
 * Obsługuje tryb komputerowy. W zależności od ustawienia, przetwarza cyfry lub znaki rzymskie.
 * @param key Klawisz naciśnięty przez użytkownika.
 */
void handleComputerMode(char key) {
    // Jeśli tryb rzymski nie jest aktywny, przetwarza cyfry.
    if (!romanModeInComputerEnabled) {
        handleDigitInput(key);
        uart_send(key); // Wysyłanie cyfry przez UART.
    } else {
        // Jeśli tryb rzymski jest aktywny, przetwarza znaki rzymskie.
        char romanChar = ConvertKeyToRoman(key);
        if (romanChar != ' ') {
            addRomanCharToBuffer(romanChar);
            LCD1602_ClearAll();
            LCD1602_Print(romanString);
            uart_send(romanChar); // Wysyłanie znaku rzymskiego przez UART.
        }
    }

    // Odtwarzanie dźwięku, jeśli buzzer jest włączony.
    if (buzzerEnabled) {
        Buzzer_PlayNoteForKey(key);
    }
}

/**
 * Aktualizuje stan wyświetlacza w zależności od potrzeb programu.
 * Aktualnie nieużywana w kodzie.
 */
void updateDisplay(void) {
    if (displayState == 1 && (tickCount - displayTimer > 1000)) { // 1000 ticków opóźnienia
        displayState = 0; // Reset stanu wyświetlacza
        LCD1602_ClearAll();
        LCD1602_SetCursor(0, 0);
        LCD1602_Print("Tryb MUSIC");
    }
    // Możliwość dopisania koolejnych stanów
}
