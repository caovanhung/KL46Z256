#include "queue.h"

char Dequeue (qRecord *Queue) 
{
    char character = '\0';
    if (Queue->NumberDeQueued <= Queue->NumberEnQueued) // Check queue empty
    {
        character = *(Queue->OUT_CurPositiPointer);
        if (Queue->OUT_CurPositiPointer >= (Queue->OUT_CurLinePointer + Queue->BufferSize - 1)
            || character == '\n' || character == '\0')
        {
            if(Queue->OUT_CurLinePointer == (Queue->StartPointer + (Queue->NumberQueue - 1)*Queue->BufferSize))
            {
                Queue->OUT_CurLinePointer = Queue->StartPointer;
            }
            else
            {
                Queue->OUT_CurLinePointer = Queue->OUT_CurLinePointer + Queue->BufferSize;
            }
            Queue->OUT_CurPositiPointer = Queue->OUT_CurLinePointer;
            Queue->NumberDeQueued++;
        }
        else
        {
            Queue->OUT_CurPositiPointer++;
        }
        if(Queue->NumberDeQueued == 0)
        {
            Queue->NumberDeQueued = 1;
        }
    }
    return character;
}

void Enqueue (char Character, qRecord *Queue) {
    if ((Queue->NumberEnQueued - Queue->NumberDeQueued) <= Queue->NumberQueue) // kiem tra queue con trong hay ko
    {
        *(Queue->IN_CurPositiPointer) = Character;
        if ((Queue->IN_CurPositiPointer >= (Queue->IN_CurLinePointer + Queue->BufferSize -1))
                    || Character == '\n' || Character == '\0')//kiem tra line con trong hay ko? 
        {
            if(Queue->IN_CurLinePointer == (Queue->StartPointer + (Queue->NumberQueue - 1)*Queue->BufferSize))
            {
                Queue->IN_CurLinePointer = Queue->StartPointer;
            }
            else
            {
                Queue->IN_CurLinePointer = Queue->IN_CurLinePointer + Queue->BufferSize;
            }
            Queue->IN_CurPositiPointer = Queue->IN_CurLinePointer;
            Queue->NumberEnQueued++;
        }
        else
        {
            Queue->IN_CurPositiPointer++;
        }

        if(Queue->NumberEnQueued == 0)
        {
            Queue->NumberEnQueued = 1;
        }
    }
}

bool check_empty_Queue(qRecord *Queue)
{
    bool check = TRUE;
    if((Queue->NumberEnQueued - Queue->NumberDeQueued) == 0)
    {
        check = FALSE;
    }
    else
    {
        check = TRUE;
    }
    return check;
}

void Init_Queue (char *QBuffer, qRecord *Queue, char NumberQueue,char BufferSize) 
{
    Queue->StartPointer = QBuffer;  // luu dia chi bat dau
    Queue->IN_CurPositiPointer = QBuffer;  // dia cho hien tai cua con tro (IN), thao tac tung byte tren line
    Queue->IN_CurLinePointer = QBuffer;// dia cho hien tai line (IN)
    Queue->OUT_CurPositiPointer = QBuffer;// dia cho hien tai cua con tro (OUT), thao tac tung byte tren line
    Queue->OUT_CurLinePointer = QBuffer;// dia cho hien tai line (OUT)
    Queue->BufferSize = BufferSize; // So byte tren 1 line
    Queue->NumberEnQueued = 0;  //So line da "get" data
    Queue->NumberDeQueued = 0; // so line da "put" data
    Queue->NumberQueue = NumberQueue; // so line toi da de xu ly, khi tran se reset ve 0
}