/*******************************************************************************
* Copyright (C) 2019 China Micro Semiconductor Limited Company. All Rights Reserved.
*
* This software is owned and published by:
* CMS LLC, No 2609-10, Taurus Plaza, TaoyuanRoad, NanshanDistrict, Shenzhen, China.
*
* BY DOWNLOADING, INSTALLING OR USING THIS SOFTWARE, YOU AGREE TO BE BOUND
* BY ALL THE TERMS AND CONDITIONS OF THIS AGREEMENT.
*
* This software contains source code for use with CMS
* components. This software is licensed by CMS to be adapted only
* for use in systems utilizing CMS components. CMS shall not be
* responsible for misuse or illegal use of this software for devices not
* supported herein. CMS is providing this software "AS IS" and will
* not be responsible for issues arising from incorrect user implementation
* of the software.
*
* This software may be replicated in part or whole for the licensed use,
* with the restriction that this Disclaimer and Copyright notice must be
* included with each copy of this software, whether used in part or whole,
* at all times.
*/

/****************************************************************************/
/** \file fmc.c
**
**	History:
**	
*****************************************************************************/
/****************************************************************************/
/*	include files
*****************************************************************************/
#include "fmc.h"
/****************************************************************************/
/*	Local pre-processor symbols/macros('#define')
*****************************************************************************/

/****************************************************************************/
/*	Global variable definitions(declared in header file with 'extern')
*****************************************************************************/

/****************************************************************************/
/*	Local type definitions('typedef')
*****************************************************************************/

/****************************************************************************/
/*	Local variable  definitions('static')
*****************************************************************************/


/****************************************************************************/
/*	Local function prototypes('static')
*****************************************************************************/

/****************************************************************************/
/*	Function implementation - global ('extern') and local('static')
*****************************************************************************/

/*****************************************************************************
 ** \brief	 main
 **
 ** \param [in]  none   
 **
 ** \return 0
*****************************************************************************/

/**
  * @brief  Unlocks the FMC control register and program memory access.
  * @param  None
  * @retval None
  */
void FMC_UnLock(void)
{
    FMC->LOCK = FMC_Lock_Key;
}

/**
  * @brief  Locks the Program memory access.
  * @param  None
  * @retval None
  */
void FMC_Lock(void)
{
    FMC->LOCK = 0;
}

/**
  * @brief  Read a word at a specified address.
  * @note   To correctly run this function, the FMC_UnLock() function must be called before.
  * @note   Call the FMC_UnLock() to disable the flash memory access.
  * @param  Address: specifies the address to be read.
  * @retval ReadData: The returned value is read data.
  */
uint32_t FMC_ReadData(uint32_t address)
{
    uint32_t data = 0;
    FMC_UnLock();
    while(FMC->CON & FMC_FMCCON_FMCBBusy);
    FMC->ADR = address;
    FMC->CMD = FMC_FMCCMD_ReadData;
    while(FMC->CON & FMC_FMCCON_FMCBBusy);
    data = FMC->DAT;
    FMC_Lock();
    return data;
}

/**
  * @brief  Write a word at a specified address.
  * @note   To correctly run this function, the FMC_UnLock() function must be called before.
  * @note   Call the FLASH_Lock() to disable the flash memory access 
  * @note   Must be erased before writing a address.
  * @param  Address: specifies the address to be read.
  * @retval ReadData: The returned value is read data.
  */
void FMC_WriteData(uint32_t address,uint32_t data)
{
    FMC_UnLock();
    while(FMC->CON & FMC_FMCCON_FMCBBusy);
    FMC->ADR = address;
    FMC->DAT = data;
    FMC->CMD = FMC_FMCCMD_WriteData;
    while(FMC->CON & FMC_FMCCON_FMCBBusy);
    FMC_Lock();
}

/**
  * @brief  Erases a specified page in program memory.
  * @note   To correctly run this function, the FLASH_Unlock() function must be called before.
  * @note   Call the FLASH_Lock() to disable the flash memory access 
  * @param  pageaddress: The page address in program memory to be erased.
  * @retval none.
  */
void FMC_ErasPage(uint32_t pageaddress)
{
    FMC_UnLock();
    while(FMC->CON & FMC_FMCCON_FMCBBusy);
    FMC->ADR = pageaddress;
    FMC->CMD = FMC_FMCCMD_ErasPage;
    while(FMC->CON & FMC_FMCCON_FMCBBusy);
    FMC_Lock();
}

/**
  * @brief  Set the KEY register.
  * @note   
* @param  key : Set the data to KEY register.
  * @retval none.
  */
void FMC_SetKey(uint32_t key)
{
    FMC_UnLock();
    FMC->LOCK = key;
    FMC_Lock();
}

/**
  * @brief  Erases all FLASH pages.
  * @note   To correctly run this function, the FLASH_Unlock() function must be called before.
  * @note   Call the FLASH_Lock() to disable the flash memory access 
  * @param  None
  * @retval None
  */
void FMC_ErasAll(void)
{
    FMC_UnLock();
    while(FMC->CON &  FMC_FMCCON_FMCBBusy);
    FMC->CMD = FMC_FMCCMD_ErasAll;
    while(FMC->CON & FMC_FMCCON_FMCBBusy);
    FMC_Lock();    
}
