/*************************************************************
Arthor :HungCV4
Creat : 29/06/2020

Object : Project using interrup of RTC to blink LED Green and LED RED.

*************************************************************/

#include "MKL46Z4.h"

#define RED_LED_PIN         (1 << 29)
#define GREEN_LED_PIN       (1 << 5)

#define RED_LED_OFF         (FPTE->PSOR |= RED_LED_PIN)
#define RED_LED_ON          (FPTE->PCOR |= RED_LED_PIN)
#define GREEN_LED_OFF       (FPTD->PSOR |= GREEN_LED_PIN)
#define GREEN_LED_ON        (FPTD->PCOR |= GREEN_LED_PIN)

//Modifile the times for interrup function RTC
#define TIME_INTERRUP       (31768U - 1000U)

void initLed();
void initRTC();
void led_blink();

/*************************************************************
Modifile the times blink led in here
Units is seconds
*************************************************************/
//the times is LED GREEN On
static unsigned char led1_on  = 1;
//the times is LED GREEN Off
static unsigned char led1_off  = 1;
//the times is LED RED On
static unsigned char led2_on  = 2;
//the times is LED RED Off
static unsigned char led2_off  = 2;

volatile unsigned char count_time_led1  = 0;
volatile unsigned char count_time_led2  = 0;

int main()
{
    initLed();
    RED_LED_ON;
    GREEN_LED_ON;
    initRTC();
    while(1)
    {
        led_blink();
    }
}

/*************************************************************
Config I/O for LED
*************************************************************/
void initLed()
{
    /* Enable clock for PORTE & PORTD */
    SIM->SCGC5 |= ( SIM_SCGC5_PORTD_MASK | SIM_SCGC5_PORTE_MASK );

    /* Initialize the RED LED (PTE29)*/
    /* Set the PTE29 pin multiplexer to GPIO mode */
    PORTE->PCR[29] = PORT_PCR_MUX(1);
    /* Set the pin's direction to output */ 
    FPTE->PDDR |= RED_LED_PIN;
    /* Set the initial output state to high */
    FPTE->PSOR |= RED_LED_PIN;

    /* Initialize the Green LED (PTD5) */
    /* Set the PTD5 pin multiplexer to GPIO mode */
    PORTD->PCR[5]= PORT_PCR_MUX(1);
    /* Set the pin's direction to output */
    FPTD->PDDR |= GREEN_LED_PIN;
    /* Set the initial output state to high */
    FPTD->PSOR |= GREEN_LED_PIN;
}

/*************************************************************
Config RTC with out clock is 1kHz using LPO
*************************************************************/
void initRTC()
{
    SIM->SCGC6 |= SIM_SCGC6_RTC_MASK;//
    SIM->SOPT1 |= SIM_SOPT1_OSC32KSEL(3);// RTC clock in LPO 1kHz
    SIM->SOPT2 |= SIM_SOPT2_CLKOUTSEL(3);//

    RTC->SR &= ~RTC_SR_TCE_MASK;//disenable time 

    RTC->TSR = 1;
    RTC->TPR = TIME_INTERRUP;
    RTC->IER |= RTC_IER_TSIE_MASK;
    NVIC_EnableIRQ(RTC_Seconds_IRQn);

    RTC->SR |= RTC_SR_TCE_MASK;// enable time 
}

/***********************************************************************
Function led_blink() using variable count_time_led1 and count_time_led2
when blink led
***********************************************************************/
void led_blink()
{
    if(count_time_led1 == led1_on)
    {
        GREEN_LED_OFF;
    }
    else if(count_time_led1 == led1_off + led1_on)
    {
        count_time_led1 = 0;
        GREEN_LED_ON;
    }

    if(count_time_led2 == led2_on)
    {
        RED_LED_OFF;
    }
    else if(count_time_led2 == led2_off + led2_on)
    {
        count_time_led2 = 0;
        RED_LED_ON;
    }
}

/*************************************************************
RTC interup with TIME_INTERRUP second at a time
When the function is called, count_time_led1 and count_time_led2
increase by 1 unit.
*************************************************************/
void RTC_Seconds_IRQHandler()
{
    RTC->SR &= ~RTC_SR_TCE_MASK;  //disenable time 
    RTC->TSR = 1;
    RTC->TPR = TIME_INTERRUP;
    RTC->SR |= RTC_SR_TCE_MASK;  // enable time 

    count_time_led1++;
    count_time_led2++;
}
