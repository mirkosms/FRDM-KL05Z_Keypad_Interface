#include "MKL05Z4.h"
#include "TPM.h"

void PWM_Init() {
    // Włączenie sygnału zegara do portu B
    SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK;

    // Ustawienie PTB8 jako wyjście TPM0 CH3 (PWM dla buzzera)
    PORTB->PCR[8] |= PORT_PCR_MUX(2);
	//PORTB->PCR[9] |= PORT_PCR_MUX(2); // TPM0 CHx dla PTB9


    // Dołączenie sygnału zegara do TPM0
    SIM->SCGC6 |= SIM_SCGC6_TPM0_MASK;

    // Wybór źródła taktowania TPMx (MCGFLLCLK clock or MCGPLLCLK/2)
    SIM->SOPT2 |= SIM_SOPT2_TPMSRC(1);

    // TPM0 w trybie zliczania "w przód"
    TPM0->SC &= ~TPM_SC_CPWMS_MASK;

    // Dzielnik zegara wejściowego równy 64; zegar=655360Hz
    TPM0->SC |= TPM_SC_PS(6);

    // Ustawienie wartości MOD dla f=4kHz
    TPM0->MOD = 655360 / 4000 - 1;

    // Konfiguracja kanału 3 TPM0 (dla buzzera) w trybie "Edge-aligned PWM Low-true pulses"
    TPM0->CONTROLS[3].CnSC = TPM_CnSC_MSB_MASK | TPM_CnSC_ELSA_MASK;

    // Wypełnienie impulsu 50%
    TPM0->CONTROLS[3].CnV = (TPM0->MOD) / 2;

    // Włącz licznik TPM0
    TPM0->SC |= TPM_SC_CMOD(1);
}

