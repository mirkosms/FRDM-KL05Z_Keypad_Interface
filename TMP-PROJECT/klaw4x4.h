#ifndef KLAW4X4_H
#define KLAW4X4_H

#include "MKL05Z4.h"

// Maski używane do identyfikacji wierszy i kolumn na porcie A
#define PTA5_8_MASK_IN_ROWS    0x000001E0  // Maska dla wierszy, obejmująca piny PTA5 do PTA8.
#define PTA9_12_MASK_OUT_COLS  0x00001E00  // Maska dla kolumn, obejmująca piny PTA9 do PTA12.
#define SET_PORTA              0x00001FE0  // Maska ustawiająca odpowiednie piny na porcie A.

/**
 * Inicjalizuje klawiaturę matrycową 4x4.
 * Konfiguruje odpowiednie piny MCU dla interfejsu klawiatury.
 */
void Klaw_Init(void);

/**
 * Odczytuje wciśnięty klawisz na klawiaturze matrycowej 4x4.
 * Zwraca znak odpowiadający wciśniętemu klawiszowi.
 * 
 * @return Znak wciśniętego klawisza, '0' jeśli żaden klawisz nie jest wciśnięty.
 */
char Klaw_Read(void);

#endif // KLAW4X4_H
