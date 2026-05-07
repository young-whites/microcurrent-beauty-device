#ifndef		PROTOCOL_APP_H
#define		PROTOCOL_APP_H
#include "sys.h"



#define		DEVICE_ID_H					(0x00)		// 设备ID高字节
#define		DEVICE_ID_L					(0xA6)		// 设备ID低字节
//------------------------------------------------------------------------
#define		FRAME_HEAD1					(0x55)		// 帧头1
#define		FRAME_HEAD2					(0xAA)		// 帧头2
#define		FRAME_TYPE_ACT				(0x31)		// 帧类型:动作命令
#define		FRAME_TYPE_SET				(0x32)		// 帧类型:参数设置
#define		FRAME_TYPE_GET				(0x33)		// 帧类型:参数获取
#define		FRAME_TYPE_POST				(0x66)		// 帧类型:主动上报
#define		FRAME_STATE_ASK				(0x02)		// 帧状态:上位请求
#define		FRAME_STATE_ACK				(0x01)		// 帧状态:下位应答
#define		FRAME_STATE_ERR				(0x00)		// 帧状态:校验出错
//------------------------------------------------------------------------
#define		FRAME_FUNC_RUN				(0x01)		// 功能码:运行（启动/停机）
#define		SET_OLD_MODE				(0x05)		// 功能码：进入老化模式
#define		FRAME_FUNC_UPGRADE			(0xAA)		// 功能吗：远程升级

#define		SET_COOLING_LEVEL			(0x0C)		// Cooling target temperature level
//------------------------------------------------------------------------
#define		PARA_SIZE_MAX				(3)			// 最大支持的参数列表长度
#define		TIME_OVER_TIME				(2000)		// 超时时间 2 秒
//------------------------------------------------------------------------
#define		FALG_UART0					(0x01)		// UART0端口标志
#define		FALG_UART1					(0x02)		// UART1端口标志
//------------------------------------------------------------------------
#define		FRAME_FUNC_VERSION			(0x40)
#define		MAJ_VERSION					(2)			// 主版本号 
#define		SUB_VERSION					(1)			// 次版本号 







//----------------------------------------------------------------------------
typedef struct {
	INT8U	len;								// 长度
	INT8U	dat[5 + PARA_SIZE_MAX];				// 数据（最大值待定）
	INT8U	sFlag;								// 串口标志
}AppFramePackDef;


typedef struct {
	INT8U	len;								// 长度
	INT8U	idH;								// id高字节
	INT8U	idL;								// id低字节
	INT8U	type;								// 命令类型
	INT8U	state;								// 命令状态
	INT8U	func;								// 功能码
	INT8U	para[PARA_SIZE_MAX];				// 参数（最大值待定）
	INT8U	sFlag;								// 串口标志
}AppFrameListDef;



typedef union {
	AppFramePackDef	pack;
	AppFrameListDef	list;
}AppFrameDef;
extern AppFrameDef		ReptFrame;				// 主动上报的一帧数据


typedef struct {
	AppFrameDef	Frame;							// 存放收到的一帧数据
	INT16U	crc;								// 校验缓存
	INT8U	step;								// 解码进程
	INT8U	datCnt;								// 数据计数
	INT16U	tovCnt;								// 超时计时
	INT8U	tovFlag;							// 超时标志
}AppDecodeDef;
extern AppDecodeDef	decodS0, decodS1;			// 解码进程用到的数据



void APP_Report(INT8U func, INT8U len, INT8U para[]);
void APP_TimeOverScan(void);

#endif