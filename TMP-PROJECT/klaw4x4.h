#ifndef KLAW4X4_H
#define KLAW4X4_H

#include "MKL05Z4.h"

// Maski używane do identyfikacji wierszy i kolumn na porcie A
#define PTA5_8_MASK_IN_ROWS    0x000001E0
#define PTA9_12_MASK_OUT_COLS  0x00001E00
#define SET_PORTA              0x00001FE0

void Klaw_Init(void);
char Klaw_Read(void);

#endif // KLAW4X4_H
