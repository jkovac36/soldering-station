#include <buttons.h>
#include <buttons_config.h>
#include <msp430g2553.h>
#include <stdint.h>

#define BUTTON_CALIB_TIME   (1000000)
#define BUTTON_ENCODER_TIME (1000)
#define BUTTON_SW_TIME      (100000)

#define DBNC_DT     0x01
#define DBNC_CLK    0x02
#define DBNC_SW     0x04
#define DBNC_PWR    0x08

struct Var
{
    volatile uint8_t debounceFlags;
    volatile uint8_t pressedFlags;
    volatile int8_t encoderFlags;
    volatile uint8_t help;
}static volatile var;


#define EncoderEnableInterrupt(bits)    \
    do                                  \
    {                                   \
        ENC_PORT_IFG &= ~(bits);        \
        ENC_PORT_IE |= bits;            \
    }                                   \
    while(0)

#define EncoderDisableInterrupt(bits) do { ENC_PORT_IE &= ~(bits); } while(0)

#define ButtonsEnableInterrupt(bits)    \
    do                                  \
    {                                   \
        BTN_PORT_IFG &= ~(bits);        \
        BTN_PORT_IE |= bits;            \
    }                                   \
    while(0)

#define ButtonsDisableInterrupt(bits) do { BTN_PORT_IE &= ~(bits); } while(0)

#define StartButtonDebounceTimer(a)     \
    do                                  \
    {                                   \
        var.help = 0;                   \
        TIMER_CCR = TIMER_R + (a);      \
        TIMER_CCTL = CCIE;              \
    }                                   \
    while(0)


static void Init()
{
    BTN_PORT_DIR &= ~(BTN_PWR_PIN + BTN_STDBY_PIN);
    BTN_PORT_OUT |= BTN_PWR_PIN + BTN_STDBY_PIN;
    BTN_PORT_REN |= BTN_PWR_PIN + BTN_STDBY_PIN;
    BTN_PORT_IES |= BTN_PWR_PIN;
    BTN_PORT_IFG &= ~BTN_PWR_PIN;
    BTN_PORT_IE |= BTN_PWR_PIN;


    ENC_PORT_DIR &= ~(ENC_CLK_PIN + ENC_SW_PIN + ENC_DT_PIN);    // switch appropriate pins to input direction
    ENC_PORT_OUT |= ENC_CLK_PIN + ENC_SW_PIN + ENC_DT_PIN;       // switch pull-up resistor
    ENC_PORT_REN |= ENC_CLK_PIN + ENC_SW_PIN + ENC_DT_PIN;       // enable pull-up
    ENC_PORT_IES |= ENC_SW_PIN;

    if(ENC_PORT_IN & ENC_CLK_PIN)
    {
        ENC_PORT_IES |= ENC_CLK_PIN + ENC_DT_PIN;
    }
    else
    {
        ENC_PORT_IES &= ~(ENC_CLK_PIN + ENC_DT_PIN);
    }

    ENC_PORT_IFG &= ~(ENC_CLK_PIN + ENC_SW_PIN + ENC_DT_PIN);
    ENC_PORT_IE |= ENC_CLK_PIN + ENC_SW_PIN + ENC_DT_PIN;

    var.debounceFlags = 0;
    var.pressedFlags = 0;
    var.encoderFlags = 0;
}

volatile uint8_t master = 0;
static inline uint8_t EncoderISR()
{
    volatile uint8_t exit = 0;
    if((ENC_PORT_IFG & ENC_SW_PIN) != 0)
    {
        EncoderDisableInterrupt(ENC_SW_PIN);
        var.debounceFlags |= DBNC_SW;
        ENC_PORT_IFG &= ~ENC_SW_PIN;
        StartButtonDebounceTimer(BUTTON_SW_TIME);
    }

    if(ENC_PORT_IFG & ENC_CLK_PIN)
    {
        EncoderDisableInterrupt(ENC_CLK_PIN);

        var.debounceFlags |= DBNC_CLK;

        ENC_PORT_IFG &= ~(ENC_CLK_PIN);
        StartButtonDebounceTimer(BUTTON_ENCODER_TIME);
    }

    if((ENC_PORT_IFG & ENC_DT_PIN) != 0)
    {
        EncoderDisableInterrupt(ENC_DT_PIN);
        var.debounceFlags |= DBNC_DT;

        ENC_PORT_IFG &= ~ENC_DT_PIN;
        StartButtonDebounceTimer(BUTTON_ENCODER_TIME);
    }
    return exit;
}


static inline void ButtonsISR()
{
    if((BTN_PORT_IFG & BTN_PWR_PIN) != 0)
    {
        ButtonsDisableInterrupt(BTN_PWR_PIN);
        var.debounceFlags |= DBNC_PWR;
        BTN_PORT_IFG &= ~BTN_PWR_PIN;
        StartButtonDebounceTimer(BUTTON_SW_TIME);
    }
}


static inline uint8_t TimerISR()
{
    TIMER_CCTL = 0;
    volatile uint8_t retval = 0;

    if((var.debounceFlags & DBNC_SW) != 0)
    {
        if((ENC_PORT_IN & ENC_SW_PIN) == 0)
        {
            var.pressedFlags |= BUTTON_ENCODER;
            retval = 1;
        }
        var.debounceFlags &= ~DBNC_SW;
        EncoderEnableInterrupt(ENC_SW_PIN);
    }

    if((var.debounceFlags & DBNC_CLK) != 0)
    {
        if((ENC_PORT_IN & ENC_CLK_PIN) != (ENC_PORT_IES & ENC_CLK_PIN))
        {
            ENC_PORT_IES ^= ENC_CLK_PIN;
            master = 1;
        }
        var.debounceFlags &= ~DBNC_CLK;
        EncoderEnableInterrupt(ENC_CLK_PIN);
    }

    if((var.debounceFlags & DBNC_DT) != 0)
    {
        if((ENC_PORT_IN & ENC_DT_PIN) != (ENC_PORT_IES & ENC_DT_PIN))
        {
            ENC_PORT_IES ^= ENC_DT_PIN;
            if (master) {
                master = 0;
                if(((ENC_PORT_IN & ENC_CLK_PIN) && (ENC_PORT_IN & ENC_DT_PIN)) ||
                   ((ENC_PORT_IN & ENC_CLK_PIN) == 0 && (ENC_PORT_IN & ENC_DT_PIN) == 0)
                ){
                    var.encoderFlags += 1;
                } else {
                    var.encoderFlags -= 1;
                }
                retval = 1;
            }
        }
        var.debounceFlags &= ~DBNC_DT;
        EncoderEnableInterrupt(ENC_DT_PIN);
    }

    if((var.debounceFlags & DBNC_PWR) != 0)
    {
        if((BTN_PORT_IN & BTN_PWR_PIN) == 0)
        {
            var.pressedFlags |= BUTTON_PWR;
            retval = 1;
        }
        var.debounceFlags &= ~DBNC_SW;
        ButtonsEnableInterrupt(BTN_PWR_PIN);
    }
    return retval;
}


static uint8_t GetButtons()
{
    uint8_t retval = var.pressedFlags;
    var.pressedFlags = 0;
    if((BTN_PORT_IN & BTN_STDBY_PIN) == 0) retval |= BUTTON_STDBY;
    return retval;
}


static int8_t GetEncoder()
{
    int8_t retval = var.encoderFlags;
    var.encoderFlags = 0;
    return retval;
}


const Buttons_t Buttons =
{
        Init,
        GetEncoder,
        GetButtons,
        P1_ISR,
        P2_ISR,
        TA0CCRO_ISR
};
