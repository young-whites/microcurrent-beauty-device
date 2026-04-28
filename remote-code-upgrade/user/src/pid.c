#include "pid.h"

// PID  pid_temp;


// void PID_Temp_Init(void)  //
// {
// 	pid_temp.set =1347;//42度

// 	pid_temp.currpwm=0;
// 	pid_temp.pwm_cycle=50;    //

// 	pid_temp.En=0;
// 	pid_temp.En_1=0;
// 	pid_temp.En_2=0;

// 	pid_temp.Kp=0.05;
// 	pid_temp.Ki=0.001;//0.05;//0.1;
// 	pid_temp.Kd=0.05;//75;
// }     

// void Pid_Temp_Calc(void)  //pid??
// {
//     static float outp,outi,outd;


// 	pid_temp.En=pid_temp.curr-pid_temp.set;  //本次误差     为正 说明小于设定值，  为负数 说明大于设定值 
// 	pid_temp.Sum_En+=pid_temp.En;   //本次偏差与上次偏差之差，为正 说明在远离设定值，为负数 说明在靠近设定值
	
// 	outp=pid_temp.Kp*pid_temp.En;
// 	outi=pid_temp.Ki*pid_temp.Sum_En;
// 	outd=pid_temp.Kd*(pid_temp.En_1-pid_temp.En_2);

// 	pid_temp.currpwm=(int)(outp+outi+outd); //本次应该输出的增量
// 	if(pid_temp.currpwm>pid_temp.pwm_cycle)
// 	{
// 	  pid_temp.currpwm=pid_temp.pwm_cycle;
// 	}
// 	if(pid_temp.currpwm<0)
// 	{
// 	 pid_temp.currpwm=0;
// 	}
// 	//if(pid_temp.En)
//  	pid_temp.En_2=pid_temp.En_1;
// 	pid_temp.En_1=pid_temp.En;
// }