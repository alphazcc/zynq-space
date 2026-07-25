/**
 * Copyright (c) 2022-2026, HelloAlpha
 *
 * Change Logs:
 * Date           Author       Notes
 */
#ifndef __APP_UART_H__
#define __APP_UART_H__

#include "uart.h"

#ifdef USING_UART

/* maximum receiver length */
#define UART_MSG_MAX_LEN 100

#ifdef USING_UART0
#define UART0_BAUDRATE 115200U
#endif

#ifdef USING_UART1
#define UART1_BAUDRATE 115200U
#endif

int Uart0_RECV(uint8_t *RECVData, uint32_t DataLength);
int Uart1_RECV(uint8_t *RECVData, uint32_t DataLength);
int Uart0_Send(uint8_t *SendData, uint32_t DataLength);
int Uart1_Send(uint8_t *SendData, uint32_t DataLength);

int app_uart_init(void);

#endif
#endif
