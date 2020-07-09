/*
 * solderingStation.c
 *
 */
#include "solderingStation.h"
#include "buttons.h"
#include "hd44780.h"
#include "lcd.h"
#include "heating.h"

#include "msp430g2553.h"

#define START_TEMP      80
#define MAX_TEMPERATURE 420
#define MIN_TEMPERATURE 0
#define ENCODER_STEP    5

typedef enum {
    INIT=0,
    OPERATION,
    NO_IRON,
    STAND_BY
}ApplicationState_t;

struct {
    ApplicationState_t state;
    int temp;
}static globals = { INIT, 0 };

static void Init() {
    globals.state = INIT;

    TA0CTL = TASSEL_2 + MC_2;
    P1DIR |= BIT5;
    P1OUT &= ~BIT5;

    Buttons.Init();
    Lcd.Init();
    Lcd.ShowTempScr();
    Heating.Init();

    P3DIR |= BIT4;  // Lcd backlight
    P3OUT |= BIT4;
}

static void Start()
{
    globals.state = OPERATION;
    Heating.SetMode(NORMAL);
    globals.temp = START_TEMP;
    Lcd.RefreshSetTemp(globals.temp);

    while(1)
    {
        __bis_SR_register(GIE);

        globals.temp += Buttons.GetEncoder() * ENCODER_STEP;
        if (globals.temp < MIN_TEMPERATURE) {
            globals.temp = MIN_TEMPERATURE;
        } else if (globals.temp > MAX_TEMPERATURE) {
            globals.temp = MAX_TEMPERATURE;
        }

        Heating.SetTemp(globals.temp);

        Heating.ControlTemperature();

        if (Heating.GetStatus() == PEN_UNPLUGGED) {
            Lcd.ShowUnpluggScr();
            globals.state = NO_IRON;
        } else {
            if (globals.state != OPERATION) {
                Lcd.ShowTempScr();
                globals.state = OPERATION;
            }
            Lcd.RefreshSetTemp(globals.temp);
            Lcd.RefreshActTemp(Heating.GetActTemp());
            Lcd.RefreshHeating(Heating.GetStatus());
        }

        __delay_cycles(50000);
    }
}

const SolderingStation_t SolderingStation =
{
        Init,
        Start
};

#pragma vector=PORT2_VECTOR
__interrupt void Port2_GPIO_ISR()
{
    Buttons.Port2ISR();
}

#pragma vector=PORT1_VECTOR
__interrupt void Port1_GPIO_ISR()
{
    if(Buttons.Port1ISR()) {
        __bic_SR_register_on_exit(LPM0_bits);
    }
}




#pragma vector=TIMER0_A0_VECTOR
__interrupt void TimerA0_CCR0_ISR()
{
    if(Buttons.TimerISR()) {
        __bic_SR_register_on_exit(LPM0_bits);
    }
}
