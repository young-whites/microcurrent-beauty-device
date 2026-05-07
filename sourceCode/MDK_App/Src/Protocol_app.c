#include "Protocol_app.h"
#include "bsp_pid.h"
#include "bsp_hard.h"



AppDecodeDef	decodS1;					// 解码进程用到的数据
AppFrameDef		ReptFrame;					// 主动上报的一帧数据



INT16U CrcCalc_Crc16Modbus(INT8U *dat,INT8U len)
{
	INT16U	CRC_index = 0xffff;
	INT16U	buffer;
	volatile	INT8U	i = 0, j = 0;
	for(i = 0; i < len; i++){
		buffer = dat[i];				
		CRC_index ^= buffer;
		for(j = 0; j < 8; j++){
			if(CRC_index & 0x0001){
				CRC_index >>= 1;
				CRC_index ^= 0xa001;
			}else{
				CRC_index >>= 1;
			}
		}
	}
	return CRC_index;
}



void APP_SendFrame(AppFrameDef *Frame)
{
	INT16U	crc;
	crc = CrcCalc_Crc16Modbus((INT8U *)Frame, Frame->pack.len+1);

	if(Frame->pack.sFlag & FALG_UART1){
		UART1_SendByte(FRAME_HEAD1);
		UART1_SendByte(FRAME_HEAD2);
		UART1_SendBytes(Frame->pack.len+1, (INT8U *)Frame);
		UART1_SendByte(crc>>8);
		UART1_SendByte(crc);
	}
}



/************
 * 功能： 主动上报一帧数据给APP
 * 参数： func  	 -->  功能码
 * 		 len   		-->  （参数列表）数据帧长度
 *       parap[]    -->  （参数列表）把要传递的数据通过数组形式传递
 ***/
void APP_Report(INT8U func, INT8U len, INT8U para[])
{
	INT8U	i;
	
	ReptFrame.list.len   = len + 5;
	ReptFrame.list.idH   = DEVICE_ID_H;
	ReptFrame.list.idL   = DEVICE_ID_L;
	ReptFrame.list.type  = FRAME_TYPE_POST;
	ReptFrame.list.state = FRAME_STATE_ACK;
	ReptFrame.list.func  = func;
	for(i=0;i<len;i++)	ReptFrame.list.para[i] = para[i];
	ReptFrame.list.sFlag =  FALG_UART1;		// 上报给UART1
	
	APP_SendFrame(&ReptFrame);
}
/*****************************************************************************
* 功能:		回应一帧数据给APP
*****************************************************************************/
void APP_RespFrame(AppFrameDef *Frame, INT8U State)
{
	Frame->list.state   = State;
	APP_SendFrame(Frame);
}


