/**
 * @file    debug_uart.c
 * @brief   Debug UART0 driver for PID cooling temperature data output
 * @note    UART0 TX only on P44, 9600 8N1, blocking transmit (no FIFO/interrupt)
 */
#include "debug_uart.h"
#include "cms32f033x.h"
#include "sys.h"
#include "Psfr_uart.h"
#include "Psfr_system.h"
#include "Psfr_gpio.h"

/**
 * @brief   Initialize UART0 for debug output
 * @note    Configures P44 as TXD0, enables UART0 clock, sets 9600 8N1
 */
void Debug_UART_Init(void)
{
    /* Enable UART0 peripheral clock */
    SYS_EnablePeripheralClk(SYS_CLK_UART0_MSK);

    /* Configure P44 as TXD0 (unlock GPIO4, set mux, lock GPIO4) */
    SYS_EnableGPIO4Protect();
    SYS_SET_IOCFG(IOP44CFG, SYS_IOCFG_P44_TXD0);
    SYS_DisableGPIO4Protect();
    GPIO_CONFIG_IO_MODE(GPIO4, GPIO_PIN_4, GPIO_MODE_OUTPUT_PUSH_PULL);

    /* Configure UART0: 9600 baud, 8-bit, no parity, 1 stop bit */
    UART_ConfigRunMode(UART0, 9600, UART_WLS_8, UART_PARITY_NONE, UART_STOP_BIT_1);
}

/**
 * @brief   Send a single byte via UART0 (blocking)
 * @param   c   Byte to send
 */
void Debug_UART_SendByte(uint8_t c)
{
    while (!UART_TxdIsEmpty(UART0))
        ;
    UART_Send(UART0, c);
}

/**
 * @brief   Send a null-terminated string via UART0
 * @param   s   Pointer to string
 */
void Debug_UART_SendString(const char* s)
{
    while (*s) {
        Debug_UART_SendByte((uint8_t)*s);
        s++;
    }
}

/**
 * @brief   Send a signed integer as ASCII string via UART0
 * @param   v   Signed integer value
 */
void Debug_UART_SendValue(int32_t v)
{
    uint8_t buf[12];
    uint8_t i = 0;
    uint32_t uv;

    if (v < 0) {
        Debug_UART_SendByte('-');
        uv = (uint32_t)(0 - v);
    } else {
        uv = (uint32_t)v;
    }

    /* Convert digits in reverse order */
    if (uv == 0) {
        Debug_UART_SendByte('0');
        return;
    }
    while (uv) {
        buf[i++] = (uint8_t)(uv % 10) + '0';
        uv /= 10;
    }
    /* Send in correct order */
    while (i) {
        Debug_UART_SendByte(buf[--i]);
    }
}

/**
 * @brief   Print formatted temperature data for PID debug
 * @param   current_temp    Current temperature in 0.1*C units
 * @param   target_temp     Target temperature in 0.1*C units
 * @param   pid_output      PID output value (0-100)
 * @note    Output format: "Cur:105 Tar:100 Out:35\r\n"
 */
void Debug_Print_TempData(int16_t current_temp, int16_t target_temp, uint8_t pid_output)
{
    Debug_UART_SendString("Cur:");
    Debug_UART_SendValue((int32_t)current_temp);
    Debug_UART_SendString(" Tar:");
    Debug_UART_SendValue((int32_t)target_temp);
    Debug_UART_SendString(" Out:");
    Debug_UART_SendValue((int32_t)pid_output);
    Debug_UART_SendString("\r\n");
}
