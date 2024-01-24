#include "calculator.h"

// Zmienne globalne, używane tylko wewnątrz tego modułu
static double currentNumber = 0.0;  // Bieżąca liczba wprowadzona przez użytkownika
static double storedNumber = 0.0;   // Zapamiętana liczba dla operacji arytmetycznych
static double decimalMultiplier = 0.1;  // Mnożnik dla liczb dziesiętnych
static Operation currentOperation = NONE;  // Bieżąca wybrana operacja (np. dodawanie, odejmowanie)
static int decimalEntered = false;  // Flag indicating whether a decimal point has been entered
static int isNegative = false;      // Flag indicating if the current number is negative

// Resetuje kalkulator do stanu początkowego
void resetCalculator(void) {
    currentNumber = 0.0;
    storedNumber = 0.0;
    decimalMultiplier = 0.1;
    currentOperation = NONE;
    decimalEntered = false;
    isNegative = false;
}

// Usuwa ostatni znak z bieżącej liczby
void deleteLastCharacter(void) {
    if (currentNumber >= 10 || currentNumber <= -10) {
        currentNumber = (int)(currentNumber / 10);
    } else if (currentNumber <= 9 && currentNumber >= -9) {
        // W przypadku liczb jednocyfrowych resetujemy do 0
        currentNumber = 0;
        isNegative = false;
    }
    // Aktualizacja wyświetlacza
    char buffer[16];
    doubleToStr(currentNumber, buffer);
    LCD1602_ClearAll();
    LCD1602_Print(buffer);
}

// Pobiera bieżącą liczbę
double getCurrentNumber(void) {
    return currentNumber;
}

// Ustawia bieżącą liczbę
void setCurrentNumber(double num) {
    currentNumber = num;
}

// Funkcja obsługująca naciśnięcie klawisza klawiatury
void processKey(char key) {
    char buffer[16];  // Bufor na ciąg znaków do wyświetlania
    char displayString[2] = {key, '\0'}; // Bufor na wyświetlanie naciśniętego klawisza

    // Odtwarzanie dźwięku tylko gdy buzzer włączony
    if (buzzerEnabled) {
        Buzzer_PlayNoteForKey(key);
    }

    // Logika dla różnych klawiszy
    if (key == '-' && currentNumber == 0.0 && !decimalEntered && currentOperation == NONE) {
        // Obsługa zmiany znaku liczby
        isNegative = !isNegative;
        LCD1602_ClearAll();
        LCD1602_Print("-0");
    } else if (key >= '0' && key <= '9') {
        // Obsługa wprowadzania cyfr
        double numValue = (key - '0');
        if (!decimalEntered) {
            // Przed wprowadzeniem przecinka dziesiętnego
            currentNumber = currentNumber * 10.0 + (isNegative ? -numValue : numValue);
        } else {
            // Po wprowadzeniu przecinka dziesiętnego
            currentNumber += (isNegative ? -numValue : numValue) * decimalMultiplier;
            decimalMultiplier *= 0.1;
        }
        doubleToStr(currentNumber, buffer);
        LCD1602_ClearAll();
        LCD1602_Print(buffer);
    } else if (key == '.') {
        // Obsługa wprowadzenia kropki dziesiętnej
        if (!decimalEntered) {
            decimalEntered = true;
            doubleToStr(currentNumber, buffer);
            strcat(buffer, ".");
            LCD1602_ClearAll();
            LCD1602_Print(buffer);
        }
    } else if (key == '+' || key == '-' || key == '*' || key == '/') {
        // Obsługa wyboru operacji arytmetycznej
        storedNumber = currentNumber;
        currentNumber = 0.0; // Resetowanie bieżącej liczby
        decimalEntered = false; // Resetowanie wprowadzania części dziesiętnej
        isNegative = false; // Resetowanie znaku na dodatni dla nowej liczby
        decimalMultiplier = 0.1;
        currentOperation = (key == '+') ? ADD :
                           (key == '-') ? SUBTRACT :
                           (key == '*') ? MULTIPLY : DIVIDE;

        LCD1602_ClearAll();
        LCD1602_Print(displayString);
    } else if (key == '=') {
        // Obsługa wykonywania obliczeń
        if (currentOperation == DIVIDE && currentNumber == 0.0) {
            // Obsługa dzielenia przez zero
            LCD1602_ClearAll();
            LCD1602_Print("N/D");
        } else {
            // Wykonywanie wybranej operacji arytmetycznej
            switch (currentOperation) {
                case ADD: currentNumber += storedNumber; break;
                case SUBTRACT: currentNumber = storedNumber - currentNumber; break;
                case MULTIPLY: currentNumber *= storedNumber; break;
                case DIVIDE: currentNumber = storedNumber / currentNumber; break;
                default: break;
            }
            doubleToStr(currentNumber, buffer);
            LCD1602_ClearAll();
            LCD1602_Print(buffer);
        }
        currentOperation = NONE;
        decimalEntered = false;
        decimalMultiplier = 0.1;
    }
}

/**
 * Konwertuje liczbę typu double na ciąg znaków.
 * 
 * @param num Liczba typu double do konwersji.
 * @param str Bufor na ciąg znaków reprezentujący liczbę.
 */
void doubleToStr(double num, char* str) {
    int isNegative = 0; // Flaga określająca, czy liczba jest ujemna.
    if (num < 0) {
        isNegative = 1; // Ustaw flagę na prawdę, jeśli liczba jest ujemna.
        num = -num; // Zmień znak liczby na dodatni dla dalszej obróbki.
    }

    int intPart = (int)num; // Część całkowita liczby.
    int decimalPart = (int)((num - (double)intPart) * 100 + 0.5); // Oblicz część dziesiętną i zaokrąglij.

    // Obsługa przepełnienia po zaokrągleniu.
    if (decimalPart >= 100) {
        decimalPart -= 100;
        intPart += 1;
    }

    int i = 0; // Indeks bufora str.
    if (isNegative) {
        str[i++] = '-'; // Dodaj znak minusa na początku, jeśli liczba jest ujemna.
    }

    // Konwersja części całkowitej na string.
    int tempIntPart = intPart;
    do {
        int digit = tempIntPart % 10;
        str[i++] = '0' + digit; // Konwersja cyfry na znak i dodanie do bufora.
        tempIntPart /= 10;
    } while (tempIntPart > 0);

    // Odwrócenie ciągu znaków części całkowitej.
    int start = isNegative ? 1 : 0;
    for (int j = start; j < (i + start) / 2; ++j) {
        char temp = str[j];
        str[j] = str[i - 1 - j + start];
        str[i - 1 - j + start] = temp;
    }

    // Dodanie części dziesiętnej tylko jeśli różna od zera.
    if (decimalPart > 0) {
        str[i++] = '.'; // Dodaj kropkę dziesiętną.
        str[i++] = '0' + (decimalPart / 10); // Dodaj pierwszą cyfrę po przecinku.
        str[i++] = '0' + (decimalPart % 10); // Dodaj drugą cyfrę po przecinku.

        // Usuwanie niepotrzebnych zer na końcu.
        if (str[i-1] == '0') {
            i--;
            if (str[i-1] == '0') {
                i -= 2; // Usuń także kropkę, jeśli obie cyfry dziesiętne to zera.
            }
        }
    }
    str[i] = '\0'; // Zakończ string znakiem null.
}
