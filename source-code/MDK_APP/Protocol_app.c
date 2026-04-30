#include "Protocol_app.h"



extern void EMS_GPIO_Config(void);
extern void EMS_Wave_1(void);
extern void EMS_Wave_1_Start(void);
extern void EMS_Wave_1_Stop(void);


AppDecodeDef	decodS0,decodS1;					// 解码进程用到的数据
AppFrameDef		ReptFrame;							// 主动上报的一帧数据



/*****************************************************************************
* 功能:		CRC校验码计算（采用Crc16Modbus标准多项式）
* 说明:		校验步骤：
			1、 设置 CRC 寄存器， 并给其赋值 FFFF(hex)
			2、 将数据的第一个 8-bit 字符与 16 位 CRC 寄存器的低 8 位进行异或， 并把结果 存入 CRC 寄存器
			3、 CRC 寄存器向右移一位， MSB 补零， 移出并检查 LSB
			4、 如果 LSB 为 0， 重复第三步； 若 LSB 为 1， CRC 寄存器与多项式码相异或
			5、 重复第 3 与第 4 步直到 8 次移位全部完成。 此时一个 8-bit 数据处理完毕
			6、 重复第 2 至第 5 步直到所有数据全部处理完成
			7、 最终 CRC 寄存器的内容即为 CRC 值
			8、 CRC(16 位)多项式为 X16+X15+X2+1， 其对应校验二进制位列为 1 1000 0000 0000 0101
*****************************************************************************/
INT16U CrcCalc_Crc16Modbus(INT8U *dat,INT8U len)
{
	INT16U	CRC_index = 0xffff;
	INT16U	buffer;
	volatile	INT8U	i = 0, j = 0;
	for(i = 0; i < len; i++){
		buffer = dat[i];							// 把数据取出来放在缓冲区
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



/*****************************************************************************
* 功能:		发送一帧数据给APP
*****************************************************************************/
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

			/*****************************************************************************************************/
			/*选择波形模式*/
			/*****************************************************************************************************/
			case SET_HAND_EMS_WAVE_MODEL:
			{
				vaildCmd = 1;
				Record.HandWave = Frame->list.para[0];


				 if (Record.HandWave == 1 || Record.HandWave == 2 || Record.HandWave == 3) {
					CCP_Stop(CCP1);
					TMR_Stop(TMR0);
					EPWM_Stop(EPWM_CH_1_MSK | EPWM_CH_2_MSK | EPWM_CH_3_MSK | EPWM_CH_4_MSK | EPWM_CH_5_MSK);
					TICK_Stop();					
					EMS_GPIO_Config();
					EPWM_Config();
				}
				else if (Record.HandWave == 4) {
					CCP_Stop(CCP1);
					TMR_Stop(TMR0);
					EPWM_Stop(EPWM_CH_1_MSK | EPWM_CH_2_MSK | EPWM_CH_3_MSK | EPWM_CH_4_MSK | EPWM_CH_5_MSK);
					TICK_Stop();					
					K1_Current_GPIO_Config();
					EPWM_Config();
				}				
				else if(Record.HandWave == 5 || Record.HandWave == 6){
					CCP_Stop(CCP1);
					TMR_Stop(TMR0);
					EPWM_Stop(EPWM_CH_1_MSK | EPWM_CH_2_MSK | EPWM_CH_3_MSK | EPWM_CH_4_MSK | EPWM_CH_5_MSK);
					TICK_Stop();
					EPT_Current_GPIO_Config();
					EPWM_Config();
				}
				 /*设置水电泳的挡位0~10*/
				 if (Record.HandWave == 1 || Record.HandWave == 2 || Record.HandWave == 3)
				 {
					 Record.EMSIonStrengGears = 0;
					 Refr_EMSIonStreng(Record.EMSIonStrengGears);
				 }
				 else if (Record.HandWave == 4)
				 {
					 Record.K1_CurrentGears = 0;
				 }
				 else if (Record.HandWave == 5)
				 {
					 Record.EPTIonStrengGears = 0;
					 if (Record.Switch == 0) {
						 CCP_ConfigDutyScale(CCP1, CCPxB, Record.EPTIonStrengGears);
						 CCP_Stop(CCP1);
					 }
					 else if (Record.Switch == 1) {
						 CCP_ConfigDutyScale(CCP1, CCPxB, Record.EPTIonStrengGears);
					 }
				 }
				 else if (Record.HandWave == 6)
				 {
					 Record.SNEIonStrengGears = 0;
				 }
			}break;


				default:									
					break;		
			}
		}
		break;


		/*------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
		/*                                                                           发送数据指令 0x32                                                                                         */
		/*------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
		case FRAME_TYPE_SET:
		{
			switch(Frame->list.func)
			{	
				/*****************************************************************************************************/
				/* 进入到K1界面后APP发送此命令给单片机，解析此命令后才允许执行后续动作 */
				/*****************************************************************************************************/
				case SET_HAND_EMS_WORK:
				{
					vaildCmd = 1;
					/* 把上位机传过来的参数数据赋给EMS的工作状态标志 */
					Record.Switch = Frame->list.para[0];

					if ((Record.Switch == 1) && ((Record.HandWave == 1) || (Record.HandWave == 2) || (Record.HandWave == 3)))
					{
						LED_On(LED_Name_EMSLED);
						Record.EMS_HotCnt = 1;
						if (Record.EMSIonStrengGears > 0)
						{
							EPWM_Config();
							/* 设置电流强度 -- 这个位置一开始跟随了开关，会导致开机的时候，且挡位为0时有能量输出 */
							Refr_EMSIonStreng(Record.EMSIonStrengGears);
							CCP_Start(CCP1);
							if ((Record.EMSIonStrengGears > 0)&& (Record.Switch == 1))
							{
								if (Record.HandWave == 1){
									EMS_Wave_1();
								}
								else if (Record.HandWave == 2) {
									EMS_Wave_3();
								}
								else if (Record.HandWave == 3) {
									EMS_Wave_6();
								}
							}
							else
							{
								if (Record.HandWave == 2 || Record.HandWave == 3) {
									EMS_Wave_3_6_Stop();
								}
								else if (Record.HandWave == 1) {
									EMS_Wave_1_Stop();
								}
							}
						}
					}
					else if ((Record.Switch == 1) && (Record.HandWave == 4))
					{
						LED_On(LED_Name_EMSLED);	
						Record.EMS_HotCnt = 1;

						if (Record.K1_CurrentGears > 0)
						{
							EPWM_Config();
							#if K1_TIMER0
							cntIndexForWaterArr_Down = 0;
							cntIndexForWaterArr_Up = 0;
							#endif 
							/*开半波*/
							TMR_Start(TMR0);
							/* 开与门 */
							SN74HC21D_OpenB();
							/* 使能A半桥 */
							Flag.cntFlagB = 1;
							EPWM_Start(EPWM_CH_2_MSK);
							EPWM_Stop(EPWM_CH_3_MSK);
							/* CCP模块开启 */
							CCP_Start(CCP1);
							Flag.K1_Systick_Start = 1;
						}
						else
						{
							/*关闭波形*/
							TMR_Stop(TMR0);
							SN74HC21D_StopAll();
							EPWM_Stop(EPWM_CH_2_MSK);
							EPWM_Stop(EPWM_CH_3_MSK);
							CCP_Stop(CCP1);
						}
					}
					// EPT Wave
					else if((Record.Switch == 1) && (Record.HandWave == 5))
					{
						LED_On(LED_Name_EMSLED);
						if((Record.EPTIonStrengGears > 0) && (Record.Switch == 1)){
							// 先对PWM输出引脚进行初始化操作
							EPT_Current_GPIO_Config();
						    /*配置 P07 VCC_ION 引脚为CPP模块的PWM输出模式 -- 这个引脚是用来控制电压大小的*/
							SYS_SET_IOCFG(IOP07CFG, SYS_IOCFG_P07_CCP1B);
							CCP_Config_PWM_Mode();
							// 设置波形的电压范围
							CCP_ConfigDutyScale(CCP1, CCPxB, Record.EPTIonStrengGears);
							CCP_Start(CCP1);
							TICK_Start();
						}
					}
					// SNE Wave
					else if((Record.Switch == 1) && (Record.HandWave == 6))
					{
						LED_On(LED_Name_EMSLED);
						if((Record.SNEIonStrengGears > 0) && (Record.Switch == 1)){
							Record.SNEIonStrengGears = Record.AllIonStrengGears;
							// 先对PWM输出引脚进行初始化操作
							SNE_Current_GPIO_Config();
						    /*配置 P07 VCC_ION 引脚为CPP模块的PWM输出模式 -- 这个引脚是用来控制电压大小的*/
							SYS_SET_IOCFG(IOP07CFG, SYS_IOCFG_P07_CCP1B);
							CCP_Config_PWM_Mode();
							CCP_Start(CCP1);
							// 启动波形
							TICK_Start();
						}
					}
					else if(Record.Switch == 0){
						CCP_Stop(CCP1);
						TMR_Stop(TMR0);
						EPWM_Stop(EPWM_CH_1_MSK | EPWM_CH_2_MSK | EPWM_CH_3_MSK | EPWM_CH_4_MSK | EPWM_CH_5_MSK);
						TICK_Stop();			
					}
				}break;


				/*****************************************************************************************************/
				/*                             挡位调节                                           		              */
				/*****************************************************************************************************/
				case SET_HAND_EMS_Level:
				{
					Record.AllIonStrengGears = Frame->list.para[0];
					if (Record.AllIonStrengGears == 0) {
						CCP_Stop(CCP1);
					}
					else {
						CCP_Start(CCP1);
					}
					vaildCmd = 1;

					//---------------------------------------------------------------------------------------------------------------
					/*设置水电泳的挡位0~10*/
					if (Record.HandWave == 1 || Record.HandWave == 2 || Record.HandWave == 3) 
					{
						Record.EMSIonStrengGears = Record.AllIonStrengGears;
						if ((Record.EMSIonStrengGears > 0) && (Record.EMSIonStrengGears <= 10)) {
							Record.EMSIonStrengGears = 1;
							Refr_EMSIonStreng(Record.EMSIonStrengGears);
						}
						else if ((Record.EMSIonStrengGears > 10) && (Record.EMSIonStrengGears <= 20)) {
							Record.EMSIonStrengGears = 2;
							Refr_EMSIonStreng(Record.EMSIonStrengGears);							
						}
						else if ((Record.EMSIonStrengGears > 20) && (Record.EMSIonStrengGears <= 30)) {
							Record.EMSIonStrengGears = 3;
							Refr_EMSIonStreng(Record.EMSIonStrengGears);
						}
						else if ((Record.EMSIonStrengGears > 30) && (Record.EMSIonStrengGears <= 40)) {
							Record.EMSIonStrengGears = 4;
							Refr_EMSIonStreng(Record.EMSIonStrengGears);
						}
						else if ((Record.EMSIonStrengGears > 40) && (Record.EMSIonStrengGears <= 50)) {
							Record.EMSIonStrengGears = 5;
							Refr_EMSIonStreng(Record.EMSIonStrengGears);
						}
						else if ((Record.EMSIonStrengGears > 50) && (Record.EMSIonStrengGears <= 60)) {
							Record.EMSIonStrengGears = 6;
							Refr_EMSIonStreng(Record.EMSIonStrengGears);
						}
						else if ((Record.EMSIonStrengGears > 60) && (Record.EMSIonStrengGears <= 70)) {
							Record.EMSIonStrengGears = 7;
							Refr_EMSIonStreng(Record.EMSIonStrengGears);
						}
						else if ((Record.EMSIonStrengGears > 70) && (Record.EMSIonStrengGears <= 80)) {
							Record.EMSIonStrengGears = 8;
							Refr_EMSIonStreng(Record.EMSIonStrengGears);
						}
						else if ((Record.EMSIonStrengGears > 80) && (Record.EMSIonStrengGears <= 90)) {
							Record.EMSIonStrengGears = 9;
							Refr_EMSIonStreng(Record.EMSIonStrengGears);
						}
						else if ((Record.EMSIonStrengGears > 90) && (Record.EMSIonStrengGears <= 100)) {
							Record.EMSIonStrengGears = 10;
							Refr_EMSIonStreng(Record.EMSIonStrengGears);
						}
					}					
					else if (Record.HandWave == 4)
					{
						Record.K1_CurrentGears = Record.AllIonStrengGears;
					}
					else if(Record.HandWave == 5)
					{
						Record.EPTIonStrengGears = Record.AllIonStrengGears;
						if(Record.Switch == 0){
							CCP_ConfigDutyScale(CCP1, CCPxB, Record.EPTIonStrengGears);
							CCP_Stop(CCP1);
						}
						else if(Record.Switch == 1){
							CCP_ConfigDutyScale(CCP1, CCPxB, Record.EPTIonStrengGears);
						}
					}
					else if(Record.HandWave == 6)
					{
						Record.SNEIonStrengGears = Record.AllIonStrengGears;
					}
					//---------------------------------------------------------------------------------------------------------------
					if ((Record.Switch == 1) && ((Record.HandWave == 1) || (Record.HandWave == 2) || (Record.HandWave == 3)))
					{
						LED_On(LED_Name_EMSLED);
						Record.EMS_HotCnt = 1;
						if (Record.EMSIonStrengGears > 0)
						{
							EPWM_Config();
							/* 设置电流强度 -- 这个位置一开始跟随了开关，会导致开机的时候，且挡位为0时有能量输出 */
							Refr_EMSIonStreng(Record.EMSIonStrengGears);
							CCP_Start(CCP1);
							if ((Record.EMSIonStrengGears > 0) && (Record.Switch == 1))
							{
								if (Record.HandWave == 1) {
									EMS_Wave_1();
								}
								else if (Record.HandWave == 2) {
									EMS_Wave_3();
								}
								else if (Record.HandWave == 3) {
									EMS_Wave_6();
								}
							}
							else
							{
								if (Record.HandWave == 2 || Record.HandWave == 3) {
									EMS_Wave_3_6_Stop();
								}
								else if (Record.HandWave == 1) {
									EMS_Wave_1_Stop();
								}
							}
						}
					}
					else if ((Record.Switch == 1) && (Record.HandWave == 4))
					{
						LED_On(LED_Name_EMSLED);
						Record.EMS_HotCnt = 1;

						if (Record.K1_CurrentGears > 0)
						{
							EPWM_Config();
#if K1_TIMER0
							cntIndexForWaterArr_Down = 0;
							cntIndexForWaterArr_Up = 0;
#endif 
							/*开半波*/
							TMR_Start(TMR0);
							/* 开与门 */
							SN74HC21D_OpenB();
							/* 使能A半桥 */
							Flag.cntFlagB = 1;
							EPWM_Start(EPWM_CH_2_MSK);
							EPWM_Stop(EPWM_CH_3_MSK);
							/* CCP模块开启 */
							CCP_Start(CCP1);
							Flag.K1_Systick_Start = 1;
						}
						else
						{
							/*关闭波形*/
							TMR_Stop(TMR0);
							SN74HC21D_StopAll();
							EPWM_Stop(EPWM_CH_2_MSK);
							EPWM_Stop(EPWM_CH_3_MSK);
							CCP_Stop(CCP1);
						}
					}
					// EPT Wave
					else if ((Record.Switch == 1) && (Record.HandWave == 5))
					{
						LED_On(LED_Name_EMSLED);
						if ((Record.EPTIonStrengGears > 0) && (Record.Switch == 1)) {
							// 先对PWM输出引脚进行初始化操作
							EPT_Current_GPIO_Config();
							/*配置 P07 VCC_ION 引脚为CPP模块的PWM输出模式 -- 这个引脚是用来控制电压大小的*/
							SYS_SET_IOCFG(IOP07CFG, SYS_IOCFG_P07_CCP1B);
							CCP_Config_PWM_Mode();
							// 设置波形的电压范围
							CCP_ConfigDutyScale(CCP1, CCPxB, Record.EPTIonStrengGears);
							CCP_Start(CCP1);
							TICK_Start();
						}
					}
					// SNE Wave
					else if ((Record.Switch == 1) && (Record.HandWave == 6))
					{
						LED_On(LED_Name_EMSLED);
						if ((Record.SNEIonStrengGears > 0) && (Record.Switch == 1)) {
							Record.SNEIonStrengGears = Record.AllIonStrengGears;
							// 先对PWM输出引脚进行初始化操作
							SNE_Current_GPIO_Config();
							/*配置 P07 VCC_ION 引脚为CPP模块的PWM输出模式 -- 这个引脚是用来控制电压大小的*/
							SYS_SET_IOCFG(IOP07CFG, SYS_IOCFG_P07_CCP1B);
							CCP_Config_PWM_Mode();
							CCP_Start(CCP1);
							// 启动波形
							TICK_Start();
						}
					}



				}break;

				/*****************************************************************************************************/
				/*                                      EMS的加热挡位调节                                            */
				/*****************************************************************************************************/
				case  SET_HAND_EMS_Heat_Level:
				{
					vaildCmd = 1;
					Record.EMS_HotGears = Frame->list.para[0];
					if (Record.EMS_HotGears != 0)
					{
						Record.EMS_HotCnt = 1;
						HeatDissipation_On();  // 开启散热
					}
					else
					{
						Record.EMS_HotCnt = 0;
						HeatDissipation_Off();  // 关闭散热
					}
				}break;
				
				/*****************************************************************************************************/
				/*                             结束                                                                  */
				/*****************************************************************************************************/
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





