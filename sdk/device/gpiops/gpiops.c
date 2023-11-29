/**
 * Copyright (c) 2022-2023，HelloAlpha
 * 
 * Change Logs:
 * Date           Author       Notes
 */
#include "gpiops.h"

uint32_t GpioPsRead(XGpioPs *GpioInstancePtr, uint32_t pin)
{
    XGpioPs_SetDirectionPin(GpioInstancePtr, pin, GPIO_MODE_INPUT);
    return XGpioPs_ReadPin(GpioInstancePtr, pin);
}

int GpioPsWrite(XGpioPs *GpioInstancePtr, uint32_t pin, uint32_t status)
{
    XGpioPs_SetDirectionPin(GpioInstancePtr, pin, GPIO_MODE_OUTPUT);
    XGpioPs_SetOutputEnablePin(GpioInstancePtr, pin, GPIO_OUTPUT_ENABLE);
    if(status & 0x01)
    {
        XGpioPs_WritePin(GpioInstancePtr, pin, GPIO_SET);
    }
    else
    {
        XGpioPs_WritePin(GpioInstancePtr, pin, GPIO_RESET);
    }

    return 0;
}

int GpioPsInit(XGpioPs *GpioInstancePtr, char GpioDeviceId)
{
    int Status;
    XGpioPs_Config *ConfigPtr;

    ConfigPtr = XGpioPs_LookupConfig(GpioDeviceId);
    if (NULL == ConfigPtr) {
        return XST_FAILURE;
    }
    Status = XGpioPs_CfgInitialize(GpioInstancePtr, ConfigPtr,
                    ConfigPtr->BaseAddr);
    if (Status != XST_SUCCESS) {
        return XST_FAILURE;
    }

    Status = XGpioPs_SelfTest(GpioInstancePtr);
    if (Status != XST_SUCCESS) {
        return XST_FAILURE;
    }

    return Status;
}

int GpioPsIntrInitBank(XScuGic *IntcInstancePtr, XGpioPs *GpioInstancePtr,
        char GpioIntrId, uint8_t Bank, uint32_t IntrType, uint32_t IntrPolarity,
        uint32_t IntrOnAny, void(* CallBack)(void *, uint32_t, uint32_t), uint32_t Mask)
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
                (Xil_ExceptionHandler)XScuGic_InterruptHandler,
                IntcInstancePtr);

    Status = XScuGic_Connect(IntcInstancePtr, GpioIntrId,
                (Xil_ExceptionHandler)XGpioPs_IntrHandler,
                (void *)GpioInstancePtr);
    if (Status != XST_SUCCESS) {
        return Status;
    }

    XGpioPs_SetIntrType(GpioInstancePtr, Bank, IntrType, IntrPolarity, IntrOnAny);

    XGpioPs_SetCallbackHandler(GpioInstancePtr, (void *)GpioInstancePtr, CallBack);

    XGpioPs_IntrEnable(GpioInstancePtr, Bank, Mask);

    XScuGic_Enable(IntcInstancePtr, GpioIntrId);

    Xil_ExceptionEnable();

    return XST_SUCCESS;
}

int GpioPsIntrInitPin(XScuGic *IntcInstancePtr, XGpioPs *GpioInstancePtr, 
        u16 GpioIntrId, void(* CallBack)(void *), uint32_t Pin, uint32_t IntrType)
{
    int status;
    XScuGic_Config *IntcConfig;

    IntcConfig = XScuGic_LookupConfig(INTC_DEVICE_ID);
    if (NULL == IntcConfig) {
        return XST_FAILURE;
    }

    status = XScuGic_CfgInitialize(IntcInstancePtr, IntcConfig,
                    IntcConfig->CpuBaseAddress);
    if (status != XST_SUCCESS) {
        return XST_FAILURE;
    }

    Xil_ExceptionInit();

    Xil_ExceptionRegisterHandler(XIL_EXCEPTION_ID_INT,
                (Xil_ExceptionHandler) XScuGic_InterruptHandler, 
                IntcInstancePtr);

    status = XScuGic_Connect(IntcInstancePtr, GpioIntrId,
                (Xil_ExceptionHandler) CallBack, 
                (void *) GpioInstancePtr);
    if (status != XST_SUCCESS) {
        return status;
    }

    XGpioPs_SetIntrTypePin(GpioInstancePtr, Pin, IntrType);

    XGpioPs_IntrEnablePin(GpioInstancePtr, Pin);

    XScuGic_Enable(IntcInstancePtr, GpioIntrId);
    
    Xil_ExceptionEnable();

    return XST_SUCCESS;
}
