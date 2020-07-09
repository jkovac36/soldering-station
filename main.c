#include "msp430g2553.h"
#include "solderingStation.h"

#include <stdint.h>

/*
 * main.c
 */

int main(void) {
    WDTCTL = WDTPW | WDTHOLD;    // Stop watchdog timer
    //BCSCTL1 = CALBC1_12MHZ;
    //DCOCTL = CALDCO_12MHZ;
    __delay_cycles(1000);

    SolderingStation.Init();
    SolderingStation.Start();
}

#pragma vector=TIMER0_A1_VECTOR
#pragma vector=TIMER1_A1_VECTOR
#pragma vector=TIMER1_A0_VECTOR
#pragma vector=ADC10_VECTOR
#pragma vector=USCIAB0TX_VECTOR
#pragma vector=WDT_VECTOR
#pragma vector=USCIAB0RX_VECTOR
#pragma vector=NMI_VECTOR
#pragma vector=COMPARATORA_VECTOR
__interrupt void ISR_trap(void) {}
