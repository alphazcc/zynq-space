/**
 * Copyright (c) 2022-2023，HelloAlpha
 * 
 * Change Logs:
 * Date           Author       Notes
 */
#ifndef __GPIOPS_H__
#define __GPIOPS_H__

#include "xgpiops.h"
#include "xscugic.h"

#ifndef INTC_DEVICE_ID
#define INTC_DEVICE_ID      XPAR_SCUGIC_SINGLE_DEVICE_ID
#endif

#define GPIO_DEVICE_ID      XPAR_XGPIOPS_0_DEVICE_ID
#define GPIO_IRPT_INTR      XPAR_XGPIOPS_0_INTR

#define GPIO_MODE_INPUT        	0
#define GPIO_MODE_OUTPUT       	1

#define GPIO_OUTPUT_DISABLE     0
#define GPIO_OUTPUT_ENABLE      1

#define GPIO_RESET              0
#define GPIO_SET                1

uint32_t GpioPsRead(XGpioPs *GpioInstancePtr, uint32_t pin);
int GpioPsWrite(XGpioPs *GpioInstancePtr, uint32_t pin, uint32_t status);
int GpioPsInit(XGpioPs *GpioInstancePtr, char GpioDeviceId);

int GpioPsIntrInitBank(XScuGic *IntcInstancePtr, XGpioPs *GpioInstancePtr,
        char GpioIntrId, uint8_t Bank, uint32_t IntrType, uint32_t IntrPolarity,
        uint32_t IntrOnAny, void(* CallBack)(void *, uint32_t, uint32_t), uint32_t Mask);

int GpioPsIntrInitPin(XScuGic *IntcInstancePtr, XGpioPs *GpioInstancePtr, 
        u16 GpioIntrId, void(* CallBack)(void *), uint32_t Pin, uint32_t IntrType);

#endif
