/*
 * heating.h
 *
 *  Created on: Nov 5, 2016
 *      Author: jakub
 */

#ifndef SOLDERING_STATION_HEATING_H_
#define SOLDERING_STATION_HEATING_H_

#include "msp430g2553.h"
#include <stdint.h>

#define HEATING_PORT_OUT    P1OUT
#define HEATING_PORT_DIR    P1DIR
#define HEATING_PIN         BIT5

typedef enum
{
    NORMAL,
    STDBY,
    OFF
}HeatMode_t;

typedef enum
{
    READY,
    HEATING,
    PEN_UNPLUGGED
}HeatStatus_t;

typedef struct
{
    void (*const Init)();
    void (*const ControlTemperature)();
    void (*const SetMode)(HeatMode_t);
    void (*const SetTemp)(int16_t);
    int16_t (*const GetActTemp)();
    HeatStatus_t (*const GetStatus)();
}Heating_t;

extern const Heating_t Heating;

#endif /* SOLDERING_STATION_HEATING_H_ */
