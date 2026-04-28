#include "oldTest.h"



void Old_Test(void)
{
	if (Flag.OldFlag == 1)
	{
		Record.OldStartCnt++;
		/* 第一个两分钟启动运行EMS模式 */
		if (Record.OldStartCnt < 120)
		{
			if (Record.OldStartCnt == 1)
			{
				BEEP_Blink(1, 0, 0);



			}
		}
	}
}
























