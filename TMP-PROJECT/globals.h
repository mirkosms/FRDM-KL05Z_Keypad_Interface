#ifndef GLOBALS_H
#define GLOBALS_H

#include <stdint.h>

extern volatile uint32_t tickCount;
extern volatile uint32_t lastSetButtonTick;
extern volatile uint32_t displayTimer;
extern volatile uint32_t lastModeChangeTick;
extern volatile int displayState;

#endif // GLOBALS_H
