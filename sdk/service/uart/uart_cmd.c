/**
 * Copyright (c) 2022-2023，HelloAlpha
 * 
 * Change Logs:
 * Date           Author       Notes
 */
#include "app_uart.h"

#ifdef USING_UART

#include <string.h>
#include "xil_printf.h"
#define kprintf xil_printf

#ifdef USING_UART0
static struct uart_msg *_uart0_msg = &g_uart0_msg;
#endif

#ifdef USING_UART1
static struct uart_msg *_uart1_msg = &g_uart1_msg;
#endif

int uart_msg_print(void)
{
#ifdef USING_UART0
    static int uart0_count = 0;
    if(_uart0_msg->ReceivedFlag)
    {
        uart0_count++;
        kprintf("UART0 count: %d\r\n", uart0_count);
        _uart0_msg->ReceivedBufferPtr = _uart0_msg->ReceivedBuffer;
        kprintf("UART0 MSG: %s\r\n", (char *)_uart0_msg->ReceivedBufferPtr);
        memset(_uart0_msg->ReceivedBufferPtr, 0, _uart0_msg->ReceivedByteNum);
        _uart0_msg->ReceivedByteNum = 0;
        _uart0_msg->ReceivedFlag = 0;
    }
#endif

#ifdef USING_UART1
    static int uart1_count = 0;
    if(_uart1_msg->ReceivedFlag)
    {
        uart1_count++;
        kprintf("UART1 count: %d\r\n", uart1_count);
        _uart1_msg->ReceivedBufferPtr = _uart1_msg->ReceivedBuffer;
        kprintf("UART1 MSG: %s\r\n", (char *)_uart1_msg->ReceivedBufferPtr);
        memset(_uart1_msg->ReceivedBufferPtr, 0, _uart1_msg->ReceivedByteNum);
        _uart1_msg->ReceivedByteNum = 0;
        _uart1_msg->ReceivedFlag = 0;
    }
#endif
    return 0;
}

/**
 * @brief Serial port loopback test
 */
int uart_lookback_test(void)
{
#ifdef USING_UART0
    if (_uart0_msg->ReceivedFlag)
    {
        _uart0_msg->ReceivedBufferPtr = _uart0_msg->ReceivedBuffer ;
        _uart0_msg->SendBufferPtr = _uart0_msg->ReceivedBuffer ;
        _uart0_msg->SendByteNum = _uart0_msg->ReceivedByteNum ;
        _uart0_msg->ReceivedByteNum = 0 ;
        _uart0_msg->ReceivedFlag = 0 ;
        UartPsSend(&Uart0_Ps, _uart0_msg->SendBufferPtr, _uart0_msg->SendByteNum);
    }
#endif

#ifdef USING_UART1
    if (_uart1_msg->ReceivedFlag)
    {
        _uart1_msg->ReceivedBufferPtr = _uart1_msg->ReceivedBuffer ;
        _uart1_msg->SendBufferPtr = _uart1_msg->ReceivedBuffer ;
        _uart1_msg->SendByteNum = _uart1_msg->ReceivedByteNum ;
        _uart1_msg->ReceivedByteNum = 0 ;
        _uart1_msg->ReceivedFlag = 0 ;
        UartPsSend(&Uart1_Ps, _uart1_msg->SendBufferPtr, _uart1_msg->SendByteNum);
    }
#endif
    return 0;
}

#endif