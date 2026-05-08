#include "ccp.h"




/*****************************************************************************
* 功能:		ccp配置为PWM模式
*****************************************************************************/
void CCP_Config_PWM_Mode(void)
{
	// (1)设置CCP的时钟
	SYS_EnablePeripheralClk(SYS_CLK_CCP_MSK);							// 开启CCP模块的时钟
	CCP_ConfigCLK(CCP0, CCP_CLK_DIV_1, CCP_RELOAD_CCPLOAD, 15000);		// Pclk=3MHz, CCP0clk = Pclk/1/3000 = 1000Hz(5ms)
	CCP_ConfigCLK(CCP1, CCP_CLK_DIV_1, CCP_RELOAD_CCPLOAD, 3000);		// Pclk=3MHz, CCP1clk = Pclk/1/3000 = 1000Hz(1ms)

	// (2)设置CCP运行模式
	CCP_EnablePWMMode(CCP0);											// 设置CCP0为PWM模式
	CCP_EnablePWMMode(CCP1);											// 设置CCP1为PWM模式

	// (3)设置CCP 占空比
	CCP_ConfigDutyScale(CCP0, CCPxA, 0); 								// 0%
	CCP_ConfigDutyScale(CCP0, CCPxB, 0); 								// 0%
	CCP_ConfigDutyScale(CCP1, CCPxA, 0); 								// 0%
	CCP_ConfigDutyScale(CCP1, CCPxB, 0); 								// 0%

	// (4)设置CCP 反相输出
	CCP_DisableReverseOutput(CCP0, CCPxA);								// 关闭反相输出
	CCP_DisableReverseOutput(CCP0, CCPxB);
	CCP_DisableReverseOutput(CCP1, CCPxA);
	CCP_DisableReverseOutput(CCP1, CCPxB);

	// (5)设置CCP 中断`
//	CCP_EnableOverflowInt(CCP0);									// 开启CCP0溢出中断
//	CCP_EnableOverflowInt(CCP1);									// 开启CCP1溢出中断
//	CCP_EnableCompareInt(CCP0);										// 开启CCP0比较中断
//	CCP_EnableCompareInt(CCP1);										// 开启CCP1比较中断
	NVIC_EnableIRQ(CCP_IRQn);
	NVIC_SetPriority(CCP_IRQn, 0);									// 优先级0~3， 0最高、3最低

	// (7)开启CCP模块
	CCP_EnableRun(CCP0);
	CCP_EnableRun(CCP1);
	CCP_Stop(CCP0);
	CCP_Stop(CCP1);
}


