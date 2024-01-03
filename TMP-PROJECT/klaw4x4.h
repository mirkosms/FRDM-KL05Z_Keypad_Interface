#ifndef KLAW_H
#define KLAW_H

#include "frdm_bsp.h"

// Definicje pinów dla wierszy klawiatury
#define ROW1 5  // PTA5 jako R1
#define ROW2 7  // PTA7 jako R2
#define ROW3 6  // PTA6 jako R3
#define ROW4 0  // PTB0 jako R4

// Definicje pinów dla kolumn klawiatury
#define COL1 6  // PTB6 jako C1
#define COL2 7  // PTB7 jako C2
#define COL3 10 // PTB10 jako C3
#define COL4 11 // PTB11 jako C4

void Klaw_Init(void);
char Klaw_Read(void);

#endif  /* KLAW_H */
