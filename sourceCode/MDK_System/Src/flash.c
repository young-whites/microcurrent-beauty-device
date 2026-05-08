#include "flash.h"


const unsigned char  IapCheckNum[IAP_CHECK_LENGTH] = { IAP_CHECK_NUMBER };	//APP可正常运行状态。
const unsigned char  BuffCheckNum[IAP_CHECK_LENGTH] = { BUFF_CHECK_NUMBER };	//代码缓存区代码就绪状态


static uint32_t FMC_ReadData(uint32_t address);
static void FMC_WriteData(uint32_t address, uint32_t data);
static void FMC_ErasPage(uint32_t pageaddress);

/**
  * @brief  Read a word at a specified address.
  * @note   To correctly run this function, the FMC_UnLock() function must be called before.
  * @note   Call the FMC_UnLock() to disable the flash memory access.
  * @param  Address: specifies the address to be read.
  * @retval ReadData: The returned value is read data.
  */
static uint32_t FMC_ReadData(uint32_t address)
{
    uint32_t data = 0;
    FMC_UnLock();
    while (FMC->CON & FMC_FMCCON_FMCBBusy);
    FMC->ADR = address;
    FMC->CMD = FMC_FMCCMD_ReadData;
    while (FMC->CON & FMC_FMCCON_FMCBBusy);
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
static void FMC_WriteData(uint32_t address, uint32_t data)
{
    FMC_UnLock();
    while (FMC->CON & FMC_FMCCON_FMCBBusy);
    FMC->ADR = address;
    FMC->DAT = data;
    FMC->CMD = FMC_FMCCMD_WriteData;
    while (FMC->CON & FMC_FMCCON_FMCBBusy);
    FMC_Lock();
}




/**
  * @brief  Erases a specified page in program memory.
  * @note   To correctly run this function, the FLASH_Unlock() function must be called before.
  * @note   Call the FLASH_Lock() to disable the flash memory access
  * @param  pageaddress: The page address in program memory to be erased.
  * @retval none.
  */
static void FMC_ErasPage(uint32_t pageaddress)
{
	FMC_UnLock();
	while (FMC->CON & FMC_FMCCON_FMCBBusy);
	FMC->ADR = pageaddress;
	FMC->CMD = FMC_FMCCMD_ErasPage;
	while (FMC->CON & FMC_FMCCON_FMCBBusy);
	FMC_Lock();
}




uint8_t IAP_WriteOneWord(uint32_t IAP_IapAddr, uint32_t Write_IAP_IapData, uint8_t area)
{
    FMC_WriteData(IAP_IapAddr, Write_IAP_IapData);

    if (IAP_ReadOneWord(IAP_IapAddr, area) == Write_IAP_IapData)
    {
        return 1;	//写入准确
    }
    else
    {
        return 0;	//写入有误
    }
}





void IAP_Erase_512B(uint32_t IAP_IapAddr, uint8_t area)//擦除一个块（512B）
{
    FMC_ErasPage(IAP_IapAddr);
}



void IAP_Erase_ALL(uint8_t area)
{
	uint16_t i;
	uint16_t k = 0;
	uint32_t begin_addr = 0;
	if (area == APROM_AREA)
	{
		k = (APP_SIZE / ONE_PAGE_SIZE);
		begin_addr = APP_ADDR;
	}

	// if(area==APROM_AREA)
	// {
	// 	k = (0x1000/ONE_PAGE_SIZE);
	// 	begin_addr = 0x7000;
	// }

#ifdef FLASH_BUFF_ENABLE
	else if (area == APROM_BUFF_AREA)
	{
		k = (APP_BUFF_SIZE / ONE_PAGE_SIZE);
		begin_addr = APP_BUFF_ADDR;
		area = APROM_AREA;
	}
#endif
	else if (area == DATA_AREA)
	{
		k = (DATA_SIZE / ONE_PAGE_SIZE);
		begin_addr = DATA_ADDR;
	}
#ifdef ENCRYPT_UID_ENABLE
	else if (area == UID_ENC_AREA)
	{
		k = 1;
		begin_addr = UID_ENC_ADRESS;
		area = APROM_AREA;
	}
#endif
	for (i = 0; i < k; i++)
	{
		IAP_Erase_512B(i * ONE_PAGE_SIZE + begin_addr, area);
	}
}



void IAP_Reset()
{
#ifdef ENCRYPT_UID_ENABLE		
	if (!CheckUID())
	{
		return;
	}
#endif
	FMC->LOCK = FMC_WRITE_KEY;//解锁操作，允许写入值
	FMC->CON |= 0X10;//选择复位后从APROM启动
	FMC->LOCK = 0x00;//锁定寄存器
	SYS_ResetCPU();  //复位操作  
	/* Trap the CPU */
	while (1);
}




uint8_t IAP_WriteMultiByte(uint32_t IAP_IapAddr, uint8_t* buff, uint16_t len, uint8_t area)	//写多字节IAP操作
{
	uint8_t i;
	uint8_t j = 0;
	uint32_t Write_IAP_IapData;
	for (i = 0; i < len; i++)
	{
		Write_IAP_IapData = ((uint32_t)buff[j + 3] << 24) + ((uint32_t)buff[j + 2] << 16) + ((uint32_t)buff[j + 1] << 8) + ((uint32_t)buff[j]);
		j = j + 4;
		//Write_IAP_IapData=0x8292c431;
		if (IAP_WriteOneWord(IAP_IapAddr + i * 4, Write_IAP_IapData, area) == 0)//判断写入是否正确
		{
			return 0;
		}
	}
	return 1;
}



uint32_t IAP_ReadOneWord(uint32_t IAP_IapAddr, uint8_t area)	//读单字节IAP操作
{
    uint32_t IAP_IapData;
    IAP_IapData = FMC_ReadData(IAP_IapAddr);
    return IAP_IapData;
}









void IAP_ReadMultiByte(uint32_t IAP_IapAddr, uint8_t* buff, uint16_t len, uint8_t area)
{
	uint16_t i;
	uint32_t value;
	for (i = 0; i < len; i = i + 4)
	{
		value = IAP_ReadOneWord(IAP_IapAddr + i, area);
		(*buff) = value;
		(*(buff + 1)) = (value >> 8);
		(*(buff + 2)) = (value >> 16);
		(*(buff + 3)) = (value >> 24);
		buff = buff + 4;
	}
}










void IAP_FlagWrite(uint8_t flag)
{
	uint8_t i;
	uint32_t Write_IAP_IapData;
	IAP_Erase_512B(IAP_CHECK_ADRESS, IAP_CHECK_AREA);
	if (flag == 1)
	{
		for (i = 0; i < IAP_CHECK_LENGTH; i = i + 4)
		{
			Write_IAP_IapData = ((uint32_t)IapCheckNum[i + 3] << 24) + ((uint32_t)IapCheckNum[i + 2] << 16) + ((uint32_t)IapCheckNum[i + 1] << 8) + ((uint32_t)IapCheckNum[i]);
			IAP_WriteOneWord(IAP_CHECK_ADRESS + i, Write_IAP_IapData, IAP_CHECK_AREA);
		}
	}
	else if (flag == 2)
	{
		for (i = 0; i < IAP_CHECK_LENGTH; i = i + 4)
		{
			Write_IAP_IapData = ((uint32_t)BuffCheckNum[i + 3] << 24) + ((uint32_t)BuffCheckNum[i + 2] << 16) + ((uint32_t)BuffCheckNum[i + 1] << 8) + ((uint32_t)BuffCheckNum[i]);
			IAP_WriteOneWord(IAP_CHECK_ADRESS + i, BuffCheckNum[i], IAP_CHECK_AREA);
		}
	}
}










uint8_t IAP_CheckAPP()
{
	uint8_t i;
	uint32_t value;
	volatile uint8_t temp = 1;
	for (i = 0; i < IAP_CHECK_LENGTH; i = i + 4)
	{
		value = ((uint32_t)IapCheckNum[i + 3] << 24) + ((uint32_t)IapCheckNum[i + 2] << 16) + ((uint32_t)IapCheckNum[i + 1] << 8) + ((uint32_t)IapCheckNum[i]);
		if (IAP_ReadOneWord(IAP_CHECK_ADRESS + i, IAP_CHECK_AREA) != value)
		{
			temp = 0;
			break;
		}
	}
	if (temp)
	{
		return temp;
	}
#ifdef FLASH_BUFF_ENABLE
	for (i = 0; i < IAP_CHECK_LENGTH; i = i + 4)
	{
		value = ((uint32_t)BuffCheckNum[i + 3] << 24) + ((uint32_t)BuffCheckNum[i + 2] << 16) + ((uint32_t)BuffCheckNum[i + 1] << 8) + ((uint32_t)BuffCheckNum[i]);
		if (IAP_ReadOneWord(IAP_CHECK_ADRESS + i, IAP_CHECK_AREA) != value)
		{
			break;
		}
	}
	if (i >= IAP_CHECK_LENGTH)
	{
		temp = 2;
	}
#endif
	return temp;
}





void IAP_ReadEncUID(uint8_t* buff)
{
	uint8_t i;
	uint32_t value;
	for (i = 0; i < UID_ENC_SIZE; i = i + 4)
	{
		value = IAP_ReadOneWord(UID_ENC_ADRESS + i, UID_ENC_AREA_AREA);
		buff[i] = value;
		buff[i + 1] = (value >> 8);
		buff[i + 2] = (value >> 16);
		buff[i + 3] = (value >> 24);
	}
}




#ifdef FLASH_BUFF_ENABLE
void IAP_Remap()//将缓存区的代码装载如运行区
{
	uint16_t i;
	IAP_Erase_ALL(APROM_AREA);//擦除APP运行区代码
	for (i = 0; i < APP_BUFF_SIZE; i = i + 4)
	{
		IAP_WriteOneWord(APP_ADDR + i, IAP_ReadOneWord(APP_BUFF_ADDR + i, APROM_AREA), APROM_AREA);
	}
}
#endif
