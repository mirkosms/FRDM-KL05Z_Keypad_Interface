#include "frdm_bsp.h"
#include "klaw4x4.h"
#include "lcd1602.h"
 
int main(void) {
    char buffer[2] = {'\0', '\0'}; // Bufor na jeden znak i znak końca ciągu

    Klaw_Init();       // Inicjalizacja klawiatury
    LCD1602_Init();    // Inicjalizacja wyświetlacza LCD
    LCD1602_ClearAll(); // Czyszczenie wyświetlacza
    
    while(1) {
        char key = Klaw_Read(); // Odczytanie klawisza

        if (key != '\0') {
            buffer[0] = key; // Ustawienie naciśniętego klawisza w buforze
            LCD1602_ClearAll();  // Wyczyszczenie ekranu przed wyświetleniem nowego klawisza
            LCD1602_SetCursor(0,0);  // Ustawienie kursora na początek ekranu
            LCD1602_Print(buffer);  // Wyświetlenie naciśniętego klawisza
        }
    }
}
