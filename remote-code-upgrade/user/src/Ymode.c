#include    "Ymode.h"
#include "flash_operate.h"
#include "system.h"

unsigned    char  NameString[]="MAJOR.bin";
#define     DEBUG_PRINTF        0

 static u8 cnt=0;



/*void DeInitBootloader(void)
{
    if(RoutinesInRAM)
    {
        FLASH_Lock(FLASH_MEMTYPE_PROG);
        FLASH_Lock(FLASH_MEMTYPE_DATA);
    }
    GPIO_DeInit(UART_RX_PORT);
    GPIO_DeInit(UART_TX_PORT);
    UART3_DeInit();
}*/


u8 EraseSector(u8 SectorNumber)
{

    // u8 i;
    // FLASH_MemType_TypeDef MemType  = FLASH_MEMTYPE_PROG;

    // //check maximum allowed value - and fail if incorrect
    // if(SectorNumber > MAX_SECTOR_NUMBER)
    //     return 0;

    // //if sectors in EEPROM - recalculate sector number
    // if(SectorNumber >= SECTORS_IN_FLASH)
    // {
    //     SectorNumber -= SECTORS_IN_FLASH;
    //     MemType = FLASH_MEMTYPE_DATA; // and choose correct memory type
    // }


    // for(i = 0; i < BLOCK_PER_SECTOR; i++)
    // {

    //     FLASH_EraseBlock(((u16)SectorNumber * BLOCK_PER_SECTOR) + i, MemType);
    // }

    return 1;
}//EraseSector
//**************************************************************************
void Updata_Start(void)
{
    IAP_Erase_ALL(0x55);
//     u8 index;
//     FLASH_Unlock(FLASH_MEMTYPE_PROG);//解锁flash
//   //  RoutinesInRAM = 1;
//     for(index = 4; index < 32; index++) /*Earse 0x9000->0xFFFF****/
//         EraseSector(index);//从8000开后后的4K开始擦除(8000-9000刚好4K)
//     block_num = 0X20;
    // Serial_Putstr("\r\nErase flash finsh\r\n");
}


/*void Jump_ApplicationArea(void)
{
    Serial_Putstr("\n  Jump_ApplicationArea");
    DeInitBootloader();
    //jump to GO address
    asm("LDW X,  SP ");
    asm("LD  A,  $FF");
    asm("LD  XL, A  ");
    asm("LDW SP, X  ");
    asm("JPF $9000");
}*/
/*******************************************************************
//函数名称：void       putchar(void)
//函数功能：打印字符
//作者：daniel
//日期：20180907
********************************************************************/
void   putchar(u8 Data)
{
    // UART3_SendData8(Data);
    // while(UART3_GetFlagStatus(UART3_FLAG_TC) == RESET);
}

void serial_printf(char *str, unsigned int value)
{
    char sendData;  //=Send_MsgValue;

    while (sendData = *(str++))
    {
        if (sendData == (char)'%') // %
        {
            sendData = *(str++);
            if (sendData == (char)'d' || sendData == (char)'x') // d
            {
                if (value)
                {
                    u8 noneZero = 0;
                    unsigned int divider = 10000;
                    char dispValue;

                    if (sendData == (char)'x')
                        divider = 0x1000;
                    while (divider)
                    {
                        dispValue = value / divider;
                        value -= dispValue * divider;
                        if (dispValue)
                            noneZero = 1;
                        if (noneZero)
                        {
                            if (dispValue > 9)
                                dispValue += 55;
                            else
                                dispValue += 0x30;
                            putchar(dispValue);
                        }
                        if (sendData == (char)'d')
                            divider /= 10;
                        else
                            divider /= 0x10;
                    }
                }
                else
                    putchar('0');
            }
        }
        else
            putchar(sendData);
    }
}

void Serial_Putstr(unsigned char  *pFmt)
{
    unsigned char  ucBff; // character buffer
    while (1)
    {
        ucBff = *pFmt; // get a character
        if (ucBff == '\0') // check end of string
            break;
        putchar(ucBff);
        pFmt++;           // next
    }                          // while

}

/*******************************************************
//函数名称:static     s8    YModem_RecvByte(u8 *ReceivedData,u32 timeout)
//函数功能:串口接收一个字节数据
//输入参数：ReceivedData存放接收的数据,timeout给定超时时间
//返回值：0：正常接收 -1 ：超时接收
//作者：Daniel
//日期：20180905
//定时100MS（总共10次,若10次都没有收到上位机回应则直接跳转到APP）
//GPIO_WriteReverse(GPIOA,GPIO_PIN_2);

********************************************************/
s8    YModem_RecvByte(u8 uart_id,u8 *ReceivedData,u32 timeout)
{
    u8 data;
    while(timeout-->0)
    {
        // if(UART_GetITStatus(UART1)==IT_UART_RecData)
        // {
        //     data=UART_ReceiveData(UART1);
        //    *ReceivedData=data;
        // }
        if (USART3_QueueBuf.receive_signal_flag)
        {
            data=UART_GetByte(&USART3_QueueBuf);  
            *ReceivedData=data;
            //UART_SendData(UART1,data);

        }
        return   0;

    }
    return  1;
}

