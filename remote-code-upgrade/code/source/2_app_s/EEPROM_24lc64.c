
#include <stdio.h>
#include "CMS32F030x.h"

#include "EEPROM_24LC64.h"
#include "iic.h"
#include "systick.h"

#define EEPROM_READ_ADDR      0xA1 /* Address of slave for read  */
#define EEPROM_WRITE_ADDR     0xA0 /* Address of slave for write */

/**
  * @brief Open I2C interface to access EEPROM
  * @param None
  * @return None
  */
void EEPROM_Init(void)
{
    /* Open I2C module and set bus clock */
    I2C_DeInit(I2C0);

    SYS_IOPConfig(E_P32,P32SDA0);
    SYS_IOPConfig(E_P35,P35SCL0);

    SYS_APBPeriphClockCmd(SYS_APB_I2C0CE,ENABLE);
    I2C_InitTypeDef i2c0_inittype;
    I2C_StructInit(&i2c0_inittype);

    i2c0_inittype.I2C_Mode = I2C_Mode_Mast;
    i2c0_inittype.I2C_OwnAddressType = I2C_OwnAddress_7Bit;
    i2c0_inittype.I2C_Ack = I2C_ACK_Use;
    i2c0_inittype.I2C_Address = 0x5;
    I2C_Init(I2C0,&i2c0_inittype);

    I2C_ClkSet(I2C0,100000);//100KHz
    I2C_Cmd(I2C0,ENABLE);

//    I2C_ITConfig(I2C0,ENABLE);
//    NVIC_ClearPendingIRQ(I2C0_IRQn);
//    NVIC_EnableIRQ(I2C0_IRQn);
}

/**
  * @brief Write data to EEPROM
  * @param u32Addr Address for writing
  * @param u8Data Data for writing
  * @return None
  */
void EEPROM_Write(uint32_t u32Addr, uint8_t *pu8Buf, uint32_t u32Size)//(uint32_t u32Addr, uint8_t u8Data)
{
    int32_t i32Err;
		int32_t i,repeat=10;
		
    do
    {
        i32Err = 0;

        /* Send start */
        I2C_GenerateSTART(I2C0, ENABLE);
        I2C_WAIT_READY(I2C0);
				I2C_ITClear(I2C0);
        /* Send control byte */
        I2C_SendData(I2C0, EEPROM_WRITE_ADDR);
        I2C_WAIT_READY(I2C0);

        if(I2C_GetStatusFlag(I2C0) == I2C_STAT_AddrWriteAck)
        {
            /* ACK */
						I2C_ITClear(I2C0);
            /* Send high address */
//            I2C_SendData(I2C0, (u32Addr >> 8) & 0xFFUL);//high address
//            I2C_WAIT_READY(I2C0);
//            if(I2C_GetStatusFlag(I2C0) == I2C_STAT_MDataTxAck)//
            {
                /* ACK */
//								I2C_ITClear(I2C0);
                /* Send low address */
                I2C_SendData(I2C0, u32Addr & 0xFFUL);//low address
                I2C_WAIT_READY(I2C0);
                if(I2C_GetStatusFlag(I2C0) == I2C_STAT_MDataTxAck)
                {
                    
										/* ACK */
										I2C_ITClear(I2C0);
                    for(i = 0; i < u32Size; i++)
                    {
                        /* Send data */
                        I2C_SendData(I2C0, pu8Buf[i]);//data
                        
                        I2C_WAIT_READY(I2C0);
                        if(I2C_GetStatusFlag(I2C0) == I2C_STAT_MDataTXNAck)
                        {
                            /* NACK */

                            /* Send data error */
                            i32Err = 4;
                        }
												I2C_ITClear(I2C0);
                    }

                    /* Send stop when no any error */
                    if(i32Err == 0)
                    {
                        /* Send stop */
                        I2C_GenerateSTOP(I2C0);
												delay_ms(10);
												return;
                    }
                }
                else
                {
                    /* NACK */

                    /* Send low address error */
                    i32Err = 3;
                }
            }
//            else
            {
                /* NACK */

                /* Send high address error */
//                i32Err = 2;
            }
        }
        else
        {
            /* NACK */

            /* Send control error */
            i32Err = 1;
        }

        if(i32Err)
        {

            /* Send stop */
            I2C_GenerateSTOP(I2C0);

            delay_ms(1);
        }

    }while(--repeat);

}

/**
  * @brief Read data from EEPROM
  * @param u32Addr Address for reading
  * @return Data
  */
