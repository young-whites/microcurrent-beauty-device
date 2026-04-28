#include "app_adc1.h"
#include "work.h"

u16 ADC_ConverStable(unsigned char channel)
{
    u8 i;
    u16 adcval;
    u16 minval = 0xFFFF;
    u16 maxval = 0;
    u16 averageval;
    u32 totalval = 0;

    for ( i = 0; i < 18; i++ )
    {
        if(!ADC1_IS_BUSY())
        {	
            ADC1_Go();
            while(ADC1_IS_BUSY());
            if(channel==1)
            {
                adcval = ADC1_GetResult(ADC1_CH_24);		
            }
            else if(channel==2)
            {
                adcval = ADC1_GetResult(ADC1_CH_25);		
            }
            else if(channel==3)
            {
                adcval = ADC1_GetResult(ADC1_CH_19);		
            }
        }

        totalval += adcval;

        if ( minval > adcval )
        {
            minval = adcval;
        }

        if ( maxval < adcval )
        {
            maxval = adcval;
        }
    }

    totalval = totalval - minval - maxval;
    averageval = totalval / 16;
    return averageval;
}

/*-------------------------------------------------------------------------------------------------------------*/
/*        
        Q:过程噪声，Q增大，动态响应变快，收敛稳定性变坏
        R:测量噪声，R增大，动态响应变慢，收敛稳定性变好        
*/
double KalmanFilter1(const double ResrcData,double ProcessNiose_Q,double MeasureNoise_R)//卡尔曼滤波
{
    double R = MeasureNoise_R;
    double Q = ProcessNiose_Q;

    static double x_last;

    double x_mid = x_last;
    double x_now;

    static double p_last;

    double p_mid;
    double p_now;
    double kg;        

    x_mid=x_last; //x_last=x(k-1|k-1),x_mid=x(k|k-1)
    p_mid=p_last+Q; //p_mid=p(k|k-1),p_last=p(k-1|k-1),Q=噪声
    kg=p_mid/(p_mid+R); //kg为kalman filter，R为噪声
    x_now=x_mid+kg*(ResrcData-x_mid);//估计出的最优值
            
    p_now=(1-kg)*p_mid;//最优值对应的covariance        

    p_last = p_now; //更新covariance值
    x_last = x_now; //更新系统状态值

    return x_now;                
}

void ADC_InitConfig(void)
{
	/*
	(1)设置ADC0时钟
	*/
	SYS_EnablePeripheralClk(SYS_CLK_ADC1_MSK);
	//ADC1_ConfigRunMode(ADC1_CONVERT_SINGLE,ADC1_CLK_DIV_64,ADC1_HOLD_10P5_CLK);//Vref=1.2V
	ADC1_ConfigRunMode(ADC1_CONVERT_CONTINUOUS,ADC1_CLK_DIV_8,ADC1_HOLD_6P5_CLK);//ADC1_ConfigRunMode(ADC1_CONVERT_CONTINUOUS,ADC1_CLK_DIV_8,ADC1_HOLD_10P5_CLK);
	
	/*
	(2)设置ADC0通道使能
	*/
	
	ADC1_EnableScanChannel(ADC1_CH_24_MSK);	     /*触发的通道选择AN24 */
	SYS_SET_IOCFG(IOP36CFG,SYS_IOCFG_P36_AN24);  /*关闭P36的数字功能*/
	
	//ADC1_EnableScanChannel(ADC1_CH_10_MSK);
	// ADC1_EnableScanChannel(ADC1_CH_20_MSK);
	// ADC1_EnableScanChannel(ADC1_CH_21_MSK);
	// ADC1_EnableScanChannel(ADC1_CH_22_MSK);
	// ADC1_EnableScanChannel(ADC1_CH_23_MSK);
	// ADC1_EnableScanChannel(ADC1_CH_24_MSK);
	
	/*
	(3)设置ADC0中断
	*/	
	/*
	(4)设置优先级
	*/	
	/*
	(5)开启ADC1
	*/		
	ADC1_Start();
	
	/*
	(6)开启ADC1校准
	*/		
	ADC1_StartAdjust();
	/*
	(7)使用ADC1校准值的ADC转换
	*/	
	ADC1_EnableAdjust();
	#if (adc_IRQ_MODE==1)
	ADC1_EnableChannelInt(ADC1_CH_10_MSK);
	NVIC_ClearPendingIRQ(ADC1_IRQn);
	NVIC_EnableIRQ(ADC1_IRQn);
 #endif
	ADC1_Go();    
}

//uint16_t bsp_Buffer[ADCLEN]  = {0};
//FlagStatus ADC_SmapleFinish=RESET;
//uint8_t  adc_count=0;

//void ADC_Array_get(void)
//{
//   if(ADC_SmapleFinish==RESET)
//   {
//	   bsp_Buffer[adc_count] = ADC1_GetResult(ADC1_CH_10);
//       if(ADC1_GetAdjustErrorFlag())//校准错误
//	   {
//		   ADC1_ClearAdjustErrorFlag();//清除校准错误
//		   return ;
//	   }
//	   if(ADC1_GetConvertErrorFlag())//转换错误
//	   {
//           ADC1_ClearConvertErrorFlag();//清除转换错误
//		   return ;
//	   }	   
//	   adc_count++;
//	   ADC1_Go();
//	   if(adc_count>=ADCLEN)//直到adc数组存满18组数据
//	  {
//		ADC_SmapleFinish = SET;
//		adc_count = 0;  
//	  }
//   }
//}
