/**
 * Copyright (c) 2022-2026, HelloAlpha
 *
 * Change Logs:
 * Date           Author       Notes
 */
#include "app_uart.h"

#ifdef USING_UART

#ifdef USING_UART_TEST
#include <stdio.h>
#include <string.h>

#define USING_ULOG
#include "dbg/ulog.h"

#ifdef USING_UART_TEST_SEND
int uart_test_send_per_1s(void)
{
#ifdef USING_UART0
    static int uart0_count = 0;
    char uart0_buff[32];
    uart0_count++;
    sprintf(uart0_buff, "Hello! Uart0 Count: %d \r\n", uart0_count);
    Uart0_Send((uint8_t *)uart0_buff, strlen(uart0_buff));
#endif

#ifdef USING_UART1
    static int uart1_count = 0;
    char uart1_buff[32];
    uart1_count++;
    sprintf(uart1_buff, "Hello! Uart1 Count: %d \r\n", uart1_count);
    Uart1_Send((uint8_t *)uart1_buff, strlen(uart1_buff));
#endif
    return 0;
}
#endif

#ifdef USING_UART_TEST_LOOKBACK
int uart_test_lookback(void)
{
#ifdef USING_UART0
    uint8_t Uart0_RECV_Data[128] = {0};
    uint32_t Uart0_Data_Length = Uart0_RECV(Uart0_RECV_Data, UART_MSG_MAX_LEN);
    if (Uart0_Data_Length > 0)
    {
        kprintf("UART0 RECV: %d \r\n", Uart0_Data_Length);
        kprintf("UART0 MSG : %s\r\n", (char *)Uart0_RECV_Data);
        Uart0_Send(Uart0_RECV_Data, Uart0_Data_Length);
    }
#endif

#ifdef USING_UART1
    uint8_t Uart1_RECV_Data[128] = {0};
    uint32_t Uart1_Data_Length = Uart1_RECV(Uart1_RECV_Data, UART_MSG_MAX_LEN);
    if (Uart1_Data_Length > 0)
    {
        kprintf("UART1 RECV: %d \r\n", Uart0_Data_Length);
        kprintf("UART1 MSG : %s\r\n", (char *)Uart0_RECV_Data);
        Uart1_Send(Uart1_RECV_Data, Uart1_Data_Length);
    }
#endif
    return 0;
}
#endif

#endif
#endif
