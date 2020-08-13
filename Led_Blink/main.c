#include "MKL46Z4.h"

#define DELAY_CNT_10ms       (20000)
#define DELAY_CNT_500ms      (1000000)

#define RED_LED_PIN          (1 << 29)
#define GREEN_LED_PIN        (1 << 5)

#define RED_LED_ON       (FPTE->PSOR |= RED_LED_PIN)
#define RED_LED_OFF      (FPTE->PCOR |= RED_LED_PIN)
#define GREEN_LED_ON       (FPTD->PSOR |= GREEN_LED_PIN)
#define GREEN_LED_OFF      (FPTD->PCOR |= GREEN_LED_PIN)

void initLed();
void delay(unsigned long t);
void PWM(float percent, unsigned long time);

int main(void)
{
    unsigned long i = 0;
    initLed();
    while (1)
    {
//        for(i = 0;i<12;i++)
//        {
            RED_LED_ON;
            GREEN_LED_OFF;
            delay(DELAY_CNT_500ms);
            RED_LED_OFF;
            GREEN_LED_ON;
            delay(DELAY_CNT_500ms);
//        }

//        RED_LED_ON;
//        GREEN_LED_OFF;
//        for(i = 0;i<400;i++)
//        {
//            PWM(i/4,DELAY_CNT_10ms);
//        }
//
//        RED_LED_OFF;
//        GREEN_LED_ON;    
//        for(i = 0;i<400;i++)
//        {
//            PWM(i/4,DELAY_CNT_10ms);
//        }
//
//        RED_LED_ON;
//        GREEN_LED_OFF;
//        for(i = 0;i<400;i++)
//        {
//            PWM(i/4,DELAY_CNT_10ms);
//        }
    }
}

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

/*

Object : Ham delay su dung nop = 1 chu ky may
Input: t : thoi gian delay = t*T( T la thoi gian cua 1 chu ky)
Output : Khong

*/
void delay(unsigned long t)
{
    unsigned long i = 0;
    for (i = 0; i < t; i++)
    {
        __asm("nop");
    }
}

/*
Object : Bam xung PWM
Input: percent : phan tram bam xung
        time : thoi gian bam xung
Output : Khong
*/
void PWM(float percent, unsigned long time)
{
    float t=0;
    t = (float)time*percent/100;

    FPTE->PTOR |= RED_LED_PIN;
    FPTD->PTOR |= GREEN_LED_PIN;
    delay((unsigned long)t);

    FPTE->PTOR |= RED_LED_PIN;
    FPTD->PTOR |= GREEN_LED_PIN;
    delay((unsigned long)(time-t));
}
