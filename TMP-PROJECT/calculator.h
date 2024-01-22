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
void processKey(char key);
void resetCalculator(void);
double getCurrentNumber(void);
void setCurrentNumber(double num);
void doubleToStr(double num, char* str);
void deleteLastCharacter(void);

#endif // CALCULATOR_H
