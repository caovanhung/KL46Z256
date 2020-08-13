#ifndef QUEUE_H
#define QUEUE_H

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

/***************************************************************
define for array 2D save the data received from file srec
***************************************************************/
#define ELEMENT_NUM     4
#define ELEMENT_SIZE    1024 

// check queue full
bool QUEUE_empty();

// check queue emty
bool QUEUE_full();

// push data
void QUEUE_push();

// pop data
void QUEUE_pop();

// return pointer to push 
char *QUEUE_PushLine();

// return pointer to pop
char *QUEUE_PopLine();

#endif