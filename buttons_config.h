/*
 * encoder_config.h
 *
 */

#ifndef SOLDERING_STATION_BUTTONS_CONFIG_H_
#define SOLDERING_STATION_BUTTONS_CONFIG_H_


/**
 * Definitions for used timer
 */

#define TIMER_CCTL  TA0CCTL0
#define TIMER_CCR   TA0CCR0
#define TIMER_R     TA0R


/**
 * Controls (Encoder + buttons + stand-by switch)
 */

#define BTN_PORT_OUT    P2OUT
#define BTN_PORT_DIR    P2DIR
#define BTN_PORT_IN     P2IN
#define BTN_PORT_REN    P2REN
#define BTN_PORT_IFG    P2IFG
#define BTN_PORT_IES    P2IES
#define BTN_PORT_IE     P2IE
#define BTN_PWR_PIN     BIT5
#define BTN_STDBY_PIN   BIT3    // not implemented yet

#define ENC_PORT_OUT    P1OUT
#define ENC_PORT_DIR    P1DIR
#define ENC_PORT_IN     P1IN
#define ENC_PORT_REN    P1REN
#define ENC_PORT_IFG    P1IFG
#define ENC_PORT_IES    P1IES
#define ENC_PORT_IE     P1IE
#define ENC_SW_PIN      BIT3
#define ENC_CLK_PIN     BIT0
#define ENC_DT_PIN      BIT2

#define P1_ISR  EncoderISR
#define P2_ISR  ButtonsISR
#define TA0CCRO_ISR TimerISR

#endif /* SOLDERING_STATION_BUTTONS_CONFIG_H_ */
