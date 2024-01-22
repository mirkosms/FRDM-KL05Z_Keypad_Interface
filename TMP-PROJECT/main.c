#include "MKL05Z4.h"
#include "frdm_bsp.h"
#include "lcd1602.h"
#include "klaw4x4.h"
#include "calculator.h"
#include "TPM.h"
#include "tsi.h"
#include "joystick.h"
#include "buzzer.h"
#include "mode_manager.h"
#include "globals.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

volatile uint32_t tickCount = 0;
volatile uint32_t lastModeChangeTick = 0;
volatile uint32_t lastSetButtonTick = 0;
volatile uint32_t displayTimer = 0;
volatile int displayState = 0;
volatile uint32_t lastActionTime = 0;

#define true 1
#define false 0
#define DEBOUNCE_COUNT 5
#define DISPLAY_UPDATE_DELAY 100 // Opóźnienie w milisekundach


int main(void) {
    // Inicjalizacja urządzeń
    Klaw_Init();
    LCD1602_Init();
    LCD1602_ClearAll();
    TSI_Init();
    Joystick_Init();
    Buzzer_Init();
    SysTick_Config(SystemCoreClock / 100);

    while (1) {
        uint8_t sliderValue = TSI_ReadSlider();
        bool midPressed = Joystick_TestPin(JOYSTICK_MID_PORT, JOYSTICK_MID_PIN);

        if (tickCount - lastActionTime > DISPLAY_UPDATE_DELAY) {
            if (sliderValue > 0) {
                if (sliderValue < 50) {
                    resetCalculator();
                    LCD1602_ClearAll();
                    LCD1602_Print("0");
                } else {
                    deleteLastCharacter();
                }
                lastActionTime = tickCount;
            }
        }

        if (Joystick_TestPin(JOYSTICK_LEFT_PORT, JOYSTICK_LEFT_PIN) && tickCount - lastModeChangeTick > 500) {
            changeMode(MUSIC);
        }

        if (Joystick_TestPin(JOYSTICK_UP_PORT, JOYSTICK_UP_PIN) && tickCount - lastModeChangeTick > 500) {
            changeMode(COMPUTER);
        }

        if (Joystick_TestPin(JOYSTICK_MID_PORT, JOYSTICK_MID_PIN)) {
            changeMode(DEFAULT);
        }

        // Obsługa przycisku SET
        handleSetButton();

        switch (currentMode) {
            case MUSIC:{
                char key = Klaw_Read();
                if (key != 0) {
                    Buzzer_PlayNoteForKey(key);
                }
                break;
            }
            case COMPUTER:
                // Logika dla tego trybu
                break;
            case DEFAULT:
            default:
                // Domyślny tryb pracy
                break;
        }
    }
}

void SysTick_Handler(void) {
    static char last_key = 0;
    static int debounce_counter = 0;

    tickCount++;

    char key = Klaw_Read();
    if (key != last_key) {
        debounce_counter = 0;
        last_key = key;
        if (key != 0) {
            if (currentMode == MUSIC || (currentMode == DEFAULT && buzzerEnabled) || (currentMode == COMPUTER && buzzerEnabled)) {
                Buzzer_PlayNoteForKey(key);
            }
        } else {
            Buzzer_StopTone();
        }
    } else if (key != 0 && debounce_counter < DEBOUNCE_COUNT) {
        debounce_counter++;
        if (debounce_counter == DEBOUNCE_COUNT && currentMode == DEFAULT) {
            processKey(key);
        }
    }

    handleSetButton();
    updateDisplay(); // Aktualizuj stan wyświetlacza
}