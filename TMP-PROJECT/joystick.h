#ifndef JOYSTICK_H
#define JOYSTICK_H

#include "MKL05Z4.h"
#include "mode_manager.h"
#include "globals.h"
#include "buzzer.h"
#include "lcd1602.h"
#include <stdbool.h> 

// Definicje pinów
#define JOYSTICK_UP_PORT    PTB
#define JOYSTICK_DOWN_PORT  PTA
#define JOYSTICK_LEFT_PORT  PTB
#define JOYSTICK_RIGHT_PORT PTA
#define JOYSTICK_MID_PORT   PTB
#define JOYSTICK_RST_PORT   PTA
#define JOYSTICK_SET_PORT   PTA

#define JOYSTICK_UP_PIN     9
#define JOYSTICK_DOWN_PIN   8
#define JOYSTICK_LEFT_PIN   5
#define JOYSTICK_RIGHT_PIN  9
#define JOYSTICK_MID_PIN    13
#define JOYSTICK_RST_PIN    10
#define JOYSTICK_SET_PIN    12

// Deklaracje funkcji
void Joystick_Init(void);
bool Joystick_TestPin(GPIO_Type* port, uint32_t pin);
void handleSetButton(void);

#endif // JOYSTICK_H
