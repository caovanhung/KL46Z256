#ifndef SREC_H
#define SREC_H

#include <math.h>
#include <stdbool.h>

#define    SREC_START_CODE              'S'
#define    SREC_START_CODE_OFFSET       0
#define    SREC_START_CODE_LEN          1
#define    SREC_TYPE_LEN                1
#define    SREC_TYPE_OFFSET             1
#define    SREC_BYTE_COUNT_LEN          2
#define    SREC_BYTE_COUNT_OFFSET       2
#define    SREC_CHECKSUM_LEN            2

typedef char boolean;
#define FALSE   (0)
#define TRUE    (1)

//define Srecord Types
enum SRecordTypes {
    S0 = 0,
    S1 = 1, 
    S2 = 2,
    S3 = 3,
    S4 = 4,
    S5 = 5, 
    S6 = 6, 
    S7 = 7,
    S8 = 8,
    S9 = 9, 
};

//define lengths of address in SREC
static int SRec_Address_Lengths[] = {
    4, // S0
    4, // S1
    6, // S2
    8, // S3
    0, // S4
    4, // S5
    6, // S6
    8, // S7
    6, // S8
    4, // S9
};

/************************************************************************************
Object: Convert char to decimal
Input : x : type char
Output: value type decimal
************************************************************************************/
  int convert_Char_to_Dec(char x);

/************************************************************************************
Object: check Record type
Input : buffer : address of array, buffer[] save value of a line Srec
        size: length of array
Output: returns TRUE if checked true, else return FALSE
************************************************************************************/
 boolean check_START(char *buffer, char size);

/************************************************************************************
Object: check BYTE COUNT
Input : buffer : address of array, buffer[] save value of a line Srec
        size: length of array
Output: returns TRUE if checked true, else return FALSE
************************************************************************************/
 boolean check_BYTE_COUNT(char *buffer, char size);

/************************************************************************************
Object: check CHECK SUM
Input : buffer : address of array, buffer[] save value of a line Srec
        size: length of array
Output: returns TRUE if checked true, else return FALSE
************************************************************************************/
 boolean check_CHECK_SUM(char *buffer, char size);

/************************************************************************************
Object: check DATA LINE COUNT
Input : buffer : address of array, buffer[] save value of a line Srec
        size: length of array
        count_LINE_DATA : srec data line number
Output: returns TRUE if checked true, else return FALSE
************************************************************************************/
 boolean check_DATA_LINE_COUNT(unsigned int *count_LINE_DATA,char *buffer, char size);

/************************************************************************************
Object: Check all indicators
Input : buffer : address of array, buffer[] save value of a line Srec
        size: length of array
        count_LINE_DATA : srec data line number
Output: returns TRUE if checked true, else return FALSE
************************************************************************************/
boolean check_SREC(char *buffer, char size,unsigned int *count_LINE_DATA);

#endif