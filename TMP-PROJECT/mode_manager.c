#include "mode_manager.h"
#include "lcd1602.h"

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
    } else if (newMode == DEFAULT) {
        LCD1602_Print("Tryb DEFAULT");
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
