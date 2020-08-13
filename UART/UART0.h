#ifndef UART0_H
#define UART0_H

#include "queue.h"

#define MODULE_CLOCK 48000000U
#define OVER_SAMPLE  16U
#define BAUD_RATE    115200U

/*****************************************************************
Object: Setup for UART0 with clock 48Mhz, Baud rate 115200
Input:  No
Output: No
******************************************************************/
void Init_UART0();


/*****************************************************************
Object: Send a character
Input:  character want to send
Output: No
******************************************************************/
void UART_Send(char character);


/*****************************************************************
Object: Send a string
Input:  p : pointer to point a string with end of string is '\0'
Output: No
******************************************************************/
void UART_Send_String(char *p);


/*****************************************************************
Object: Interrup UART0, a character received put in Queue
Input:  No
Output: No
******************************************************************/
void UART0_IRQHandler (void);


/*****************************************************************
Object: Get a character from Queue
Input:  No
Output: a character
******************************************************************/
char UART_GetData();


/*****************************************************************
Object: disable interrup UART0
Input:  No
Output: No
******************************************************************/
void disable_UART_Intrrup();
#endif