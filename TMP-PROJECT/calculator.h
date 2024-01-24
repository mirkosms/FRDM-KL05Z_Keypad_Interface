#ifndef CALCULATOR_H
#define CALCULATOR_H

#include "MKL05Z4.h"
#include "frdm_bsp.h"
#include "lcd1602.h"
#include "klaw4x4.h"
#include "TPM.h"
#include "buzzer.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// Typ wyliczeniowy dla operacji kalkulatora
typedef enum { NONE, ADD, SUBTRACT, MULTIPLY, DIVIDE } Operation;

// Deklaracje funkcji kalkulatora
void processKey(char key); // Przetwarza naciśnięty klawisz kalkulatora
void resetCalculator(void); // Resetuje kalkulator do stanu początkowego
double getCurrentNumber(void); // Pobiera bieżącą liczbę z kalkulatora
void setCurrentNumber(double num); // Ustawia bieżącą liczbę w kalkulatorze
void doubleToStr(double num, char* str); // Konwertuje liczbę typu double na ciąg znaków
void deleteLastCharacter(void); // Usuwa ostatni znak z wyświetlacza kalkulatora

#endif // CALCULATOR_H
