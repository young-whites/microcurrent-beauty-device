#include "sys.h"
#include "bsp_pid.h"
#include "bsp_hard.h"

/**
  * @brief  ϵͳ������ʼ������
  * @param  None
  */
void SystemParaInit(void)
{

}





/**
  * @brief  ϵͳ����״̬���
  * @param  None
  */
void SystemWorkStatusCheck(void)
{

	/***********************************************************/
	/*                 ��Դ����LED״̬ɨ��                     */
	/***********************************************************/
	if (Record.PowerKeyCnt == 0)
	{
		LED_On(LED_Name_PowerKeyOff);
		LED_Off(LED_Name_PowerKeyOn);
		Flag.PowerOn = 0;
		if (Flag.UsartConfig == 0)
		{
			Flag.UsartConfig = 1;
			/* �ر�������Դ */
			Bluetooth_PowerOff();
			/* P34�ܽ����� (MCU_TXD1/BLE_RX) */
			SYS_SET_IOCFG(IOP34CFG, SYS_IOCFG_P34_GPIO);
			GPIO_CONFIG_IO_MODE(GPIO3, GPIO_PIN_4, GPIO_MODE_INPUT_FLOAT);
			/* P35�ܽ����� (MCU_RXD1/BLE_TX) */
			SYS_SET_IOCFG(IOP35CFG, SYS_IOCFG_P35_GPIO);
			GPIO_CONFIG_IO_MODE(GPIO3, GPIO_PIN_5, GPIO_MODE_INPUT_FLOAT);
			/***********************************************************/
			/*                 �ر����й���                            */
			/***********************************************************/
			/* ���е�EPWMʱ�ӹر� */
			/* Stop cooling */
			PID_SetEnabled(0);
			Cooling_Off();
			EPWM_Stop(EPWM_CH_1_MSK | EPWM_CH_2_MSK | EPWM_CH_3_MSK | EPWM_CH_4_MSK);
			/* �ر�CCP0 �� CCP1  */
			CCP_Stop(CCP0);
			CCP_Stop(CCP1);
		}
	}
	else if (Record.PowerKeyCnt == 1)
	{
		LED_Off(LED_Name_PowerKeyOff);
		LED_On(LED_Name_PowerKeyOn);
		Flag.PowerOn = 1;
		if (Flag.UsartConfig == 1)
		{
			Flag.UsartConfig = 0;
			/* ��������������־ */
			btPowerOnCnt = 0;
			/* ����������Դ */
			Bluetooth_PowerOn();
			/* ���������ô���1���Ÿ��� */
			SYS_SET_IOCFG(IOP34CFG, SYS_IOCFG_P34_TXD1);
			SYS_SET_IOCFG(IOP35CFG, SYS_IOCFG_P35_RXD1);
			/***********************************************************/
			/*                 �������ܲ�����Ӧ��ʼ��                  */
			/***********************************************************/

		}
	}
	else if (Record.PowerKeyCnt == 2)
	{
		LED_On(LED_Name_PowerKeyOff);
		LED_Off(LED_Name_PowerKeyOn);
		Flag.PowerOn = 0;
		Record.PowerKeyCnt = 0;

		if (Flag.UsartConfig == 0)
		{
			Flag.UsartConfig = 1;
			/* �ر�������Դ */
			Bluetooth_PowerOff();
			/* P34�ܽ����� (MCU_TXD1/BLE_RX) */
			SYS_SET_IOCFG(IOP34CFG, SYS_IOCFG_P34_GPIO);
			GPIO_CONFIG_IO_MODE(GPIO3, GPIO_PIN_4, GPIO_MODE_INPUT_FLOAT);
			/* P35�ܽ����� (MCU_RXD1/BLE_TX) */
			SYS_SET_IOCFG(IOP35CFG, SYS_IOCFG_P35_GPIO);
			GPIO_CONFIG_IO_MODE(GPIO3, GPIO_PIN_5, GPIO_MODE_INPUT_FLOAT);

			/***********************************************************/
			/*                 �ر����й���                            */
			/***********************************************************/
			/* ���е�EPWMʱ�ӹر� */
			/* Stop cooling */
			PID_SetEnabled(0);
			Cooling_Off();
			EPWM_Stop(EPWM_CH_1_MSK | EPWM_CH_2_MSK | EPWM_CH_3_MSK | EPWM_CH_4_MSK);
			/* �ر�CCP0 �� CCP1  */
			CCP_Stop(CCP0);
			CCP_Stop(CCP1);
		}
	}
}




























