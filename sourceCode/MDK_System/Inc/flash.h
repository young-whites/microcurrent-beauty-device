#ifndef __FLASH_H
#define __FLASH_H
#include "sys.h"



#define APP_ADDR                0X0000				//APP的起始位置
#define APP_SIZE                (0x8000-0X1000)	//APP代码最大长度

#define LDROM_ADDR				0X0000				//LDROM的起始位置
#define LDROM_SIZE				0x1000				//LDROM的大小

#define DATA_ADDR				0x1C000000			//程序状态标志DATA Flash的起始位置
#define DATA_SIZE				1024				//程序状态标志DATA的大小

#define ONE_PAGE_SIZE           512                 //一页的长度

#define IAP_CHECK_ADRESS 		0x1C000000    	    //更新成功数字码存储的起始地址
#define IAP_CHECK_LENGTH		4		  			//更新成功数字码长度,最大14Byte
#define IAP_CHECK_AREA			APROM_AREA			//标志所处区域
#define	IAP_CHECK_NUMBER		0XAA,0X55,0X55,0XAA //表示APP代码区程序正常的数字码，最大14Byte

#define APP_BUFF_ADDR           0x4000		        //APP缓存区的起始位置
#define APP_BUFF_SIZE           (0x4000-0X1000)	    //APP缓存区最大长度
#define	BUFF_CHECK_NUMBER		0X55,0XAA,0XAA,0X55 //表示APP缓存区装载完备的数字码，最大14Byte

#define UID_ENC_ADRESS			(0x8000-512) 		//UID密文存储地址
#define UID_ENC_SIZE			16					//UID密文长度
#define UID_SIZE				(96/8)				//UID有效长度
#define UID_ENC_AREA_AREA		APROM_AREA			//UID密文所在的存储区域

#define APP_TO_BOOT             0x55
#define BOOT_TO_APP             0xAA
#define	APROM_AREA	            0x55				//APROM区
#define DATA_AREA               0xAA				//DATA区
#define LDROM_AREA	            0X96				//LDROM区
#define APROM_BUFF_AREA			0x69				//APP缓存区
#define UID_ENC_AREA			0x22				//UID密文存储区
extern uint8_t IAP_IapLength;	        //用于IAP操作数据长度缓存
#define	FMC_WRITE_KEY	(0x55AA6699)

uint8_t IAP_WriteOneWord(uint32_t IAP_IapAddr, uint32_t Write_IAP_IapData, uint8_t area);
void IAP_Erase_512B(uint32_t IAP_IapAddr, uint8_t area);//擦除一个块（512B）
void IAP_Erase_ALL(uint8_t area);
void IAP_Reset(void);
uint8_t IAP_WriteMultiByte(uint32_t IAP_IapAddr, uint8_t* buff, uint16_t len, uint8_t area);
uint32_t IAP_ReadOneWord(uint32_t IAP_IapAddr, uint8_t area);
void IAP_ReadMultiByte(uint32_t IAP_IapAddr, uint8_t* buff, uint16_t len, uint8_t area);
void IAP_FlagWrite(uint8_t flag);
uint8_t IAP_CheckAPP(void);
void IAP_ReadEncUID(uint8_t* buff);







#endif /*__FLASH_H*/
