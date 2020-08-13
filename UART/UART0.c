#include <MKL46Z4.h>
#include "UART0.h"

#define UART_BUFFER_SIZE (80)
#define UART_BUFFER_NUM  (2)



char RxQueueBuffer[UART_BUFFER_NUM][UART_BUFFER_SIZE];
char *ptr;
qRecord RxQueue;

void Init_UART0() 
{
    unsigned int div = MODULE_CLOCK/(BAUD_RATE*(OVER_SAMPLE));
    ptr = (char*)RxQueueBuffer;
    Init_Queue (ptr, &RxQueue, UART_BUFFER_NUM,UART_BUFFER_SIZE);
/***************************************************************/
/* Initializes UART0 for 8 bitdata, 1 bitstop , none parity format*/
/***************************************************************/
    SIM->SCGC5 |= SIM_SCGC5_PORTA_MASK; 
    SIM->SCGC4 |= SIM_SCGC4_UART0_MASK;
    PORTA->PCR[1] &= ~(PORT_PCR_MUX_MASK); 
    PORTA->PCR[1] |= PORT_PCR_MUX(2);
    PORTA->PCR[2] &= ~(PORT_PCR_MUX_MASK);
    PORTA->PCR[2] |= PORT_PCR_MUX(2);
    SIM->SOPT2 &= ~SIM_SOPT2_UART0SRC_MASK;
    SIM->SOPT2 |= (SIM_SOPT2_UART0SRC(1)); /* UART0SRC = 01 - Select MCGFLLCLK */
    UART0->C2 &= ~(UART0_C2_RE_MASK | UART0_C2_TE_MASK);// Disable UART0
    
    MCG->C2 &= ~( 1 << 2); //External reference clock - EREFS0 = 0 // dung
    MCG->C1 = (MCG_C1_CLKS(2) | MCG_C1_FRDIV(3)); //  External reference clock and Divide Factor is 8 

    /* Configure 48MHz */
    MCG->C4 |= MCG_C4_DRST_DRS(1); // DRST_DRS = 01
    MCG->C4 |= ( 1 << 7); // DMX32 = 1;

    /* Configure baud rate */
    UART0->C4 |= UART0_C4_OSR(OVER_SAMPLE - 1);
    UART0->BDH &= ~(1 << 7); // LBKDIE = 0;
    UART0->BDH &= ~(1 << 6); // RXEDGIE = 0;
    UART0->BDH &= ~(1 << 5); // SBNS = 0 - one stop bits 
    UART0->BDL = (div & UART0_BDL_SBR_MASK); 

    NVIC_EnableIRQ(UART0_IRQn);//Enable Interrup
    UART0->C2 |= UART0_C2_RIE_MASK;
    UART0->D = 0;
    UART0->C2 |= (UART0_C2_RE_MASK | UART0_C2_TE_MASK);//Enable TE and RE
}

void UART_Send(char character)
{
       while (!(UART0->S1 & UART_S1_TDRE_MASK));
       UART0->D = character;
}

void UART_Send_String(char *p){
    while ((*p) != '\0')
    {
       while (!(UART0->S1 & UART_S1_TDRE_MASK));
       UART0->D = *(p++);
    }
}

void UART0_IRQHandler (void) 
{
        if (UART0->S1 & UART0_S1_RDRF_MASK) 
        {
            Enqueue(UART0->D,&RxQueue);
        }
}

char UART_GetData()
{
    char character = '@';
    if(check_empty_Queue(&RxQueue))
    {
        character = Dequeue(&RxQueue);
    }
    return character;
}

void disable_UART_Intrrup()
{
    UART0->C2 &= ~(UART0_C2_RE_MASK | UART0_C2_TE_MASK);
    NVIC_DisableIRQ(UART0_IRQn);
    UART0->C2 &= ~UART0_C2_RIE_MASK;
}


