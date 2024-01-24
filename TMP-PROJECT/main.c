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

// Globalne zmienne do przechowywania stanu systemu
volatile uint32_t tickCount = 0;
volatile uint32_t lastModeChangeTick = 0;
volatile uint32_t lastSetButtonTick = 0;
volatile uint32_t lastRstButtonTick = 0;
volatile uint32_t displayTimer = 0;
volatile int displayState = 0;
volatile uint32_t lastActionTime = 0;

/**
 * Główna funkcja programu.
 * Inicjalizuje urządzenia i obsługuje główną pętlę programu.
 */

int main(void) {
    // Inicjalizacja podsystemów urządzenia
    Klaw_Init();
    LCD1602_Init();
    LCD1602_ClearAll();
    TSI_Init();
    Joystick_Init();
    Buzzer_Init();
    UART_Init();
    SysTick_Config(SystemCoreClock / 100);

    // Główna pętla programu
    while (1) {
        // Odczyt wartości z suwaka TSI
        uint8_t sliderValue = TSI_ReadSlider();
        // Logika obsługi suwaka TSI
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
        // Logika obsługi joysticka
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

        // Obsługa przycisków SET i RST
        handleSetButton();
        handleRstButton();

        // Logika obsługi różnych trybów działania urządzenia
        switch (currentMode) {
            case MUSIC:
                // Logika dla trybu MUSIC
                break;
            case COMPUTER:
                // Logika dla trybu COMPUTER
                break;
            case ROMAN:
                // Logika dla trybu ROMAN
                break;
            case DEFAULT:
            default:
                // Logika dla domyślnego trybu działania
                break;
        }
    }
}

/**
 * Funkcja obsługi przerwań SysTick.
 * Wywoływana co każdy tick SysTick, służy do obsługi klawiatury i logiki programu.
 */
void SysTick_Handler(void) {
    static char last_key = 0;  // Ostatnio naciśnięty klawisz
    static int debounce_counter = 0;  // Licznik dla debouncingu klawisza

    tickCount++;  // Inkrementacja globalnego licznika czasu

    // Obsługa przycisków reset i set
    handleRstButton(); 
    handleSetButton();

    // Odczyt stanu klawiatury
    char key = Klaw_Read();
    if (key != last_key) {
        // Resetowanie licznika debouncingu przy zmianie klawisza
        last_key = key;

        // Obsługa naciśnięcia klawisza
        if (key != 0) {
            // Wywołanie funkcji obsługującej naciśnięcie klawisza w zależności od trybu
            handleKeyboardInput(key); 

            // Specjalna obsługa dla trybu MUSIC
            if (currentMode == MUSIC) {
                handleMusicMode(key);  // Odtwarzanie dźwięku
            } 
            // Obsługa kalkulatora w trybie DEFAULT
            else if (currentMode == DEFAULT) {
                processKey(key);  
            }
        } else {
            // Zatrzymanie dźwięku buzzera, gdy klawisz zostanie zwolniony
            Buzzer_StopTone();
        }
    } 
    // Logika debouncingu klawiatury
    else if (key != 0 && debounce_counter < DEBOUNCE_COUNT) {
        debounce_counter++;
    }
}

