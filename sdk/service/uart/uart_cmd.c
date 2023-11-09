/**
 * Copyright (c) 2022-2023，HelloAlpha
 * 
 * Change Logs:
 * Date           Author       Notes
 */
#include "app_uart.h"

#define USING_ULOG
#include "ulog.h"

static struct uart_msg *_uart0_msg = &g_uart0_msg;

int uart_msg_print(void)
{
    static int count;

    if(_uart0_msg->ReceivedFlag)
    {
        _uart0_msg->ReceivedFlag = 0;
        count++;
        kprintf("UART0 count: %d\r\n", count);
        _uart0_msg->ReceivedBufferPtr = _uart0_msg->ReceivedBuffer;
        kprintf("UART0 MSG: %s\r\n", (char *)_uart0_msg->ReceivedBufferPtr);
    }

    return 0;
}

/**
 * @brief Serial port loopback test
 */
int uart_lookback_test(void)
{
    if (_uart0_msg->ReceivedFlag)
    {
        _uart0_msg->ReceivedBufferPtr = _uart0_msg->ReceivedBuffer ;
        _uart0_msg->SendBufferPtr = _uart0_msg->ReceivedBuffer ;
        _uart0_msg->SendByteNum = _uart0_msg->ReceivedByteNum ;
        _uart0_msg->ReceivedByteNum = 0 ;
        _uart0_msg->ReceivedFlag = 0 ;
        UartPsSend(&Uart0_Ps, _uart0_msg->SendBufferPtr, _uart0_msg->SendByteNum);
    }

    return 0;
}