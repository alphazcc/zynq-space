/**
 * Copyright (c) 2022-2023，HelloAlpha
 * 
 * Change Logs:
 * Date           Author       Notes
 */
#include "gt.h"

#include "stdio.h"

void GtStart(void)
{
    /* 启动全局定时器 */
    GT_WR_REG(GT_CTRL_REG, AUTO_INC_BIT | IRQ_ENABLE_BIT 
                | COMP_ENABLE_BIT | TMR_ENABLE_BIT);
}

int GtIntrInit(XScuGic *GtInstancePtr, 
                uint64_t Value, void(* CallBack)(void *))
{
    int Status;

    /* 停止全局定时器 */
    GT_WR_REG(GT_CTRL_REG, 0);
    /* 清空计数器低 32 位 */
    GT_WR_REG(GT_CNT_REG0, 0);
    /* 清空计数器高 32 位 */
    GT_WR_REG(GT_CNT_REG1, 0);
    /* 清除中断标志位 */
    GT_WR_REG(GT_INTR_STAT_REG, 1);
    /* 加载比较器低 32 位 */
    GT_WR_REG(COMP_VAL_REG0, (uint32_t)Value);
    /* 加载比较器高 32 位 */
    GT_WR_REG(COMP_VAL_REG1, 0);
    /* 加载递增寄存器数值 */
    GT_WR_REG(AUTO_INC_REG, (uint32_t)(Value >> 32));
    /* 绑定全局定时器中断服务函数 */
    Status = XScuGic_Connect(GtInstancePtr, GT_INTR,
        (Xil_ExceptionHandler)CallBack, 0);
    if (Status != XST_SUCCESS)
    {
        return Status;
    }
    /* 将 27 号全局定时器中断映射到 CPU1 */
    XScuGic_InterruptMaptoCpu(GtInstancePtr, 1, GT_INTR);
    /* 打开全局定时器中断（27号） */
    XScuGic_Enable(GtInstancePtr, GT_INTR);
    return Status;
}

/* 清零计数器 */
void gt_tic(void)
{
    *((volatile int*)(GT_CTRL_REG)) = 0x00;
    *((volatile int*)(GT_CNT_REG0)) = 0x00000000;
    *((volatile int*)(GT_CNT_REG1)) = 0x00000000;
    *((volatile int*)(GT_CTRL_REG)) = 0x01;
}

/** 读取计数器 输出当前时间 单位：ms
 *  可配合 gt_tic 作如下使用：
 *  {
 *      gt_tic();
 *      function_to_get_running_time();
 *      gt_toc();
 *  }
 */
double gt_toc(void)
{
    *((volatile int*)(GT_CTRL_REG)) = 0x00;
    long long cnt = *((volatile int*)(GT_CNT_REG1));
    double elapsed_time = cnt << 32;
    cnt = *((volatile int*)(GT_CNT_REG0));
    elapsed_time += cnt;
    elapsed_time /= CLK_3x2x;
    elapsed_time *= 1000;
    printf("Elapsed time is %f ms.\r\n",elapsed_time);
    return elapsed_time;
}

/* 获取当前时间（单位秒 second） */
float get_time_s(void)
{
    XTime tCur = 0;
    XTime_GetTime(&tCur);
    return (tCur / (float) COUNTS_PER_SECOND);
}
