#include "bootloader.h"


typedef void (*pFunction)(void);

void Bootloader_JumpToApplication(void)
{
    uint32_t  JumpAddress = Read_FlashAddress(APP_ADDRESS + 4);
    pFunction Jump        = (pFunction)JumpAddress;
    
    SCB->VTOR = APP_ADDRESS;
    __set_MSP(Read_FlashAddress(APP_ADDRESS));
    Jump();
}

void Bootloader_EraseFlash(void)
{
    Erase_Multi_Sector(APP_ADDRESS,2);
}

void Bootloader_writingDataFlash(uint32_t Addr,uint8_t *Data,uint8_t count_elementDataSrec)
{
    uint8_t index = 0;
    for(index = 0; index < count_elementDataSrec ; index += 4)
    {
        Program_LongWord_8B(Addr + index, &Data[index]);
    }
    for(index = 0;index < count_elementDataSrec;index++)
    {
        Data[index] = 0xFF;
    }
}