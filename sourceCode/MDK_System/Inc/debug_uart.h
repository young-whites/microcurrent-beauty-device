/**
 * @file    debug_uart.h
 * @brief   Debug UART0 driver for PID cooling temperature data output
 * @note    UART0 TX only (P44), 115200 8N1, blocking transmit
 */
#ifndef __DEBUG_UART_H
#define __DEBUG_UART_H

#include "sys.h"

void    Debug_UART_Init(void);
void    Debug_UART_SendByte(uint8_t c);
void    Debug_UART_SendString(const char* s);
void    Debug_UART_SendValue(int32_t v);
void    Debug_Print_TempData(int16_t current_temp, int16_t target_temp, uint8_t pid_output);

#endif /* __DEBUG_UART_H */
