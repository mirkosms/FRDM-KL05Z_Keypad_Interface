#ifndef MODE_MANAGER_H
#define MODE_MANAGER_H

// Definicja typu wyliczeniowego Mode
typedef enum { DEFAULT, MUSIC, COMPUTER, ROMAN } Mode;


// Zmienna przechowująca aktualny tryb
extern Mode currentMode;
// Prototypy funkcji
void changeMode(Mode newMode);
void updateDisplay(void);
void handleKeyboardInput(char key);
void handleMusicMode(char key);
void handleDefaultMode(char key);
void handleComputerMode(char key);
void handleRomanMode(char key);

#endif // MODE_MANAGER_H
