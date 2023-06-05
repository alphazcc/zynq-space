/**
 * Copyright (c) 2022-2023，HelloAlpha
 * 
 * Change Logs:
 * Date           Author       Notes
 */
#include "app_uart.h"

#define UART0_BAUDRATE   115200U

extern XScuGic IntcInstPtr;

static XUartPsFormat _Uart0Format = {UART0_BAUDRATE, 8, 0, 1};

/**
 * @brief Interrupt handling functions
 *  
 * @param CallBackRef is a pointer to an upper-level callback reference
 */
static void Uart0Handler(void *CallBackRef)
{
    XUartPs *UartInstancePtr = (XUartPs *) CallBackRef;
    struct uart_msg *_uart0_msg = &g_uart0_msg;
    static uint32_t ReceivedCount = 0;
    static uint32_t UartSrValue = 0;

    _uart0_msg->ReceivedFlag = 0 ;

    /* Read interrupt status and enable bits */
    UartSrValue = XUartPs_ReadReg(UartInstancePtr->Config.BaseAddress, XUARTPS_IMR_OFFSET);
    UartSrValue &= XUartPs_ReadReg(UartInstancePtr->Config.BaseAddress, XUARTPS_ISR_OFFSET);

    /* check if receiver FIFO trigger */
    if (UartSrValue & XUARTPS_IXR_RXOVR)
    {
        ReceivedCount = UartPsRev(UartInstancePtr, _uart0_msg->ReceivedBufferPtr, UART_MSG_MAX_LEN);
        _uart0_msg->ReceivedByteNum += ReceivedCount;
        _uart0_msg->ReceivedBufferPtr += ReceivedCount;
    }
    /* check if receiver FIFO empty */
    if (UartSrValue & XUARTPS_IXR_RXEMPTY)
    {
        /* do nothing */
    }
    /* check if it is a timeout interrupt */
    if (UartSrValue & XUARTPS_IXR_TOUT)
    {
        _uart0_msg->ReceivedFlag = 1;
    }

    /* clear trigger interrupt */
    XUartPs_WriteReg(UartInstancePtr->Config.BaseAddress, XUARTPS_ISR_OFFSET, UartSrValue);
}

int app_uart_init(void)
{
    int Status;
    XUartPs *_Uart0_Ps = &Uart0_Ps;

    /* init UART */
    Status = UartInit(&Uart0_Ps, &_Uart0Format, UART0_DEVICE_ID);
    if (Status != XST_SUCCESS) {
        return XST_FAILURE;
    }

    /* init UART interrupt */
    Status = UartIntrInit(&IntcInstPtr, &Uart0_Ps, UART0_IRPT_INTR, Uart0Handler);
    if (Status != XST_SUCCESS) {
        return XST_FAILURE;
    }

    /* clear trigger interrupt */
    XUartPs_WriteReg(_Uart0_Ps->Config.BaseAddress, XUARTPS_ISR_OFFSET, XUARTPS_IXR_MASK);

    return XST_SUCCESS;
}
