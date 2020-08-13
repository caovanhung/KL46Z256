#ifndef __BOOTLOADER_H
#define __BOOTLOADER_H

#include <MKL46Z4.h>
#include "stdint.h"
#include "Flash.h"

/***************************************************************
define address of are app
***************************************************************/
#define APP_ADDRESS ((uint32_t)0x0000A000)

/***************************************************************
                            API                                 
***************************************************************/


/************************************************
define for UART0
*************************************************/
void Bootloader_JumpToApplication(void);


/************************************************
Erase the flash Sector
*************************************************/
void Bootloader_EraseFlash(void);


/************************************************
write Data in Addr of Flash
*************************************************/
void Bootloader_writingDataFlash(uint32_t Addr,uint8_t *Data,uint8_t count_elementDataSrec);

#endif