/**************************************************************
//函数名称:void    YModem_SendByte(u8 Data)
//函数功能:串口发送一个数据
//输入参数：Data  待发送数据
//作者：daniel
//日期：20180905
***************************************************************/
void   YModem_SendByte(u8           uart_id,u8 Data)
{
    // if(uart_id == UART1_TX)
    // {
	UART_SendData(UART1,Data);
    //delay_ms(20);
    //    while(UART1_GetFlagStatus(UART1_FLAG_TC) == RESET);
    // }
	// else if(uart_id == UART3_TX)
	// {
    //    UART3_SendData8(Data);
    //    while(UART3_GetFlagStatus(UART3_FLAG_TC) == RESET);
	// }
}

/******************************************************************
//函数名称:s8 YModem_RecvPacket(u8 *data, s32 *length, u32 timeout)
//函数功能:接收一个数据包
//输入参数：data：存放接收到的数据 ；length：数据包的长度；timeout：超时时间
//返回参数：0：正常接收 ；-1：接收错误
//作者：daniel
//日期：20180905
*******************************************************************/
s8 YModem_RecvPacket(u8 uart_id,u8 *data, u16 *length, u32 timeout)
{
    u16 i,Packet_size;
    u8 c;
    *length = 0;
    if(YModem_RecvByte(uart_id,&c,timeout)!=0)//接收数据包的第一个字节
    {
        return  -1;  //超时接收
    }
    switch(c)
    {
        case    SOH://128byte数据包
        {
            Packet_size=128;
            // UART_SendData(UART1,128);
        }
        break;
        case    STX://1024byte数据包
        {
            Packet_size=1024;
            // UART_SendData(UART1,24);
        }
        break;
        case    EOT://数据结束字符
        {
            return  0;//接收终止
        }
        break;
        case    CA://接收终止标志
        {
            if((YModem_RecvByte(uart_id,&c, timeout) == 0) && (c == CA))//等待接收中止字符
            {
                *length=-1;//接收到中止字符
                return  0;
            }
            else//接收超时
            {
                return  -1;
            }
        }
        break;
        default:
            // UART_SendData(UART1,0XFF);
            return  -1;
            break;
    }
    *data=c;//保存第一个字节
    for(i=1; i<Packet_size+PACKET_OVERHEAD; i++)
    {
        if(YModem_RecvByte(uart_id,data+i,timeout)!=0)
        {
            return  -1;
        }
    }
    if(data[1]!=((data[2]^0xff)&0xff))
    {
        return  -1;//接收错误
        // UART_SendData(UART1,20);
    }
    *length=Packet_size;//保存接收到的数据长度
#if      DEBUG_PRINTF
    serial_printf("The packetsize is %d\r\n",Packet_size);
#endif
    return  0;
}

/*****************************************************************
//函数名称:u16      YmodeCRC_Calculate(u8   *bytes,u16 length )
//函数功能：用CRC16/CRC_Xmode校验方式计算CRC数据
//入口参数：bytes为CRC校验的数据，length为数据长度
//返回类型：计算的CRC校验值
******************************************************************/
u16      YmodeCRC_Calculate(u8   *bytes,u16 length )
{
    u16 CRC_date=0;
    u16 i,j;
    for(i=0; i<length; i++)
    {
        CRC_date=CRC_date^bytes[i]<<8;
        for(j=0; j<8; j++)
        {
            if((CRC_date&0x8000)!=0)
            {
                CRC_date=CRC_date<<1^0X1021;
            }
            else
            {
                CRC_date=CRC_date<<1;
            }
        }
    }
    return  CRC_date&0xffff;
}

