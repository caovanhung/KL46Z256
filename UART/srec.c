#include "srec.h"

int convert_Char_to_Dec(char x)
{
    if(x == '0' || x == '1' || x == '2' || x == '3' || x == '4'
        || x == '5' || x == '6' || x == '7' || x == '8' || x == '9')
    {
        return  (int)(x - '0');
    }
        return (int)(x - 'A' + 10);
}

boolean check_START(char *buffer, char size)
{
    char i = 0;
    boolean check =  TRUE;

    if(buffer[SREC_START_CODE_OFFSET] != SREC_START_CODE) // check character 'S'
        {
            check = FALSE;
        }
    if(check == TRUE)
    {
        for(i = 1; i<=size; i++) //check character hex
        {
            if(convert_Char_to_Dec(buffer[i]) > 15)
            {
                check = FALSE;
            }
        }
        if(convert_Char_to_Dec(buffer[SREC_TYPE_OFFSET]) < 0          // check type
            || convert_Char_to_Dec(buffer[SREC_TYPE_OFFSET]) > 9 
            || convert_Char_to_Dec(buffer[SREC_TYPE_OFFSET]) == 4)
        {
            check = FALSE;
        }
    }

    return check;
}

boolean check_BYTE_COUNT(char *buffer, char size)
{
    char byte_Count = 0;
    boolean check =  TRUE;

    byte_Count = convert_Char_to_Dec(buffer[SREC_BYTE_COUNT_OFFSET])*16
                +convert_Char_to_Dec(buffer[SREC_BYTE_COUNT_OFFSET+1]);
    if((byte_Count*2) != (size - SREC_START_CODE_LEN - SREC_TYPE_LEN - SREC_BYTE_COUNT_LEN ))
    {
        check = FALSE;
    }

    return check;
}

boolean check_CHECK_SUM(char *buffer, char size)
{
    unsigned int i = 0,len_data = 0,len_address=0;
    unsigned int data=0,address=0,byte_Count=0,check_CHECKSUM = 0,checksum=0;
    boolean check =  TRUE;

    len_address = SRec_Address_Lengths[convert_Char_to_Dec(buffer[SREC_TYPE_OFFSET])];
    len_data = size - SREC_START_CODE_LEN - SREC_TYPE_LEN -SREC_BYTE_COUNT_LEN - len_address - SREC_CHECKSUM_LEN;

    // gia tri truong byte count
    byte_Count = convert_Char_to_Dec(buffer[SREC_BYTE_COUNT_OFFSET])*16
                +convert_Char_to_Dec(buffer[SREC_BYTE_COUNT_OFFSET+1]) ;

    // gia tri truong check sum
    checksum = convert_Char_to_Dec(buffer[size-2])*16
                +convert_Char_to_Dec(buffer[size-1]);

    // gia tri truong address
    for(i=0;i<len_address>>1;i++)
    {
        address +=  (convert_Char_to_Dec(buffer[SREC_START_CODE_LEN + SREC_TYPE_LEN + SREC_BYTE_COUNT_LEN + i*2])*16) 
                +convert_Char_to_Dec(buffer[SREC_START_CODE_LEN + SREC_TYPE_LEN + SREC_BYTE_COUNT_LEN + i*2 + 1]);
    }

    // gia tri truong data
    for(i = 0;i < len_data>>1 ;i++)
    {
        data += (convert_Char_to_Dec(buffer[SREC_START_CODE_LEN + SREC_TYPE_LEN + SREC_BYTE_COUNT_LEN +len_address + i*2 ])*16)
            +convert_Char_to_Dec(buffer[SREC_START_CODE_LEN + SREC_TYPE_LEN + SREC_BYTE_COUNT_LEN +len_address + i*2 + 1]);
    }

    check_CHECKSUM = byte_Count+address+data+checksum;
    if((check_CHECKSUM & 0xFF) != 0xFF)
    {
        check =  FALSE;
    }

    return check;
}

boolean check_DATA_LINE_COUNT(unsigned int *count_LINE_DATA,char *buffer,char size)
{
    boolean check =  TRUE;

    if(convert_Char_to_Dec(buffer[SREC_TYPE_OFFSET]) == 1 
		|| convert_Char_to_Dec(buffer[SREC_TYPE_OFFSET]) == 2
		|| convert_Char_to_Dec(buffer[SREC_TYPE_OFFSET]) == 3)
    {
        (*count_LINE_DATA)++;
    }

    if(convert_Char_to_Dec(buffer[SREC_TYPE_OFFSET]) == 5 || convert_Char_to_Dec(buffer[SREC_TYPE_OFFSET]) == 6)
    {
        unsigned int i = 0,len_data = 0;
        unsigned int data=0;
        len_data = size - SREC_START_CODE_LEN - SREC_TYPE_LEN -SREC_BYTE_COUNT_LEN - SREC_CHECKSUM_LEN;

        for(i = 0; i < len_data ;i++)
            {
                data += convert_Char_to_Dec(buffer[size-SREC_CHECKSUM_LEN - i-1])*pow(16,i);
            }
        if(*count_LINE_DATA != data)
        {
            check =  FALSE;
        }
    }

    return check;
}

boolean check_SREC(char *buffer, char size,unsigned int *count_LINE_DATA)
{
    boolean check =  TRUE;

    check = check_START(buffer,size) && check_BYTE_COUNT(buffer,size) 
            && check_CHECK_SUM(buffer,size) 
            && check_DATA_LINE_COUNT(count_LINE_DATA,buffer,size);
			
    return check;
}