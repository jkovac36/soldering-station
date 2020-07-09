/*
 * lcd.h
 *
 *  Created on: Nov 13, 2016
 *      Author: jakub
 */

#ifndef SOLDERING_STATION_LCD_H_
#define SOLDERING_STATION_LCD_H_

#include <stdint.h>

#define LCD_PORT_OUT        P3OUT
#define LCD_PORT_DIR        P3DIR
#define LCD_PORT_IN         P3IN

#define LCD_RS              BIT7
#define LCD_RW              BIT6
#define LCD_EN              BIT5
#define LCD_DATA            (BIT3 | BIT2 | BIT1 | BIT0)
#define LCD_DATA_OFFSET     0
#define LCD_BACKLIGHT       BIT4

typedef struct
{
    void (*const Init)();
    void (*const ShowTempScr)();
    void (*const RefreshActTemp)(int16_t);
    void (*const RefreshSetTemp)(int16_t);
    void (*const RefreshHeating)(uint8_t);
    void (*const ShowStdbScr)();
    void (*const ShowUnpluggScr)();
}Lcd_t;

extern const Lcd_t Lcd;

#endif /* SOLDERING_STATION_LCD_H_ */
