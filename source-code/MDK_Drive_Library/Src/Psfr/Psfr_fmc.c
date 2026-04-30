#include "cms32f033x.h"
#include "Psfr_fmc.h"

/*****************************************************************************
 ** 功能：	使能操作FMC寄存器
*****************************************************************************/
void FMC_UnLock(void)
{
	FMC->LOCK = FMC_Lock_Key;
}

/*****************************************************************************
 ** 功能：	禁止操作FMC寄存器
*****************************************************************************/
void FMC_Lock(void)
{
	FMC->LOCK = 0;
}

/*****************************************************************************
 ** 功能：	等待FMC编程、擦除等操作完成
*****************************************************************************/
void FMC_WaitIdle(void)
{
	while(FMC->CON & FMC_FMCCON_FMCBBusy);
}

/*****************************************************************************
 ** 功能：	擦除整个芯片
*****************************************************************************/
void FMC_ErasAll(void)
{
	FMC_UnLock();
	FMC_WaitIdle();
	FMC->CMD = FMC_FMCCMD_ErasAll;
	FMC_WaitIdle();
	FMC_Lock();
}

/*****************************************************************************
 ** 功能：	擦除CodeROM区指定的页
 ** 参数：	PageAddr: 页首地址
*****************************************************************************/
void FMC_CodeROM_ErasPage(uint32_t PageAddr)
{
	FMC_UnLock();
	FMC_WaitIdle();
	FMC->ADR = (PageAddr & FMC_WORD_ALIGN_MSK);
	FMC_WaitIdle();
	FMC->CMD = FMC_FMCCMD_ErasPage;
	FMC_WaitIdle();
	FMC_Lock();
}

/*****************************************************************************
 ** 功能：	写一个字宽度的数据到CodeROM区指定的地址
 ** 说明：	写入之前须确保已执行擦除
 ** 参数：	WordAddr: CodeROM区指定的地址
 ** 		Data:     待写入的字宽数据
*****************************************************************************/
void FMC_CodeROM_WriteWord(uint32_t WordAddr,uint32_t Data)
{
	FMC_UnLock();
	FMC_WaitIdle();
	FMC->ADR = (WordAddr & FMC_WORD_ALIGN_MSK);
	FMC_WaitIdle();
	FMC->DAT = Data;
	FMC_WaitIdle();
	FMC->CMD = FMC_FMCCMD_WriteData;
	FMC_WaitIdle();
	FMC_Lock();
}

/*****************************************************************************
 ** 功能：	从CodeROM区指定的地址读一个字宽度的数据
 ** 参数：	WordAddr: CodeROM区指定的地址
 ** 返回：	读到的字宽数据
*****************************************************************************/
uint32_t FMC_CodeROM_ReadWord(uint32_t WordAddr)
{
	uint32_t data = 0;
	FMC_UnLock();
	FMC_WaitIdle();
	FMC->ADR = (WordAddr & FMC_WORD_ALIGN_MSK);
	FMC_WaitIdle();
	FMC->CMD = FMC_FMCCMD_ReadData;
	FMC_WaitIdle();
	data = FMC->DAT;
	FMC_Lock();
	return data;
}

/*****************************************************************************
 ** 功能：	擦除DataROM区指定的页
 ** 参数：	PageAddr: 页首地址
*****************************************************************************/
void FMC_DataROM_ErasPage(uint32_t PageAddr)
{
	FMC_UnLock();
	FMC_WaitIdle();
	FMC->ADR = ((PageAddr & FMC_WORD_ALIGN_MSK) | FMC_DATA_BASE_MSK);
	FMC_WaitIdle();
	FMC->CMD = FMC_FMCCMD_ErasPage;
	FMC_WaitIdle();
	FMC_Lock();
}

/*****************************************************************************
 ** 功能：	写一个字宽度的数据到DataROM区指定的地址
 ** 说明：	写入之前须确保已执行擦除
 ** 参数：	WordAddr: DataROM区指定的地址
 ** 		Data:     待写入的字宽数据
*****************************************************************************/
void FMC_DataROM_WriteWord(uint32_t WordAddr,uint32_t Data)
{
	FMC_UnLock();
	FMC_WaitIdle();
	FMC->ADR = ((WordAddr & FMC_WORD_ALIGN_MSK) | FMC_DATA_BASE_MSK);
	FMC_WaitIdle();
	FMC->DAT = Data;
	FMC_WaitIdle();
	FMC->CMD = FMC_FMCCMD_WriteData;
	FMC_WaitIdle();
	FMC_Lock();
}

