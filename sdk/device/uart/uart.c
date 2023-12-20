/**
 * Copyright (c) 2022-2023，HelloAlpha
 * 
 * Change Logs:
 * Date           Author       Notes
 */
#include "uart.h"

#ifdef USING_UART

/**
 * @brief UART initialization
 * 
 * @param UartInstancePtr UART instance
 * @param UartFormat UART communication format
 * @param UartDeviceId UART device ID
 * @return int 
 */
int UartPsInit(XUartPs* UartInstancePtr, XUartPsFormat* UartFormat, uint16_t UartDeviceId)
{
    int Status;
    XUartPs_Config *UartConfigPtr;

    UartConfigPtr = XUartPs_LookupConfig(UartDeviceId);
    if (NULL == UartConfigPtr) {
        return XST_FAILURE;
    }

    Status = XUartPs_CfgInitialize(UartInstancePtr, UartConfigPtr, 
                        UartConfigPtr->BaseAddress);
    if (Status != XST_SUCCESS) {
        return XST_FAILURE;
    }

    Status = XUartPs_SelfTest(UartInstancePtr);
    if (Status != XST_SUCCESS) {
        return XST_FAILURE;
    }

    /* Set UART mode Baud Rate 115200, 8bits, no parity, 1 stop bit */
    XUartPs_SetDataFormat(UartInstancePtr, UartFormat) ;
    /* Set the UART in Normal Mode */
    XUartPs_SetOperMode(UartInstancePtr, XUARTPS_OPER_MODE_NORMAL);

    return Status;
}

/**
 * @brief Initialization of the UART interrupt
 * 
 * @param IntcInstancePtr Interrupt instance
 * @param UartInstancePtr UART interrupt instance
 * @param UartIntrId UART interrupt ID
 * @param CallBack Interrupt service function
 * @return int 
 */
int UartPsIntrInit(XScuGic *IntcInstancePtr, XUartPs *UartInstancePtr,
        uint32_t UartIntrId, void(* CallBack)(void *))
{
    int Status;

    XScuGic_Config *IntcConfig;

    IntcConfig = XScuGic_LookupConfig(INTC_DEVICE_ID);
    if (NULL == IntcConfig) {
        return XST_FAILURE;
    }

    Status = XScuGic_CfgInitialize(IntcInstancePtr, IntcConfig,
            IntcConfig->CpuBaseAddress);
    if (Status != XST_SUCCESS) {
        return XST_FAILURE;
    }

    Xil_ExceptionInit();
    Xil_ExceptionRegisterHandler(XIL_EXCEPTION_ID_INT,
            (Xil_ExceptionHandler) XScuGic_InterruptHandler,
            IntcInstancePtr);
    Xil_ExceptionEnable();

    Status = XScuGic_Connect(IntcInstancePtr, UartIntrId,
            (Xil_ExceptionHandler) CallBack,
            (void *) UartInstancePtr);
    if (Status != XST_SUCCESS) {
        return XST_FAILURE;
    }

    /* Set the receiver timeout.*/
    XUartPs_SetRecvTimeout(UartInstancePtr, 8);

    /*Set receiver FIFO interrupt trigger level, here set to 1*/
    XUartPs_SetFifoThreshold(UartInstancePtr, 1);
    /* Set the interrupt triggering type */
    XUartPs_SetInterruptMask(UartInstancePtr, 
                        XUARTPS_IXR_RXOVR | XUARTPS_IXR_RXEMPTY | XUARTPS_IXR_TOUT);
    XScuGic_Enable(IntcInstancePtr, UartIntrId);

    return Status ;
}

/**
 * @brief UART send function
 * 
 * @param InstancePtr UART instance
 * @param BufferPtr Send buffer pointer
 * @param NumBytes The number of bytes to send
 * @return int The number of bytes successfully sent
 */