uint8_t EEPROM_Read(uint32_t u32Addr, uint8_t *pu8Buf, uint32_t u32Size)//(uint32_t u32Addr)
{
    int32_t i32Err;
    int32_t i;
		int32_t repeat=10;
    do
    {
        i32Err = 0;

        /* Send start */
        I2C_GenerateSTART(I2C0, ENABLE);
        I2C_WAIT_READY(I2C0);
				I2C_ITClear(I2C0);
        /* Send control byte */
        I2C0->DAT = EEPROM_WRITE_ADDR;//I2C_SendData(I2C0, EEPROM_WRITE_ADDR);
        
        I2C_WAIT_READY(I2C0);
				
        if(I2C_GetStatusFlag(I2C0) == I2C_STAT_AddrWriteAck)
        {
            /* ACK */
						I2C_ITClear(I2C0);
            /* Send high address */
//            I2C_SendData(I2C0, (u32Addr >> 8) & 0xFFUL);//high address
            
//            I2C_WAIT_READY(I2C0);
//            if(I2C_GetStatusFlag(I2C0) == I2C_STAT_MDataTxAck)//
            {
                /* ACK */
//							I2C_ITClear(I2C0);
                /* Send low address */
                I2C0->DAT = u32Addr & 0xFFUL;//I2C_SendData(I2C0, u32Addr & 0xFFUL);//low address
                
                I2C_WAIT_READY(I2C0);
                if(I2C_GetStatusFlag(I2C0) == I2C_STAT_MDataTxAck)
                {
                    /* ACK */
										I2C_ITClear(I2C0);
                    /* Send data */
                    I2C_GenerateSTART(I2C0, ENABLE);
                    I2C_WAIT_READY(I2C0);
                    if(I2C_GetStatusFlag(I2C0) == I2C_STAT_ReStart)
                    {
                        /* ACK */
												I2C_ITClear(I2C0);
                        /* Send control byte */
                        I2C0->DAT = EEPROM_READ_ADDR;//I2C_SendData(I2C0, EEPROM_READ_ADDR);
                        
                        I2C_WAIT_READY(I2C0);
                        if(I2C_GetStatusFlag(I2C0) == I2C_STAT_AddrReadAck)
                        {
														I2C_ITClear(I2C0);
                            for(i = 0; i < u32Size - 1; i++)
                            {
                                
																I2C_AcknowledgeConfig(I2C0, ENABLE);
                                I2C_WAIT_READY(I2C0);
																
                                /* Read data */
                                pu8Buf[i] = I2C0->DAT;//I2C_ReceiveData(I2C0);
																I2C_ITClear(I2C0);
                            }
														I2C_AcknowledgeConfig(I2C0, DISABLE);
                            
                            I2C_WAIT_READY(I2C0);
                            pu8Buf[i] = I2C0->DAT;//I2C_ReceiveData(I2C0);
														I2C_ITClear(I2C0);
														/* Send stop */
														I2C_GenerateSTOP(I2C0);
														return u32Size;
														
                        }
                        else
                        {
                            /* NACK */

                            /* Send control read error */
                            i32Err = 5;
                        }
                    }
                    else
                    {
                        /* NACK */

                        /* Send start error */
                        i32Err = 4;
                    }
                }
                else
                {
                    /* NACK */

                    /* Send low address error */
                    i32Err = 3;
                }
            }
//            else
            {
                /* NACK */

                /* Send high address error */
//                i32Err = 2;
            }
        }
        else
        {
            /* NACK */

            /* Send control write error */
            i32Err = 1;

        }

        if(i32Err)
        {
            /* Send stop */
            I2C_GenerateSTOP(I2C0);

            //CLK_SysTickDelay(10);
        }

    }while(--repeat);

    return u32Size;
}


uint8_t eepromData[8];/* =
{
	0x5a,0x55,0xaa,0xa5,0x0f,0xf0,0x12,0x34,
};*/

uint32_t eepromAddr=0x08;
uint32_t eepromCount=0;
uint32_t rw=0;

void I2C0_IRQHandler(void)
{
    if(I2C_GetITStatus(I2C0) == SET)
    {
        switch(I2C0->STAT)
        {
          case I2C_STAT_Start:
					{
								
								I2C_SendData(I2C0, EEPROM_WRITE_ADDR);
								//I2C_ITClear(I2C0);
								break;
						
					}
					case I2C_STAT_AddrWriteAck:
					{
								
								I2C_SendData(I2C0, eepromAddr & 0xFFUL);
								//I2C_ITClear(I2C0);
								break;
						
					}
					case I2C_STAT_MDataTxAck:
					{
						if(rw)
						{
							I2C_ITClear(I2C0);
							I2C_GenerateSTART(I2C0, ENABLE);
						}
						else
						{
							if(eepromCount>=sizeof(eepromData))
							{
								eepromCount = 0;
								I2C_GenerateSTOP(I2C0);
								break;
							}							
							I2C_SendData(I2C0, eepromData[eepromCount]);
							eepromCount++;
							//I2C_ITClear(I2C0);
						}
						break;
						
					}
					case I2C_STAT_ReStart:
					{
								
								I2C_SendData(I2C0, EEPROM_READ_ADDR);
								//I2C_ITClear(I2C0);
								break;
						
					}
					
					case I2C_STAT_AddrReadAck:
					{
						I2C_AcknowledgeConfig(I2C0, ENABLE);
						I2C_ITClear(I2C0);
						break;
						
					}
					case I2C_STAT_DataRevedAck:
					{
						
						eepromData[eepromCount] = I2C_ReceiveData(I2C0);
						if(++eepromCount>=(sizeof(eepromData)-1)) I2C_AcknowledgeConfig(I2C0, DISABLE);
						
						break;
					}
					
					case I2C_STAT_DataRevedNAck:
					{
						eepromData[eepromCount] = I2C_ReceiveData(I2C0);
						eepromCount = 0;
						I2C_GenerateSTOP(I2C0);
						break;
					}
					
					default:
					{
						I2C_GenerateSTOP(I2C0);
						
					}
					
        }
    }
		
    NVIC_ClearPendingIRQ(I2C0_IRQn);
}

