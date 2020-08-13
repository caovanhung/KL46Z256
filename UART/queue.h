#ifndef QUEUE_H
#define QUEUE_H


typedef struct {
    char *StartPointer;  // con tro vao
    char *IN_CurPositiPointer; // con tro ra
    char *IN_CurLinePointer; //dai chi buffer bat dau
    char *OUT_CurPositiPointer; // dia chi buffer ket thuc
    char *OUT_CurLinePointer; // dia chi buffer ket thuc
    char BufferSize; // kich thuoc buffer ( so cot)
    unsigned int NumberEnQueued; // so queue da nhap ( so hang)
    unsigned int NumberDeQueued;
    char NumberQueue; // so queue ( so hang)
} qRecord;

typedef int bool;
#define FALSE   (0)
#define TRUE    (1)


/************************************************************************************
Object: Init of queue
Input : QBuffer : address of array Buffer[][],Buffer save value on queue
        Queue:  pointer type qRecord
        NumberQueue : column number of Buffer[][]
        BufferSize : row number of Buffer[][]
Output: No
************************************************************************************/
void Init_Queue (char *QBuffer, qRecord *Queue, char NumberQueue,char BufferSize);


/************************************************************************************
Object: get a character with Queue if queue not empty
Input : Queue : pointer type qRecord
Output: return character
************************************************************************************/
char Dequeue (qRecord *Queue);


/************************************************************************************
Object: push a character on Queue
Input : Character : character want to push on Queue
        Queue: lpointer type qRecord
Output: No
************************************************************************************/
void Enqueue (char Character, qRecord *Queue);


/************************************************************************************
Object: check queue empty or not
Input : Queue : pointer type qRecord
Output: returns TRUE if queue empty, else return FALSE
************************************************************************************/
bool check_empty_Queue(qRecord *Queue);

#endif