/*****************************************************************************
 ** 功能：	从DataROM区指定的地址读一个字宽度的数据
 ** 参数：	WordAddr: DataROM区指定的地址
 ** 返回：	读到的字宽数据
*****************************************************************************/
uint32_t FMC_DataROM_ReadWord(uint32_t WordAddr)
{
	uint32_t data = 0;
	FMC_UnLock();
	FMC_WaitIdle();
	FMC->ADR = ((WordAddr & FMC_WORD_ALIGN_MSK) | FMC_DATA_BASE_MSK);
	FMC_WaitIdle();
	FMC->CMD = FMC_FMCCMD_ReadData;
	FMC_WaitIdle();
	data = FMC->DAT;
	FMC_Lock();
	return data;
}

/**************************************************************************************************
* 功能:		从DataROM指定位置读取指定数量的数据
* 参数:		memAddr		待读取数据所在位置
* 			Size		待读取数据的字节个数（以字为单位读取，须确保是4的倍数）
* 			buffer		读取到的数据的存放地址
* 返回值:	0:成功
* 			1:读取地址或数量超出DataROM的容量范围
* 			2:无需读取（参数size为0）
* 说明1:	该函数访问1KB内区域的DataROM存储器。
**************************************************************************************************/
uint32_t FMC_DataROM_Read(uint32_t memAddr, uint32_t Size, uint32_t *buffer)
{
	uint32_t	i;
	
	if((memAddr+Size)>DATA_ROM_SIZE)	return 1;		// 读取地址或数量超出FLASH的容量范围
	if(!Size)							return 2;		// 无需读取
	
	for(i=0;i<Size;i+=4) {
		*buffer++ = FMC_DataROM_ReadWord(memAddr);		// 逐个字节读取
		memAddr += 4;
	}
	
	return 0;											// 成功返回
}

/**************************************************************************************************
* 功能:		将指定数量的数据写入DataROM指定位置
* 参数:		memAddr		数据写入到的目标位置
* 			Size		待写入数据的字节个数（以字为单位读取，须确保是4的倍数）
* 			buffer		待写入数据所在位置
* 返回值:	0:成功
* 			1:写入地址或数量超出DataROM的容量范围
* 			2:无需写入（参数size为0）
* 			3:写入失败
* 说明1:	该函数访问1KB内区域的DataROM存储器。
* 说明2:	写入数据之前，该函数会自动擦除写入数据所在扇区（512字节）。如果一次只更新某个扇区内的部分内容，则该扇区内的其他数据将丢失。
* 说明3:	该函数对系统扇区内的数据执行写入时，只能将0写为1，不能将1再写回0。（因为软件擦除该扇区时不会成功）
**************************************************************************************************/
uint32_t FMC_DataROM_Writ(uint32_t memAddr, uint32_t Size, uint32_t *buffer)
{
	uint32_t	i;
	uint32_t	Len;									// 单次写入的数据字节数
	uint32_t	first = 1;								// 首次写入标志
	
	if((memAddr+Size)>DATA_ROM_SIZE)	return 1;		// 写入地址或数量超出FLASH的容量范围
	if(!Size)							return 2;		// 无需写入
	
	do	{												// 当需要写入数据的数量超过一个扇区时，需要分多次擦除及写入
		if(first)	{									// 计算每次写入的字节数量
			first=0;									// 第一次写入到扇区的末尾
			Len=PAGE_SIZE-(memAddr&(PAGE_SIZE-1));
			if(Len>Size)	Len=Size;					// 总写入字节数量还不到扇区的末尾，则不能写到扇区末尾，需以实际数量为准
		}
		else		{
			if(Size<PAGE_SIZE)	Len=Size;				// 剩余的字节数量不足一个扇区（最后一次写入），以实际数量为准
			else				Len=PAGE_SIZE;			// 中间的数据，每次写整个扇区
		}
		FMC_DataROM_ErasPage(memAddr&(~(PAGE_SIZE-1)));	// 擦除当前扇区
		for(i=0;i<Len;i+=4) {						// 逐个字节写入FLASH（写入一个字节大约需要50微秒，写入期间，程序停止执行）（XBYTE本是外部数据存储区的首地址(0)，但是因为PSCTL.0已被置1，所以XBYTE临时变成了程序存储区的首地址(0)）
			FMC_DataROM_WriteWord(memAddr, *buffer++);
			memAddr += 4;
		}
		Size-=Len;
	}while(Size);
	
	return 0;
}

