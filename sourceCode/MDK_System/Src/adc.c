#include "adc.h"



/*****************************************************************************
* 功能:		ADC配置
* 参数:		chMask:	需要转换的通道掩码 可多选
*****************************************************************************/
void ADC_Config(uint32_t chMask, uint32_t ADC_CHx)
{
    // (1)设置ADC1通道转换初始化
    ADC_Init(chMask);													// ADC初始化

    // (2)设置IN30通道连接的信号
    ADC1_SetAN30Channel(ADC1_CH_30_BG);									// IN30通道连接到 Bandgap 1.2V

    // (3)设置ADC1中断
    // NVIC_EnableIRQ(ADC1_IRQn);										// 开启ADC1中断
    NVIC_SetPriority(ADC1_IRQn, 3);										// 优先级0~3， 0最高、3最低
}




//----------------------------------------------------------------------------
static	uint32_t	_adcResult[ADC1_CN_NUM] = { 0 };	// 各通道转换结果
static	uint32_t	_adcFilter[ADC1_CN_NUM] = { 0 };	// 各通道滤波周期数（默认不滤波）
//----------------------------------------------------------------------------

/*****************************************************************************
* 功能:		ADC读取指定通道的经过滤波后的转换数据
* 参数:		ch:通道号 (0~30)
* 返回:		AD转换值
*****************************************************************************/
uint32_t ADC_Read(uint32_t ch)
{
	return _adcResult[ch];
}

/*****************************************************************************
* 功能:		ADC设置滤波周期数
* 参数:		ch:	通道号 (0~30)
* 			n:	滤波周期的指数部分 (0~4)
* 说明:		滤波周期数m是指，每次采样仅取其采样结果的1/m，这样经过m次累加后，才反映实际结果。
* 			为了简化计算，滤波周期数只选用2^n，其中0<=n<=4，对应m=0/2/4/8/16。
*****************************************************************************/
void ADC_SetFilter(uint32_t ch, uint32_t n)
{
	if (ch >= ADC1_CN_NUM)	return;
	if (n > 4)			return;
	_adcFilter[ch] = n;
}

/*****************************************************************************
* 功能:		ADC扫描
* 说明:		1ms扫描一次（也可放主循环扫描）
*****************************************************************************/
void ADC_DrvScan(void)
{
	INT16U	buff;										// 在12位基础上，16位数据仅支持2^4=16倍放大，需要更大则该变量须改为INT32U
	INT8U	i;
	if (ADC1_IS_IDLE()) {								// ADC转换已经结束
		for (i = 0; i < ADC1_CN_NUM; i++) {				// 逐个通道读取转换结果并滤波
			if (ADC1_GetChannelScanFlag(i)) {			// 仅处理已使能的通道
				buff = _adcResult[i] << _adcFilter[i];	// 放大m倍（得到之前值的m个累加和）
				buff -= _adcResult[i];					// 丢掉其中一个值
				buff += ADC1_GetResult(i);				// 用新采样值替换
				_adcResult[i] = buff >> _adcFilter[i];	// 缩小m倍（m个值求平均，得到新值）
			}
		}
		ADC1_Go();										// 重新启动下一次转换
	}
}

/*****************************************************************************
* 功能:		ADC初始化
* 参数:		chMask:	需要转换的通道掩码 可多选
*****************************************************************************/
void ADC_Init(uint32_t chMask)
{
	INT8U	i;

	// (1)设置ADC1时钟
	SYS_EnablePeripheralClk(SYS_CLK_ADC1_MSK);
	ADC1_ConfigRunMode(ADC1_CONVERT_CONTINUOUS, ADC1_CLK_DIV_1, ADC1_HOLD_6P5_CLK);	// 一次转换完所有使能的通道

	// (2)设置ADC1中断
	ADC1_DisableChannelInt(chMask);						// 关转换中断

	// (3)设置ADC1通道使能
	ADC1_EnableScanChannel(chMask);						// 设置ADC1通道使能
	for (i = 0; i < ADC1_CN_NUM; i++) {
		if (chMask & 0x1) {
			ADC1_ConfigPinAN(i);						// 逐个通道的IO管脚配置为模拟方式
			ADC_SetFilter(i, 3);						// 软件滤波8周期 不用太灵敏
		}
		chMask >>= 1;
	}

	// (4)开启ADC1
	ADC1_Start();

	// (5)开启ADC1校准
	ADC1_StartAdjust();

	// (6)使用ADC1校准值的ADC转换
	ADC1_EnableAdjust();

	// (7)开始转换
	ADC1_Go();
}