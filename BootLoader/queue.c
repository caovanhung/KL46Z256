#include "queue.h"

char Data[ELEMENT_NUM][ELEMENT_SIZE];
static uint32_t head = 0;
static uint32_t tail = 0;
static volatile  uint8_t numberQueue = 0;

bool QUEUE_empty()
{
    bool flag_check = false;
    
    if (numberQueue == 0)
    {
        flag_check =  true;
    }
    return flag_check;
}

bool QUEUE_full()
{
    bool flag_check = false;
    
    if (numberQueue == ELEMENT_NUM)
    {
        return true;
    }
    
    return flag_check;
}

void QUEUE_push()
{
    if (QUEUE_full() == false)
    {
        tail ++;
        tail %= ELEMENT_NUM;
        numberQueue ++;
    }
}

void QUEUE_pop()
{
    if (QUEUE_empty() == false)
    {
        head ++;
        head %= ELEMENT_NUM;
        numberQueue --;
    }
}

char *QUEUE_PushLine()
{
    char *ptr = NULL;
    
    if (QUEUE_full() == false)
    {
      ptr = &Data[tail][0];
    }

    return ptr;
}

char *QUEUE_PopLine()
{
    char *ptr = 0;

    if (QUEUE_empty() == false)
    {
        ptr = &Data[head][0];
    }

    return ptr;
}