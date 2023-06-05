/**
 * Copyright (c) 2022-2023，HelloAlpha
 * 
 * Change Logs:
 * Date           Author       Notes
 */
#ifndef __APP_TIMER_H__
#define __APP_TIMER_H__

#include "timer.h"

/* CPU时钟频率 */
#ifndef CPU_CLK_FREQ_HZ
#define CPU_CLK_FREQ_HZ 	XPAR_CPU_CORTEXA9_0_CPU_CLK_FREQ_HZ
#endif

/* 私有定时器的时钟频率 = CPU时钟频率/2 = 333MHz */
#define TIMER_HZ 			CPU_CLK_FREQ_HZ/2

/*
 * 精确计算 装载值 = 定时时间（单位：秒）* 333333333
 * 0.5s 166,666,666.5 0x9EF21AA
 * 0.1s  333,333,33.3 0x1FCA055
 * 0.05s 16,666,666,65 0xFE502A
 */
#define TIMER_LOAD_VALUE 0xFE502A

XScuTimer Timer;

struct timer_flag
{
    char timer_flag_50ms;
    char timer_flag_500ms;
    char timer_flag_1s;
};

typedef struct timer_flag timer_flag_t;

timer_flag_t g_timer_flag;

int app_timer_init(void);

#endif
