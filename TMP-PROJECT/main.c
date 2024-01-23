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
volatile uint32_t lastRstButtonTick = 0; 
volatile uint32_t displayTimer = 0;
volatile int displayState = 0;
volatile uint32_t lastActionTime = 0;
volatile int romanModeInComputerEnabled = 0;

#define true 1
#define false 0
#define DEBOUNCE_COUNT 5
#define DISPLAY_UPDATE_DELAY 100 // Opóźnienie w milisekundach
#define MAX_ROMAN_STRING_LENGTH 20
#define MAX_DIGIT_STRING_LENGTH 20
char romanString[MAX_ROMAN_STRING_LENGTH] = {0}; // Bufor na znaki rzymskie


char digitString[MAX_DIGIT_STRING_LENGTH] = {0}; // Bufor na cyfry

void addDigitToBuffer(char digit) {
    size_t len = strlen(digitString);
    if (len < MAX_DIGIT_STRING_LENGTH - 1) {
        digitString[len] = digit;
        digitString[len + 1] = '\0';
    }
}

void resetDigitBuffer(void) {
    memset(digitString, 0, MAX_DIGIT_STRING_LENGTH);
}

void handleDigitInput(char key) {
    if (key >= '0' && key <= '9') {
        addDigitToBuffer(key);
        LCD1602_ClearAll();
        LCD1602_Print(digitString);
    }
}

char ConvertKeyToRoman(char key) {
    switch(key) {
        case '1': return 'I';
        case '2': return 'V';
        case '3': return 'X';
        case '4': return 'L';
        case '5': return 'C';
        case '6': return 'D';
        case '7': return 'M';
        default: return ' '; // Nieaktywny klawisz
    }
}

void resetRomanBuffer(void) {
    memset(romanString, 0, MAX_ROMAN_STRING_LENGTH);
}

void addRomanCharToBuffer(char romanChar) {
    size_t len = strlen(romanString);
    if (len < MAX_ROMAN_STRING_LENGTH - 1) {
        romanString[len] = romanChar;
        romanString[len + 1] = '\0';
    }
}

void handleRstButton(void) {
    static int lastRstState = 0;
    static uint32_t lastRstChangeTick = 0;

    int currentRstState = Joystick_TestPin(JOYSTICK_RST_PORT, JOYSTICK_RST_PIN);
    if (currentRstState != lastRstState && tickCount - lastRstChangeTick > DEBOUNCE_COUNT) {
        lastRstChangeTick = tickCount;
        lastRstState = currentRstState;

        if (currentRstState) {
            if (currentMode == COMPUTER) {
                romanModeInComputerEnabled = !romanModeInComputerEnabled;
                LCD1602_ClearAll();
                LCD1602_Print(romanModeInComputerEnabled ? "Roman Mode ON" : "Roman Mode OFF");
            } else {
                LCD1602_ClearAll();
                LCD1602_Print("Brak tej opcji,");
                LCD1602_SetCursor(0, 1);
                LCD1602_Print("tylko w COMPUTER");
            }
        }
    }
}

int main(void) {
    Klaw_Init();
    LCD1602_Init();
    LCD1602_ClearAll();
    TSI_Init();
    Joystick_Init();
    Buzzer_Init();
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
            case ROMAN:
                // Obsługa trybu Roman jest realizowana w SysTick_Handler
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
    handleRstButton(); // Handle RST button within SysTick
    handleSetButton();

    char key = Klaw_Read();
    if (key != last_key) {
        debounce_counter = 0;
        last_key = key;
        if ((currentMode == MUSIC) || (buzzerEnabled && currentMode != MUSIC)) {
            if (key != 0) {
                Buzzer_PlayNoteForKey(key);
            } else {
                Buzzer_StopTone();
            }
        }
        if (key != 0) {
            if (currentMode == COMPUTER && !romanModeInComputerEnabled) {
                handleDigitInput(key);
            } else if (currentMode == COMPUTER && romanModeInComputerEnabled) {
                char romanChar = ConvertKeyToRoman(key);
                if (romanChar != ' ') {
                    addRomanCharToBuffer(romanChar);
                    LCD1602_ClearAll();
                    LCD1602_Print(romanString);
                }
            } else if (currentMode == ROMAN) {
                char romanChar = ConvertKeyToRoman(key);
                if (romanChar != ' ') {
                    addRomanCharToBuffer(romanChar);
                    LCD1602_ClearAll();
                    LCD1602_Print(romanString);
                }
            }
        }
    } else if (key != 0 && debounce_counter < DEBOUNCE_COUNT) {
        debounce_counter++;
        if (debounce_counter == DEBOUNCE_COUNT && currentMode == DEFAULT) {
            processKey(key);
        }
    }

    updateDisplay(); // Aktualizuj stan wyświetlacza
}

