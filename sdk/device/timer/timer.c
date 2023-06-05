/**
 * Copyright (c) 2022-2023，HelloAlpha
 * 
 * Change Logs:
 * Date           Author       Notes
 */
#include "timer.h"

int TimerInit(XScuTimer *TimerInstancePtr, uint16_t TimerDeviceId, 
                            uint32_t TimerLoadValue)
{
    int Status;
    XScuTimer_Config *ConfigPtr;

    ConfigPtr = XScuTimer_LookupConfig(TimerDeviceId);

    Status = XScuTimer_CfgInitialize(TimerInstancePtr, ConfigPtr,
                    ConfigPtr->BaseAddr);
    if (Status != XST_SUCCESS) {
        return XST_FAILURE;
    }

    Status = XScuTimer_SelfTest(TimerInstancePtr);
    if (Status != XST_SUCCESS) {
        return XST_FAILURE;
    }

    XScuTimer_EnableAutoReload(TimerInstancePtr);

    XScuTimer_LoadTimer(TimerInstancePtr, TimerLoadValue);

    return Status;
}

int TimerIntrInit(XScuGic *IntcInstancePtr, XScuTimer *TimerInstancePtr, 
        uint32_t TimerIntrId, void(* CallBack)(void *))
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

    Xil_ExceptionEnable();

    Status = XScuGic_Connect(IntcInstancePtr, TimerIntrId,
          (Xil_ExceptionHandler)CallBack, (void *)TimerInstancePtr);
    if (Status != XST_SUCCESS) {
        return Status;
    }

    XScuGic_Enable(IntcInstancePtr, TimerIntrId);

    XScuTimer_EnableInterrupt(TimerInstancePtr);

    return XST_SUCCESS;
}
