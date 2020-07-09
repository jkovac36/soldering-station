/*
 * spi.h
 *
 *  Created on: Nov 5, 2016
 *      Author: jakub
 */

#ifndef SOLDERING_STATION_SPI_H_
#define SOLDERING_STATION_SPI_H_

#include <stdint.h>

#define SPI_CS_PORT_DIR P2DIR
#define SPI_CS_PORT_OUT P2OUT
#define SPI_CS_PIN      BIT0

#define SPI_PORT_SEL    P1SEL
#define SPI_PORT_SEL2   P1SEL2
#define SPI_PIN_MISO    BIT1
#define SPI_PIN_SCK     BIT4

typedef struct
{
    uint8_t *pCmd;
    uint8_t cmdLen;
    uint8_t *pData;
    uint8_t dataLen;
}SpiMsg_t;

typedef struct
{
    void (*const Init)();
    void (*const ReadBurst)(SpiMsg_t *pMsg);
}SPI_t;

extern const SPI_t SPI;

#endif /* SOLDERING_STATION_SPI_H_ */
