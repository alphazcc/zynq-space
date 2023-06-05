/**
 * Copyright (c) 2022-2023，HelloAlpha
 * 
 * Change Logs:
 * Date           Author       Notes
 */
#ifndef __TIMER_H__
#define __TIMER_H__

#include "xscutimer.h"
#include "xscugic.h"

/* CPU时钟频率 */
#ifndef CPU_CLK_FREQ_HZ
#define CPU_CLK_FREQ_HZ     XPAR_CPU_CORTEXA9_0_CPU_CLK_FREQ_HZ
#endif
/* 私有定时器的时钟频率 = CPU时钟频率/2 = 333MHz */
#define TIMER_HZ            CPU_CLK_FREQ_HZ/2

#ifndef INTC_DEVICE_ID
#define INTC_DEVICE_ID      XPAR_SCUGIC_SINGLE_DEVICE_ID
#endif

#define TIMER_DEVICE_ID     XPAR_XSCUTIMER_0_DEVICE_ID
#define TIMER_IRPT_INTR     XPAR_SCUTIMER_INTR

/*
 * Function declaration
 */
int TimerInit(XScuTimer *TimerInstancePtr, uint16_t TimerDeviceId, 
                            uint32_t TimerLoadValue);
int TimerIntrInit(XScuGic *IntcInstancePtr, XScuTimer *TimerInstancePtr, 
        uint32_t TimerIntrId, void(* CallBack)(void *));

#endif
