/*
 * encoder.h
 *
 */

#ifndef ENCODER_H_
#define ENCODER_H_

#include <stdint.h>

#define BUTTON_ENCODER  0x01
#define BUTTON_PWR      0x02
#define BUTTON_STDBY    0x03

typedef struct
{
    void    (*const Init)();
    int8_t  (*const GetEncoder)();
    uint8_t (*const GetButtons)();
    uint8_t    (*const Port1ISR)();
    void    (*const Port2ISR)();
    uint8_t (*const TimerISR)();
}Buttons_t;

extern const Buttons_t Buttons;

#endif /* ENCODER_H_ */
