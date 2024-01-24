#ifndef MODE_MANAGER_H
#define MODE_MANAGER_H

// Definicja typu wyliczeniowego Mode, reprezentującego różne tryby działania programu
typedef enum { DEFAULT, MUSIC, COMPUTER, ROMAN } Mode;

// Zmienna przechowująca aktualny tryb działania programu
extern Mode currentMode;

// Prototypy funkcji
void changeMode(Mode newMode); // Funkcja zmieniająca tryb działania programu
void updateDisplay(void);      // Funkcja aktualizująca wyświetlacz
void handleKeyboardInput(char key); // Funkcja obsługująca wprowadzanie danych z klawiatury
void handleMusicMode(char key);     // Funkcja obsługująca tryb MUSIC
void handleDefaultMode(char key);   // Funkcja obsługująca tryb DEFAULT
void handleComputerMode(char key);  // Funkcja obsługująca tryb COMPUTER
void handleRomanMode(char key);     // Funkcja obsługująca tryb ROMAN

#endif // MODE_MANAGER_H
