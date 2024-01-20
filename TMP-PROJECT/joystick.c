#include "joystick.h"
#include "lcd1602.h"
#include <stdbool.h>

// Inicjalizacja joysticka
void Joystick_Init() {
    SIM->SCGC5 |= SIM_SCGC5_PORTA_MASK | SIM_SCGC5_PORTB_MASK; // Wlaczenie zegara dla portów A i B

    // Ustawienie pinów jako wejscia z wlaczonym rezystorem pull-up
    PORTB->PCR[JOYSTICK_UP_PIN] = PORT_PCR_MUX(1) | PORT_PCR_PE_MASK | PORT_PCR_PS_MASK;
    PORTA->PCR[JOYSTICK_DOWN_PIN] = PORT_PCR_MUX(1) | PORT_PCR_PE_MASK | PORT_PCR_PS_MASK;
    PORTB->PCR[JOYSTICK_LEFT_PIN] = PORT_PCR_MUX(1) | PORT_PCR_PE_MASK | PORT_PCR_PS_MASK;
    PORTA->PCR[JOYSTICK_RIGHT_PIN] = PORT_PCR_MUX(1) | PORT_PCR_PE_MASK | PORT_PCR_PS_MASK;
    PORTB->PCR[JOYSTICK_MID_PIN] = PORT_PCR_MUX(1) | PORT_PCR_PE_MASK | PORT_PCR_PS_MASK;
    PORTA->PCR[JOYSTICK_RST_PIN] = PORT_PCR_MUX(1) | PORT_PCR_PE_MASK | PORT_PCR_PS_MASK;
    PORTA->PCR[JOYSTICK_SET_PIN] = PORT_PCR_MUX(1) | PORT_PCR_PE_MASK | PORT_PCR_PS_MASK;

    // Konfiguracja pinów jako wejscia
    PTB->PDDR &= ~(1U << JOYSTICK_UP_PIN | 1U << JOYSTICK_LEFT_PIN | 1U << JOYSTICK_MID_PIN);
    PTA->PDDR &= ~(1U << JOYSTICK_DOWN_PIN | 1U << JOYSTICK_RIGHT_PIN | 1U << JOYSTICK_RST_PIN | 1U << JOYSTICK_SET_PIN);
}

// Testowanie stanu wybranego pinu, true - naciśniety
bool Joystick_TestPin(GPIO_Type* port, uint32_t pin) {
    return !(port->PDIR & (1 << pin));
}

