/**
 * Copyright (c) 2022-2023，HelloAlpha
 * 
 * Change Logs:
 * Date           Author       Notes
 */
#ifndef __UART_H__
#define __UART_H__

#include "xuartps.h"
#include "xscugic.h"

#ifndef INTC_DEVICE_ID
#define INTC_DEVICE_ID      XPAR_SCUGIC_SINGLE_DEVICE_ID
#endif

#define UART0_DEVICE_ID     XPAR_XUARTPS_0_DEVICE_ID
#define UART1_DEVICE_ID     XPAR_XUARTPS_1_DEVICE_ID
#define UART0_IRPT_INTR     XPAR_XUARTPS_0_INTR
#define UART1_IRPT_INTR     XPAR_XUARTPS_1_INTR

/*
 * Function declaration
 */
int UartInit(XUartPs* UartInstancePtr, XUartPsFormat* UartFormat, uint16_t UartDeviceId);
int UartIntrInit(XScuGic *IntcInstancePtr,	XUartPs *UartInstancePtr, 
        uint32_t UartIntrId, void(* CallBack)(void *));
int UartPsSend(XUartPs *InstancePtr, uint8_t *BufferPtr, uint32_t NumBytes) ;
int UartPsRev (XUartPs *InstancePtr, uint8_t *BufferPtr, uint32_t NumBytes) ;
int UartSetBaudRate(uint16_t UartDeviceId , uint32_t Baudrate);

#endif
