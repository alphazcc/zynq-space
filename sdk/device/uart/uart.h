/**
 * Copyright (c) 2022-2023，HelloAlpha
 * 
 * Change Logs:
 * Date           Author       Notes
 */
#ifndef __UART_H__
#define __UART_H__

/**
 * UART config begin
 * @{
 */
#define USING_UART

#ifdef USING_UART
#define USING_UART0
#define USING_UART1
#endif
/**@}*/

#ifdef USING_UART
#include "xuartps.h"
#include "xscugic.h"

#ifndef INTC_DEVICE_ID
#define INTC_DEVICE_ID      XPAR_SCUGIC_SINGLE_DEVICE_ID
#endif

#ifdef USING_UART0
#define UART0_DEVICE_ID     XPAR_XUARTPS_0_DEVICE_ID
#define UART0_IRPT_INTR     XPAR_XUARTPS_0_INTR
#endif

#ifdef USING_UART1
#define UART1_DEVICE_ID     XPAR_XUARTPS_1_DEVICE_ID
#define UART1_IRPT_INTR     XPAR_XUARTPS_1_INTR
#endif

/* Function declaration */
int UartPsInit(XUartPs* UartInstancePtr, XUartPsFormat* UartFormat, uint16_t UartDeviceId);
int UartPsIntrInit(XScuGic *IntcInstancePtr, XUartPs *UartInstancePtr,
        uint32_t UartIntrId, void(* CallBack)(void *));
int UartPsSend(XUartPs *InstancePtr, uint8_t *BufferPtr, uint32_t NumBytes);
int UartPsRev(XUartPs *InstancePtr, uint8_t *BufferPtr, uint32_t NumBytes);
int UartPsSetBaudRate(uint16_t UartDeviceId , uint32_t Baudrate);

#endif
#endif
