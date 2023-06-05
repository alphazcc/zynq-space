/**
 * Copyright (c) 2022-2023，HelloAlpha
 * 
 * Change Logs:
 * Date           Author       Notes
 */
#include "app_timer.h"

#define USING_ULOG
#include "ulog.h"

extern XScuGic IntcInstPtr;

/**
 * @brief 定时器中断处理，中断时间 50ms
 */
void TimerIntrHandler(void *CallBackRef)
{
    XScuTimer *TimerInstancePtr = (XScuTimer *) CallBackRef;
    struct timer_flag *_timer_flag = &g_timer_flag;
    static uint16_t count = 0;
    
    /* 标志位触发 */
//  XScuTimer_IsExpired(TimerInstancePtr);
    ULOG("--- timer handle --- \r\n");

    count++;

    if(count == 101)
    {
        count = 0;
    }

    ULOG("count: %d\r\n", count);

    _timer_flag->timer_flag_50ms = 1;

    if(count % 10 == 0)
    {
        _timer_flag->timer_flag_500ms = 1;
        ULOG("--- 500 MS --- \r\n");

    }
    if(count % 20 == 0)
    {
        _timer_flag->timer_flag_1s = 1;
        ULOG("---  1 S --- \r\n");
    }

    XScuTimer_ClearInterruptStatus(TimerInstancePtr);
    /* 关闭自动重装载值 关闭中断 */
    // XScuTimer_DisableAutoReload(TimerInstancePtr);
}

int app_timer_init(void)
{
    int Status;

    Status = TimerInit(&Timer, TIMER_DEVICE_ID, TIMER_LOAD_VALUE);
    if (Status != XST_SUCCESS) {
        return XST_FAILURE;
    }
    Status = TimerIntrInit(&IntcInstPtr, &Timer, TIMER_IRPT_INTR, TimerIntrHandler);
    if (Status != XST_SUCCESS) {
        return XST_FAILURE;
    }
    XScuTimer_Start(&Timer);

    return Status;
}
