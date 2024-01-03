#include "klaw4x4.h"

void Klaw_Init(void) {
    // Włączenie zegara dla portów A i B
    SIM->SCGC5 |= SIM_SCGC5_PORTA_MASK | SIM_SCGC5_PORTB_MASK;

    // Konfiguracja pinów wierszy jako wejść z wewnętrznymi rezystorami podciągającymi
    PORTA->PCR[ROW1] = PORT_PCR_MUX(1) | PORT_PCR_PE_MASK | PORT_PCR_PS_MASK;
    PORTA->PCR[ROW2] = PORT_PCR_MUX(1) | PORT_PCR_PE_MASK | PORT_PCR_PS_MASK;
    PORTA->PCR[ROW3] = PORT_PCR_MUX(1) | PORT_PCR_PE_MASK | PORT_PCR_PS_MASK;
    PORTB->PCR[ROW4] = PORT_PCR_MUX(1) | PORT_PCR_PE_MASK | PORT_PCR_PS_MASK;

    // Konfiguracja pinów kolumn jako wyjść
    PORTB->PCR[COL1] = PORT_PCR_MUX(1);
    PORTB->PCR[COL2] = PORT_PCR_MUX(1);
    PORTB->PCR[COL3] = PORT_PCR_MUX(1);
    PORTB->PCR[COL4] = PORT_PCR_MUX(1);

    // Ustawienie stanu wysokiego na pinach kolumn
    PTB->PDDR |= (1 << COL1) | (1 << COL2) | (1 << COL3) | (1 << COL4);
    PTB->PSOR = (1 << COL1) | (1 << COL2) | (1 << COL3) | (1 << COL4);

    // Ustawienie pinów wierszy jako wejść
    PTA->PDDR &= ~((1 << ROW1) | (1 << ROW2) | (1 << ROW3));
    PTB->PDDR &= ~(1 << ROW4);
}

char Klaw_Read(void) {
    // Stany klawiszy dla klawiatury 4x4
    const char keymap[4][4] = {
        {'1', '2', '3', 'A'},
        {'4', '5', '6', 'B'},
        {'7', '8', '9', 'C'},
        {'*', '0', '#', 'D'}
    };

    for (int col = 0; col < 4; col++) {
        PTB->PCOR = (1 << COL1) | (1 << COL2) | (1 << COL3) | (1 << COL4); // Wszystkie kolumny na stan niski
        PTB->PSOR = (1 << (COL1 + col)); // Ustawienie aktualnej kolumny na stan wysoki

        for (int row = 0; row < 4; row++) {
            int row_pin = (row < 3) ? (ROW1 + row) : ROW4;
            GPIO_Type *port = (row < 3) ? PTA : PTB;
            uint32_t row_mask = (1 << row_pin);
            
            if (!(port->PDIR & row_mask)) {
                PTB->PSOR = (1 << COL1) | (1 << COL2) | (1 << COL3) | (1 << COL4); // Reset stanu kolumn
                return keymap[row][col];
            }
        }
    }

    return '\0'; // Jeśli żaden klawisz nie jest naciśnięty
}