/*****************************************************************************
* 功能:		APP端协议命令解码
*****************************************************************************/
void APP_DecodeCmd(AppFrameDef *Frame)
{
	/*定义一个判断命令有效的标志，如果成功进入指令就让valid置1，否则就不变*/
	INT8U	vaildCmd= 0;

	/*校验设备地址*/
	if((Frame->list.idH != DEVICE_ID_H) || (Frame->list.idL != DEVICE_ID_L))	return;	
	/*选择判断传入帧的类型*/
	switch(Frame->list.type)
	{


		/*------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
		/*                                                                           发送数据指令 0x31                                                                                        */
		/*------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
		case FRAME_TYPE_ACT:
		{
			switch(Frame->list.func)// 功能码
			{


			/*****************************************************************************************************/
			/*设置设备启动/停机*/
			/*****************************************************************************************************/
			case FRAME_FUNC_RUN:
			{
				vaildCmd = 1;
				Flag.WorkStart = Frame->list.para[0];

				if (Flag.WorkStart == 1)
				{
					/* Enable cooling: init PID, start cooling subsystem */
					PID_Init();
					Cooling_Init();
					PID_SetEnabled(1);
				}
				else
				{
					/* Disable cooling */
					PID_SetEnabled(0);
					Cooling_Off();
				}
			}
			break;

			/*****************************************************************************************************/
			/*进入老化模式*/
			/*****************************************************************************************************/
			case SET_OLD_MODE:
			{
				if ((Flag.PowerOn == 1) && (Flag.WorkStart == 1))
				{
					vaildCmd = 1;
					Flag.OldFlag = 1;
				}
			}break;


			/*****************************************************************************************************/
			/*进入升级模式*/
			/*****************************************************************************************************/
			case FRAME_FUNC_UPGRADE:
			{
				vaildCmd = 1;
				Flag.update = 1;
				if ((Flag.PowerOn == 1) && (Flag.WorkStart == 1))
				{
					vaildCmd = 1;
					Flag.update = 1;
				}

			}break;


				default:									
					break;		
			}
		}
		break;


		/*------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
		/*                                                                           发送数据指令 0x32                                                                                        */
		/*------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
		case FRAME_TYPE_SET:
		{
			switch(Frame->list.func)
			{	
				case SET_COOLING_LEVEL:
				{
					vaildCmd = 1;
					/* para[0]: cooling level 1~5 maps to temperature range */
					/* Level 1 = 20C, Level 2 = 17C, Level 3 = 13C, Level 4 = 8C, Level 5 = 5C */
					static const int16_t level_to_temp[] = {200, 170, 130, 80, 50};
					uint8_t level = Frame->list.para[0];
					if (level >= 1 && level <= 5)
					{
						PID_SetTarget(level_to_temp[level - 1]);
					}
				}break;

	
				default:
						break;
			}

		}		
	

		/*------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
		/*                                                                           接受到读取数据指令 0x33                                                                                  */
		/*------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
		case FRAME_TYPE_GET:
		{
			switch(Frame->list.func)
			{
				
				case	FRAME_FUNC_VERSION:
				{
					vaildCmd = 1;
					Frame->list.len+=2;
					Frame->list.para[0] = MAJ_VERSION;
					Frame->list.para[1] = SUB_VERSION;
				}break;

			case 0x41:
				{
					vaildCmd = 1;
					Frame->list.len += 2;
					Frame->list.para[0] = (INT8U)(g_cooling_pid.current_temp >> 8);
					Frame->list.para[1] = (INT8U)(g_cooling_pid.current_temp & 0xFF);
				}break;
		
				
				default:
					break;
			}
		}
		

		default:	break;
	}



	if(vaildCmd)
	{
		BEEP_ShortPress();
		LED_Blink(LED_Name_MessageSuccessful,1,0,0);
		APP_RespFrame(Frame,FRAME_STATE_ACK);		// 正常的回应
	}
}

/*****************************************************************************
* 功能:		APP端协议帧解码
* 			数据帧格式：
* 			          { 1     2     3      4      5       6       7    ...  len+1}-------|
* 			                                                                             |
* 			            |---{ 1     2      3      4       5       6    ...   len }      / \
* 			            |                                                              /   \
* 			0x55 0xAA [len] [idH] [idL] [type] [state] [func ] [para1] ... [paraN] [crcH] [crcL]
* 			                   \   /      |       |       |       \__________/
* 			                    \ /       |       |     功能码      参数列表       
* 			                     |        |       |
* 			                     |        |       |    /--- 0x00:(从机回)出错
* 			                     |        |       |---<---- 0x01:(从机回)成功
* 			                     |        |            \--- 0x02:(主机发)命令/数据
* 			                     |        |
* 			                     |        |            /--- 0x31:发命令
* 			                     |        |-----------<---- 0x32:写参数
* 			                     |                     \--- 0x33:读参数
* 			                     |
* 			                     |                     /--- :
* 			                     |--------------------<---- 0x0022:DIL01-1
* 			                                           \--- 0x0004:K1
*****************************************************************************/
void APP_DecodeFrame(INT8U dat, INT8U sFlag, AppDecodeDef *decod)
{
	INT16U	crcCalc;
	
	decod->Frame.pack.sFlag = sFlag;
	if(decod->tovFlag)	decod->step=0;				// 超时判断（舍弃很久之前的不完整帧数据）
	
	switch(decod->step)
	{
	case 0:											// 帧头
		if(dat == FRAME_HEAD1)	{
			decod->tovFlag = 0;	decod->tovCnt = 0;	// 计时器清零（开始计时）
			decod->step++;
		}
		break;
	case 1:											// 帧头
		if(dat == FRAME_HEAD2)	decod->step++;
		else			decod->step=0;
		break;
	case 2:											// 长度域
		decod->Frame.pack.len = dat;
		decod->datCnt = 0;
		decod->step++;
		break;
	case 3:											// 数据域
		decod->Frame.pack.dat[decod->datCnt++] = dat;
		if(decod->datCnt == decod->Frame.pack.len)	decod->step++;
		break;
	case 4:											// 校验码
		decod->crc = dat;
		decod->crc <<= 8;
		decod->step++;
		break;
	case 5:											// 校验码
		decod->crc += dat;
		crcCalc = CrcCalc_Crc16Modbus((INT8U *)&decod->Frame, decod->Frame.pack.len+1);
		#if 1
		if(decod->crc == crcCalc)	{				// 校验成功，则继续解码
			APP_DecodeCmd(&decod->Frame);
		}else{										// 校验失败，则回应出错帧
			APP_RespFrame(&decod->Frame,FRAME_STATE_ERR);
		}
		#else
		APP_DecodeCmd(&decod->Frame);
		#endif
	default:
		decod->step=0;
		break;
	}
}

/*****************************************************************************
* 功能:		超时扫描（1ms扫描周期）
*****************************************************************************/
void APP_TimeOverScan(void)
{
	if(++decodS1.tovCnt >= TIME_OVER_TIME)
	{
		decodS1.tovFlag = 1;
	}
}

/*****************************************************************************
* 功能:		串口数据帧解码
* 说明:		UART1通常连接蓝牙模块（进而连接手机），用于与手机端APP通信。
*****************************************************************************/
void UART1_RecvDecode(INT8U dat)
{
	if (Flag.PowerOn == 1)
	{
		if (Bluetooth_GetConnect())
		{
			APP_DecodeFrame(dat, FALG_UART1, &decodS1);
		}
		else
		{
			/*一旦断开连接，需要复位解码进程*/
			decodS1.step = 0;
		}
	}

}





