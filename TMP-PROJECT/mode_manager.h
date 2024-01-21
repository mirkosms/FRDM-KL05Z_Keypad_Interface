#ifndef MODE_MANAGER_H
#define MODE_MANAGER_H

// Definicja typu wyliczeniowego Mode
typedef enum { DEFAULT, MUSIC, COMPUTER } Mode;

// Zmienna przechowująca aktualny tryb
extern Mode currentMode;
// Prototypy funkcji
void changeMode(Mode newMode);
void updateDisplay(void);

#endif // MODE_MANAGER_H
