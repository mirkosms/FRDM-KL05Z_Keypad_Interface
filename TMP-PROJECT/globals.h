#ifndef GLOBALS_H
#define GLOBALS_H

#include <stdint.h>

// Definicje stałych używanych w całym programie
#define DEBOUNCE_COUNT 5             // Liczba cykli SysTick wymagana do debouncingu przycisków
#define DISPLAY_UPDATE_DELAY 100     // Czas opóźnienia w milisekundach dla aktualizacji wyświetlacza

// Deklaracje zmiennych globalnych
extern volatile uint32_t tickCount;             // Globalny licznik cykli SysTick
extern volatile uint32_t lastModeChangeTick;    // Licznik ostatniej zmiany trybu
extern volatile uint32_t lastSetButtonTick;     // Licznik ostatniego naciśnięcia przycisku SET
extern volatile uint32_t lastRstButtonTick;     // Licznik ostatniego naciśnięcia przycisku RST
extern volatile uint32_t displayTimer;          // Timer dla aktualizacji wyświetlacza
extern volatile int displayState;               // Stan wyświetlacza (np. czy wymaga aktualizacji)
extern volatile uint32_t lastActionTime;        // Licznik ostatniej akcji (np. interakcji z użytkownikiem)

#endif // GLOBALS_H
