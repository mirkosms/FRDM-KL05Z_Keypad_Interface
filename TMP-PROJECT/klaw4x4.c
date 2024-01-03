#include "klaw4x4.h"

void Klaw_Init(void) {
    SIM->SCGC5 |= SIM_SCGC5_PORTA_MASK | SIM_SCGC5_PORTB_MASK;  // Włączenie zegara dla portu A i B

    // Inicjalizacja wierszy (wejścia)
    PORTA->PCR[5] = PORT_PCR_MUX(1) | PORT_PCR_PE_MASK | PORT_PCR_PS_MASK; // R1 - PTA5
    PORTA->PCR[7] = PORT_PCR_MUX(1) | PORT_PCR_PE_MASK | PORT_PCR_PS_MASK; // R2 - PTA7
    PORTA->PCR[6] = PORT_PCR_MUX(1) | PORT_PCR_PE_MASK | PORT_PCR_PS_MASK; // R3 - PTA6
    PORTB->PCR[0] = PORT_PCR_MUX(1) | PORT_PCR_PE_MASK | PORT_PCR_PS_MASK; // R4 - PTB0

    // Inicjalizacja kolumn (wyjścia)
    PORTB->PCR[6]  = PORT_PCR_MUX(1); // C1 - PTB6
    PORTB->PCR[7]  = PORT_PCR_MUX(1); // C2 - PTB7
    PORTB->PCR[10] = PORT_PCR_MUX(1); // C3 - PTB10
    PORTB->PCR[11] = PORT_PCR_MUX(1); // C4 - PTB11

    // Ustawienie kierunków pinów
    PTA->PDDR &= ~(1<<5 | 1<<7 | 1<<6);   // Wiersze jako wejścia
    PTB->PDDR &= ~(1<<0);                  // Wiersze jako wejścia
    PTB->PDDR |= (1<<6 | 1<<7 | 1<<10 | 1<<11);  // Kolumny jako wyjścia
}

char Klaw_Read(void) {
    const char keymap[4][4] = {
        {'*', '0', '#', 'D'},
        {'7', '8', '9', 'C'},
        {'4', '5', '6', 'B'},
        {'1', '2', '3', 'A'}
    };

    const uint32_t rows[4] = {5, 7, 6, 0}; // R1 - PTA5, R2 - PTA7, R3 - PTA6, R4 - PTB0
    const uint32_t cols[4] = {6, 7, 10, 11}; // C1 - PTB6, C2 - PTB7, C3 - PTB10, C4 - PTB11

    for (int col = 0; col < 4; col++) {
        // Ustawienie wszystkich kolumn na stan wysoki
        PTB->PSOR = (1 << cols[0]) | (1 << cols[1]) | (1 << cols[2]) | (1 << cols[3]);

        // Ustawienie aktualnej kolumny na stan niski
        PTB->PCOR = (1 << cols[col]);

        for (int row = 0; row < 4; row++) {
            GPIO_Type *port = (row < 3) ? PTA : PTB;
            uint32_t row_mask = (1 << rows[row]);

            if (!(port->PDIR & row_mask)) {
                // Resetowanie stanu wszystkich kolumn
                PTB->PSOR = (1 << cols[0]) | (1 << cols[1]) | (1 << cols[2]) | (1 << cols[3]);
                return keymap[row][col];
            }
        }
    }

    return 0; // Jeśli żaden klawisz nie jest naciśnięty
}

