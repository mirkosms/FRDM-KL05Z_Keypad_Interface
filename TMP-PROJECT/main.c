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
#include "romanDigit.h"
#include "globals.h"
#include "uart.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

volatile uint32_t tickCount = 0;
volatile uint32_t lastModeChangeTick = 0;
volatile uint32_t lastSetButtonTick = 0;
volatile uint32_t lastRstButtonTick = 0;
volatile uint32_t displayTimer = 0;
volatile int displayState = 0;
volatile uint32_t lastActionTime = 0;

int main(void) {
    Klaw_Init();
    LCD1602_Init();
    LCD1602_ClearAll();
    TSI_Init();
    Joystick_Init();
    Buzzer_Init();
    UART_Init();
    SysTick_Config(SystemCoreClock / 100);

    while (1) {
        uint8_t sliderValue = TSI_ReadSlider();

        if (tickCount - lastActionTime > DISPLAY_UPDATE_DELAY) {
            if (sliderValue > 0) {
                if (currentMode == ROMAN || currentMode == COMPUTER) {
                    resetRomanBuffer();
                    resetDigitBuffer();
                    LCD1602_ClearAll();
                    LCD1602_Print("Reset All");
                } else if (sliderValue < 50) {
                    resetCalculator();
                    LCD1602_ClearAll();
                    LCD1602_Print("Reset");
                } else if (sliderValue >= 50) {
                    deleteLastCharacter();
                }
                lastActionTime = tickCount;
            }
        }

        if (Joystick_TestPin(JOYSTICK_LEFT_PORT, JOYSTICK_LEFT_PIN) && tickCount - lastModeChangeTick > 500) {
            changeMode(MUSIC);
            LCD1602_ClearAll();
            LCD1602_Print("Tryb MUSIC");
        }

        if (Joystick_TestPin(JOYSTICK_UP_PORT, JOYSTICK_UP_PIN) && tickCount - lastModeChangeTick > 500) {
            changeMode(COMPUTER);
        }

        if (Joystick_TestPin(JOYSTICK_RIGHT_PORT, JOYSTICK_RIGHT_PIN) && tickCount - lastModeChangeTick > 500) {
            changeMode(ROMAN);
        }

        if (Joystick_TestPin(JOYSTICK_MID_PORT, JOYSTICK_MID_PIN)) {
            changeMode(DEFAULT);
        }

        handleSetButton();
        handleRstButton();

        switch (currentMode) {
            case MUSIC:
                break;
            case COMPUTER:
                break;
            case ROMAN:
                break;
            case DEFAULT:
            default:
                break;
        }
    }
}

void SysTick_Handler(void) {
    static char last_key = 0;
    static int debounce_counter = 0;

    tickCount++;
    handleRstButton(); 
    handleSetButton();

    char key = Klaw_Read();
    if (key != last_key) {
        last_key = key;

        if (key != 0) {
            handleKeyboardInput(key);  // Wywołanie funkcji obsługującej tryby

            if (currentMode == MUSIC) {
                handleMusicMode(key);  // Bezpośrednie odtwarzanie dźwięku w trybie MUSIC
            } else if (currentMode == DEFAULT) {
                processKey(key);  // Obsługa kalkulatora w trybie DEFAULT
            }
        } else {
            Buzzer_StopTone();
        }
    } else if (key != 0 && debounce_counter < DEBOUNCE_COUNT) {
        debounce_counter++;
    }
}

