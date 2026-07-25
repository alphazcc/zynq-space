/**
 * Copyright (c) 2022-2026, HelloAlpha
 *
 * Change Logs:
 * Date           Author       Notes
 */
#include "app_uart.h"

#ifdef USING_UART

extern XScuGic IntcInstPtr;

typedef struct uart_msg
{
    uint8_t ReceivedBuffer[UART_MSG_MAX_LEN];
    uint8_t SendBuffer[UART_MSG_MAX_LEN];
    uint8_t *ReceivedBufferPtr;
    uint8_t *SendBufferPtr;
    uint32_t ReceivedByteNum;
    uint32_t SendByteNum;
    char ReceivedFlag;
} uart_msg_t;

#ifdef USING_UART0
static XUartPs Uart0_Ps;
static uart_msg_t g_uart0_msg;
static uart_msg_t *uart0_msg = &g_uart0_msg;
static XUartPsFormat Uart0Format = {UART0_BAUDRATE, 8, 0, 1};
#endif

#ifdef USING_UART1
static XUartPs Uart1_Ps;
static uart_msg_t g_uart1_msg;
static uart_msg_t *uart1_msg = &g_uart1_msg;
static XUartPsFormat Uart1Format = {UART1_BAUDRATE, 8, 0, 1};
#endif

int Uart0_RECV(uint8_t *RECVData, uint32_t DataLength)
{
    if (uart0_msg->ReceivedFlag)
    {
        uart0_msg->ReceivedBufferPtr = uart0_msg->ReceivedBuffer;
        DataLength = uart0_msg->ReceivedByteNum;
        memcpy(RECVData, uart0_msg->ReceivedBufferPtr, DataLength);
        uart0_msg->ReceivedByteNum = 0;
        uart0_msg->ReceivedFlag = 0;
        return DataLength;
    }
    return 0;
}

int Uart1_RECV(uint8_t *RECVData, uint32_t DataLength)
{
    if (uart1_msg->ReceivedFlag)
    {
        uart1_msg->ReceivedBufferPtr = uart1_msg->ReceivedBuffer;
        DataLength = uart1_msg->ReceivedByteNum;
        memcpy(RECVData, uart1_msg->ReceivedBufferPtr, DataLength);
        uart1_msg->ReceivedByteNum = 0;
        uart1_msg->ReceivedFlag = 0;
        return DataLength;
    }
    return 0;
}

int Uart0_Send(uint8_t *SendData, uint32_t DataLength)
{
    UartPsSend(&Uart0_Ps, SendData, DataLength);
    return 0;
}

int Uart1_Send(uint8_t *SendData, uint32_t DataLength)
{
    UartPsSend(&Uart1_Ps, SendData, DataLength);
    return 0;
}

#ifdef USING_UART0
/**
 * @brief Interrupt handling functions
 *
 * @param CallBackRef is a pointer to an upper-level callback reference
 */
static void Uart0Handler(void *CallBackRef)
{
    XUartPs *UartInstancePtr = (XUartPs *)CallBackRef;
    struct uart_msg *uart0_msg = &g_uart0_msg;
    static uint32_t ReceivedCount = 0;
    static uint32_t UartSrValue = 0;

    uart0_msg->ReceivedFlag = 0;

    /* Read interrupt status and enable bits */
    UartSrValue = XUartPs_ReadReg(UartInstancePtr->Config.BaseAddress, XUARTPS_IMR_OFFSET);
    UartSrValue &= XUartPs_ReadReg(UartInstancePtr->Config.BaseAddress, XUARTPS_ISR_OFFSET);

    /* check if receiver FIFO trigger */
    if (UartSrValue & XUARTPS_IXR_RXOVR)
    {
        ReceivedCount = UartPsRev(UartInstancePtr, uart0_msg->ReceivedBufferPtr, UART_MSG_MAX_LEN);
        uart0_msg->ReceivedByteNum += ReceivedCount;
        uart0_msg->ReceivedBufferPtr += ReceivedCount;
    }
    /* check if receiver FIFO empty */
    if (UartSrValue & XUARTPS_IXR_RXEMPTY)
    {
        /* do nothing */
    }
    /* check if it is a timeout interrupt */
    if (UartSrValue & XUARTPS_IXR_TOUT)
    {
        uart0_msg->ReceivedFlag = 1;
    }

    /* clear trigger interrupt */
    XUartPs_WriteReg(UartInstancePtr->Config.BaseAddress, XUARTPS_ISR_OFFSET, UartSrValue);
}
#endif

