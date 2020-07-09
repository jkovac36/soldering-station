/*
 * spi.c
 *
 *  Created on: Nov 5, 2016
 *      Author: jakub
 */

#include "spi.h"

#include "msp430g2553.h"
#include <stdint.h>

#define SPI_CS_ON  SPI_CS_PORT_OUT &= ~SPI_CS_PIN
#define SPI_CS_OFF SPI_CS_PORT_OUT |= SPI_CS_PIN

static void Init()
{
    UCA0CTL1 = 0x01;

    // TODO Now all regarding USCA0 interface
    UCA0CTL0 = 0xA9;
    UCA0CTL1 |= 0x80;
    UCA0BR1 = 0;
    UCA0BR0 = 4;

    SPI_CS_PORT_DIR |= SPI_CS_PIN;
    SPI_CS_OFF;

    SPI_PORT_SEL |= SPI_PIN_MISO + SPI_PIN_SCK;
    SPI_PORT_SEL2 |= SPI_PIN_MISO + SPI_PIN_SCK;

    UCA0CTL1 &= ~0x01;
}

static void ReadBurst(SpiMsg_t *pMsg)
{
    uint8_t toSend = pMsg->cmdLen;
    uint8_t* pAct = pMsg->pCmd;

    SPI_CS_ON;                                      // Enable chip select
    __delay_cycles(1000);
    while(toSend-- > 0){                            // Loop for command transmision
        while (UCA0STAT & UCBUSY);                  // Check if TX buffer is ready
        UCA0TXBUF = *pAct++;                        // Send byte
    }

    while (UCA0STAT & UCBUSY);                      // Check if TX buffer is ready
    UCA0RXBUF;

    toSend = pMsg->dataLen;
    pAct = pMsg->pData;
    while(toSend-- > 0){                            // Loop for data reception
        while (UCA0STAT & UCBUSY);                  // Check if TX buffer is ready
        UCA0TXBUF = 0xFF;                           // Send dummy byte
        while (UCA0STAT & UCBUSY);                  // Check if byte reception is completed
        *pAct++ = UCA0RXBUF;                        // Read byte from buffer
    }
    SPI_CS_OFF;                                     // Disable chip select
}

const SPI_t SPI =
{
    Init,
    ReadBurst
};

