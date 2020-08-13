/******************************************************************************************
Author :HungCV4
Created:05/07/2020
Object : Using UART0(baudrate : 115200, f = 48Mhz)
            to receive data with format file SREC.
        And, use the queue to support UART tranfer to a void data loss.
        After receiving data, check data line by line match format file SREC 
            with function is define in SREC.h, if check = TRUE-> print : >>
            if check = FALSE-> print : "error" and stop.
Define : UART_BUFFER_SIZE(is byte in a line of SREC) and UART_BUFFER_NUM(is number line)
            is define in UART0.h, using save data receve in queue
*******************************************************************************************/

#include "UART0.h"
#include "srec.h"

static char buffer[80] = {0};
static unsigned int count_LineDataSrec = 0;
static unsigned char i = 0;
void main()
{
    char character;
    bool check = TRUE,flag_while = TRUE;
    Init_UART0();
    while(flag_while)
    {
        character = UART_GetData();
        if(character != '\n' && character != '@'&& character != '\0')
        {
            buffer[i] = character;
            ++i;
        }
        else if(character == '\n')// check: if end of line
        {
            check = check_SREC(buffer,i,&count_LineDataSrec); // check SREC?
            buffer[i] = '\0';
            if(check == TRUE)
            {
                UART_Send_String(">>\n\0");
            }
            else
            {
                UART_Send_String("error\n\0");
                flag_while = FALSE;
                disable_UART_Intrrup();
            }
            i=0;
        }
    }
}