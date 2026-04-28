#ifndef __PID_H_
#define __PID_H_

typedef struct
{
   short curr;  //当前温度
	 short set;   //设定温度
   
	
	 short En;
	 short En_1;
	 short En_2;
	 int Sum_En;
	
	 float  Kp;   //比例系数
	 float  Ki;   //比例系数
	 float  Kd;   //比例系数
	
	//float Tsam; //采样周期---控制周期，每隔Tsam控制器输出一次PID运算结果
	//float Ti;//积分时间常数
	//float Td;//微分时间常数
	
	 short Dout;//增量PID计算本次应该输出的增量值--本次计算的结果
	
	 int currpwm;  //当前的pwm宽度
	 short pwm_cycle;//pwm的周期
}PID;

extern PID  pid_temp;

void PID_Temp_Init(void);  //
void Pid_Temp_Calc(void);  //pid??

#endif