#ifdef USING_UART1
/**
 * @brief Interrupt handling functions
 *
 * @param CallBackRef is a pointer to an upper-level callback reference
 */
static void Uart1Handler(void *CallBackRef)
{
    XUartPs *UartInstancePtr = (XUartPs *)CallBackRef;
    struct uart_msg *uart1_msg = &g_uart1_msg;
    static uint32_t ReceivedCount = 0;
    static uint32_t UartSrValue = 0;

    uart1_msg->ReceivedFlag = 0;

    /* Read interrupt status and enable bits */
    UartSrValue = XUartPs_ReadReg(UartInstancePtr->Config.BaseAddress, XUARTPS_IMR_OFFSET);
    UartSrValue &= XUartPs_ReadReg(UartInstancePtr->Config.BaseAddress, XUARTPS_ISR_OFFSET);

    /* check if receiver FIFO trigger */
    if (UartSrValue & XUARTPS_IXR_RXOVR)
    {
        ReceivedCount = UartPsRev(UartInstancePtr, uart1_msg->ReceivedBufferPtr, UART_MSG_MAX_LEN);
        uart1_msg->ReceivedByteNum += ReceivedCount;
        uart1_msg->ReceivedBufferPtr += ReceivedCount;
    }
    /*check if receiver FIFO empty */
    if (UartSrValue & XUARTPS_IXR_RXEMPTY)
    {
        /* do nothing */
    }
    /* check if it is a timeout interrupt */
    if (UartSrValue & XUARTPS_IXR_TOUT)
    {
        uart1_msg->ReceivedFlag = 1;
    }

    /* clear trigger interrupt */
    XUartPs_WriteReg(UartInstancePtr->Config.BaseAddress, XUARTPS_ISR_OFFSET, UartSrValue);
}
#endif

int app_uart_init(void)
{
    int Status = XST_SUCCESS;

#ifdef USING_UART0
    /* init UART */
    Status = UartPsInit(&Uart0_Ps, &Uart0Format, UART0_DEVICE_ID);
    if (Status != XST_SUCCESS)
    {
        return XST_FAILURE;
    }
    /* init UART interrupt */
    Status = UartPsIntrInit(&IntcInstPtr, &Uart0_Ps, UART0_IRPT_INTR, Uart0Handler);
    if (Status != XST_SUCCESS)
    {
        return XST_FAILURE;
    }
    /* clear trigger interrupt */
    XUartPs_WriteReg(Uart0_Ps.Config.BaseAddress, XUARTPS_ISR_OFFSET, XUARTPS_IXR_MASK);
#endif

#ifdef USING_UART1
    Status = UartPsInit(&Uart1_Ps, &Uart1Format, UART1_DEVICE_ID);
    if (Status != XST_SUCCESS)
    {
        return XST_FAILURE;
    }
    Status = UartPsIntrInit(&IntcInstPtr, &Uart1_Ps, UART1_IRPT_INTR, Uart1Handler);
    if (Status != XST_SUCCESS)
    {
        return XST_FAILURE;
    }
    XUartPs_WriteReg(Uart1_Ps.Config.BaseAddress, XUARTPS_ISR_OFFSET, XUARTPS_IXR_MASK);
#endif

    return Status;
}

#endif
