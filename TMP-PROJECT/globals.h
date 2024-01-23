#ifndef GLOBALS_H
#define GLOBALS_H

#include <stdint.h>

// Definicje stałych
#define DEBOUNCE_COUNT 5
#define DISPLAY_UPDATE_DELAY 100 // Opóźnienie w milisekundach

// Deklaracje zmiennych
extern volatile uint32_t tickCount;
extern volatile uint32_t lastModeChangeTick;
extern volatile uint32_t lastSetButtonTick;
extern volatile uint32_t lastRstButtonTick;
extern volatile uint32_t displayTimer;
extern volatile int displayState;
extern volatile uint32_t lastActionTime;

#endif // GLOBALS_H
