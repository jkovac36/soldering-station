#include "lcd.h"
#include "hd44780.h"
#include "msp430g2553.h"

#define TEMP_SCR_1      0
#define TEMP_SCR_2      1
#define STDB_SCR_1      2
#define UNPLUGG_SCR_1   3

static const char screenString[4][17] =
{
    "set:  | act:  | ",
    "   .C |    .C | ", //0xDF
    "    Stand-by    ",
    " Iron unplugged ",
};

#define SET_POS 1
#define ACT_POS 9


static void Init()
{
    hd44780_init(HD44780_CMD_2_LINE_MODE);      // Display init
    __delay_cycles(500000);
    hd44780_send_command(HD44780_CMD_DISPLAY_ON);
    __delay_cycles(50000);
    hd44780_send_command(HD44780_CMD_CLEAR_DISPLAY);
    hd44780_send_command(HD44780_CMD_DISPLAY_1_LINE);
}

static void ShowTempScr()
{
    hd44780_send_command(HD44780_CMD_CLEAR_DISPLAY);
    hd44780_write_string((screenString[TEMP_SCR_1]));
    hd44780_send_command(HD44780_CMD_DISPLAY_2_LINE);
    hd44780_write_string((screenString[TEMP_SCR_2]));
}

static void RefreshTemp(int16_t temp, uint8_t position)
{
    char tempStr[4] = "   ";
    tempStr[0] = temp / 100 + 48;
    if(tempStr[0] == '0') tempStr[0] = ' ';
    tempStr[1] = (temp % 100) / 10 + 48;
    if(tempStr[1] == '0' && tempStr[0] == ' ') tempStr[1] = ' ';
    tempStr[2] = temp % 10 + 48;

    hd44780_send_command(HD44780_CMD_DISPLAY_2_LINE);
    while(--position > 0) hd44780_send_command(HD44780_CMD_MOVE_CURSOR_RIGHT);
    hd44780_write_string(tempStr);
}

static void RefreshActTemp(int16_t temp)
{
    RefreshTemp(temp, ACT_POS);
}

static void RefreshSetTemp(int16_t temp)
{
    RefreshTemp(temp, SET_POS);
}

static void RefreshHeating(uint8_t on)
{
    uint8_t i = 0;
    hd44780_send_command(HD44780_CMD_DISPLAY_2_LINE);
    for(; i < 15; i++) hd44780_send_command(HD44780_CMD_MOVE_CURSOR_RIGHT);
    hd44780_write_string(on ? "*" : " ");
}

static void ShowStdbScr()
{
    hd44780_send_command(HD44780_CMD_CLEAR_DISPLAY);
    hd44780_write_string((screenString[STDB_SCR_1]));
}

static void ShowUnpluggScr()
{
    hd44780_send_command(HD44780_CMD_CLEAR_DISPLAY);
    hd44780_write_string((screenString[UNPLUGG_SCR_1]));
}


const Lcd_t Lcd = {
    Init,
    ShowTempScr,
    RefreshActTemp,
    RefreshSetTemp,
    RefreshHeating,
    ShowStdbScr,
    ShowUnpluggScr,
};