/*****************************************************************
//函数名称：s32        YModem_Receive()
//函数功能：ymodem接收
//输入参数：
//作者：daniel
//日期：20180905
******************************************************************/
u8  YModem_Receive(void)
{
    //u8  packet_data[PACKET_1K_SIZE + PACKET_OVERHEAD];
    u8  file_name[30],i;//定义文件名缓冲区
    u8  packets_received=0;//定义数据包的帧数
    u16 packet_length = 0;//定义数据包的长度
    u8  *file_ptr=0;//定义8位指针
    u16 CRC_Value=0;//CRC值
    u8 file_done=0,session_done=0;
    u32  APP_IapAddr;
	
    for(;;)//死循环，一直接收数据
    {
            switch(YModem_RecvPacket(1,packet_data,&packet_length,NAK_TIMEOUT))//不断接收数据包
            {
                case    0:
                {
                    switch(packet_length)
                    {
                        case    -1://发送中止传输
                        {
                            YModem_SendByte(1,ACK);
#if      DEBUG_PRINTF
                            Serial_Putstr("Send Packet Stop\r\n");
#endif
                            return  0;
                        }
                        break;
                        case    0:// 接收结束或接收错误
                        {
                            YModem_SendByte(1,ACK);
                            file_done = 1;//接收完成
                              
#if      DEBUG_PRINTF
                            Serial_Putstr("Receive packet finsh\r\n");
#endif
                            IAP_Erase_512B(0x1C000200,0xAA);
                            IAP_WriteOneWord(0x1C000200,0x00,0x55);//清除进入升级流程标志位
                            delay_ms(10);
                            IAP_Erase_512B(0x1C000000,0xAA);
                            IAP_WriteOneWord(0x1C000000,0x00,0x55);//升级完成标志位
                            SYS_ResetCPU();  //系统复位操作
                        }
                        break;
                        default://APP擦写操作
                        {
#if      DEBUG_PRINTF
                            serial_printf("The packetNum is %d\r\n",packet_data[PACKET_NUM_INDEX]);
#endif
                            if((packet_data[PACKET_NUM_INDEX]&0xff)!=(packets_received & 0xff))//校验
                            {
                                YModem_SendByte(1,NAK);//接收错误的数据，回复NAK
#if      DEBUG_PRINTF
                                Serial_Putstr("The packetnum Error\r\n");
                                serial_printf("The PacketNum is %d",packet_data[PACKET_NUM_INDEX]);
#endif
                            }
                            else//接收到正确的数据
                            {
                                if(packets_received == 0)//接收第一帧数据
                                {
                                    if(packet_data[PACKET_HEADER]!=0)//检查文件名称
                                    {
                                        for(i = 0,file_ptr = packet_data + PACKET_HEADER; (*file_ptr != 0) && (i < FILE_NAME_LENTH); )
                                        {
                                            file_name[i++] = *file_ptr++;//保存文件名
                                        }
                                        file_name[i] = '\0';//文件名以'\0'结束
                                        for(i=0; file_name[i] != '\0'; i++) //文件名比较
                                        {
                                            if(file_name[i]!=NameString[i])
                                            {
#if      DEBUG_PRINTF
                                                Serial_Putstr("The FileName ERROR\r\n");
#endif
                                                YModem_SendByte(1,ACK);//回复ACK
                                                file_done = 1;//停止接收
                                                session_done = 1;//结束对话
                                                return  1;//文件名错误，
                                            }
                                        }
#if      DEBUG_PRINTF
                                        Serial_Putstr("FileName OK\r\n");
                                        Serial_Putstr("Send ACK\r\n");
#endif
                                        Updata_Start();//擦除flash
                                        YModem_SendByte(1,ACK);//回复ACk
                                        YModem_SendByte(1,'C');//发送'C',询问数据
                                        delay_ms(200);
                                        // UART_SendDatas(UART1,Buf_C,2);
                                        packets_received++;//
                                    }
                                }
                                else//正常数据包
                                {
                                    CRC_Value=(u16)packet_data[packet_length+PACKET_HEADER];
                                    CRC_Value<<=8;
                                    CRC_Value|=packet_data[packet_length+PACKET_HEADER+1];
                                    if(CRC_Value==YmodeCRC_Calculate(packet_data+PACKET_HEADER,(u16)packet_length))//CRC校验
                                    {
                                        //FLASH_ProgramBlock(block_num++, FLASH_MEMTYPE_PROG, FLASH_PROGRAMMODE_STANDARD,packet_data+PACKET_HEADER);//写入数据
                                        // if(packets_received==1)
                                            APP_IapAddr=begin_adress+0x0080*(packets_received-1);
                                            IAP_WriteMultiByte(APP_IapAddr,packet_data+PACKET_HEADER,32,0x55);
                                            // IAP_WriteMultiByte(APP_IapAddr,Buf_C,128,0x55);
                                            
                                        //     delay_ms(20);
                                        // }
                                        YModem_SendByte(1,ACK);//flash烧写成功，回复ACK
                                        delay_ms(200);
                                    }
                                    else
                                    {
                                        YModem_SendByte(1,NAK);//请求重发
                                        delay_ms(200);
                                    }
                                    packets_received++;//收到的数据包个数计数
                                }
                            }
                        }
                        break;
                    }
                }
                break;
                case    1:
                {
                    /*YModem_SendByte(u8UartCom,CA);
                    YModem_SendByte(u8UartCom,CA);    //连续发送2次中止符CA
                    return 4; */     //烧写中止
                    YModem_SendByte(1,'C');//发送'C',询问数据
                }
                break;
                default:
                {
                   YModem_SendByte(1,'C');//发送'C',询问数据
                } break;
            }
            if((file_done != 0)|| (session_done != 0))//文件接收完毕或对话结束，退出循环
		    {
			   break;
		    }
        }
    return  12;
}



