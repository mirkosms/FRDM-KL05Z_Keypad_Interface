#ifndef BUZZER_H
#define BUZZER_H

#include "MKL05Z4.h"
#include "TPM.h"
#include "lcd1602.h"
#include "klaw4x4.h"
#include "global.h"

// Definicje częstotliwości nut
#define C4_FREQ  262
#define D4_FREQ  294
#define E4_FREQ  330
#define F4_FREQ  349
#define G4_FREQ  392
#define A4_FREQ  440
#define B4_FREQ  494
#define C5_FREQ  523  // Następna oktawa C
#define D5_FREQ  587
#define E5_FREQ  659
#define F5_FREQ  698
#define G5_FREQ  784
#define A5_FREQ  880
#define B5_FREQ  988
#define C6_FREQ  1047 // Następna oktawa C
#define D6_FREQ  1175

// Prototypy funkcji
void Buzzer_Init(void);
void Buzzer_PlayTone(uint32_t frequency);
void Buzzer_StopTone(void);
void Buzzer_PlayNoteForKey(char key);

#endif // BUZZER_H