int UartPsSend(XUartPs *InstancePtr, uint8_t *BufferPtr, uint32_t NumBytes)
{
    uint32_t SentCount = 0U;

    /* Setup the buffer parameters */
    InstancePtr->SendBuffer.RequestedBytes = NumBytes;
    InstancePtr->SendBuffer.RemainingBytes = NumBytes;
    InstancePtr->SendBuffer.NextBytePtr = BufferPtr;


    while (InstancePtr->SendBuffer.RemainingBytes > SentCount)
    {
        /* Fill the FIFO from the buffer */
        if (!XUartPs_IsTransmitFull(InstancePtr->Config.BaseAddress))
        {
            XUartPs_WriteReg(InstancePtr->Config.BaseAddress,
                    XUARTPS_FIFO_OFFSET,
                    ((uint32_t)InstancePtr->SendBuffer.
                            NextBytePtr[SentCount]));

            /* Increment the send count. */
            SentCount++;
        }
    }

    /* Update the buffer to reflect the bytes that were sent from it */
    InstancePtr->SendBuffer.NextBytePtr += SentCount;
    InstancePtr->SendBuffer.RemainingBytes -= SentCount;

    return SentCount;
}

/**
 * @brief UART receive function
 * 
 * @param InstancePtr UART instance
 * @param BufferPtr Receive buffer pointer
 * @param NumBytes The number of bytes to read
 * @return int The number of bytes successfully read
 */
int UartPsRev(XUartPs *InstancePtr, uint8_t *BufferPtr, uint32_t NumBytes)
{
    uint32_t ReceivedCount = 0;
    uint32_t CsrRegister;

    /* Setup the buffer parameters */
    InstancePtr->ReceiveBuffer.RequestedBytes = NumBytes;
    InstancePtr->ReceiveBuffer.RemainingBytes = NumBytes;
    InstancePtr->ReceiveBuffer.NextBytePtr = BufferPtr;

    /*
     * Read the Channel Status Register to determine if there is any data in
     * the RX FIFO
     */
    CsrRegister = XUartPs_ReadReg(InstancePtr->Config.BaseAddress,
            XUARTPS_SR_OFFSET);

    /*
     * Loop until there is no more data in RX FIFO or the specified
     * number of bytes has been received
     */
    while((ReceivedCount < InstancePtr->ReceiveBuffer.RemainingBytes)&&
            (((CsrRegister & XUARTPS_SR_RXEMPTY) == (u32)0)))
    {
        InstancePtr->ReceiveBuffer.NextBytePtr[ReceivedCount] =
                XUartPs_ReadReg(InstancePtr->Config.BaseAddress, XUARTPS_FIFO_OFFSET);

        ReceivedCount++;

        CsrRegister = XUartPs_ReadReg(InstancePtr->Config.BaseAddress,
                XUARTPS_SR_OFFSET);
    }
    InstancePtr->is_rxbs_error = 0;
    /*
     * Update the receive buffer to reflect the number of bytes just
     * received
     */
    if(NULL != InstancePtr->ReceiveBuffer.NextBytePtr){
        InstancePtr->ReceiveBuffer.NextBytePtr += ReceivedCount;
    }
    InstancePtr->ReceiveBuffer.RemainingBytes -= ReceivedCount;

    return ReceivedCount;
}

/**
 * @brief Set the UART baud rate
 * 
 * @param UartDeviceId UART device ID
 * @param Baudrate Baud rate to set
 * @return int 
 */
int UartPsSetBaudRate(uint16_t UartDeviceId , uint32_t Baudrate)
{
    static XUartPs Uart;
    XUartPs_Config *UartConfigPtr;
    UartConfigPtr = XUartPs_LookupConfig(UartDeviceId);
    if (NULL == UartConfigPtr) {
        return XST_FAILURE;
    }
    XUartPs_CfgInitialize(&Uart, UartConfigPtr, UartConfigPtr->BaseAddress);
    XUartPs_SetBaudRate(&Uart, Baudrate);
    return XST_FAILURE;
}
#endif
