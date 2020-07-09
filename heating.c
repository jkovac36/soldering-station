/*
 * heating.c
 *
 *  Created on: Nov 5, 2016
 *      Author: jakub
 */

#include "heating.h"
#include "measurement.h"

#include <stdint.h>

#define STANDBY_TEMP        90
#define TEMP_HYSTERESIS     3

struct
{
    HeatMode_t  mode;
    int16_t     temp;
    int16_t     act;
    HeatStatus_t  status;
}static var = { OFF, 0, 0, READY };

#define StartHeating() HEATING_PORT_OUT |= HEATING_PIN
#define StopHeating()  HEATING_PORT_OUT &= ~HEATING_PIN

#define HeatingSignOn()
#define HeatingSignOff()

static void Init()
{
    HEATING_PORT_DIR |= HEATING_PIN;
    Measurement.Init();
};

static void ControlTemperature()
{
    var.act = Measurement.ReadTemp();
    if(var.act < 0)
    {
        var.status = PEN_UNPLUGGED;
        StopHeating();
        return;
    }

    if(
        ( (var.mode == NORMAL) && (var.act <= (var.temp - 5)) )    ||
        ( (var.mode == STDBY) && (var.act <= (STANDBY_TEMP - TEMP_HYSTERESIS)) )
    )
    {
        StartHeating();
        var.status = HEATING;
    }
    else if(
        ( (var.mode == NORMAL) && (var.act >= (var.temp - 10)) )    ||
        ( (var.mode == STDBY) && (var.act >= (STANDBY_TEMP)) ) ||
        ( var.mode == OFF )
    )
    {
        StopHeating();
        var.status = READY;
    }
}

static void SetMode(HeatMode_t mode)
{
    var.mode = mode;
}

static void SetTemp(int16_t temp)
{
    var.temp = temp;
}

static int16_t GetActTemp()
{
    return var.act;
}

static HeatStatus_t GetStatus()
{
    return var.status;
}

const Heating_t Heating =
{
        Init,
        ControlTemperature,
        SetMode,
        SetTemp,
        GetActTemp,
        GetStatus
};
