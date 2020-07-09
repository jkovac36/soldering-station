
#ifndef __HD44780_CONFIG_H__
#define __HD44780_CONFIG_H__

#define HD44780_PORT        P3OUT
#define HD44780_PORTDIR     P3DIR
#define HD44780_PORTIN      P3IN

#define HD44780_RS          BIT7
#define HD44780_RW          BIT6
#define HD44780_EN          BIT5
#define HD44780_DATA        (BIT3 | BIT2 | BIT1 | BIT0)
#define HD44780_DATA_OFFSET 0

#endif // __HD44780_CONFIG_H__
