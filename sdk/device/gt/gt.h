/**
 * Copyright (c) 2022-2023，HelloAlpha
 * 
 * Change Logs:
 * Date           Author       Notes
 */
#ifndef __GT_H__
#define __GT_H__

#include "xtime_l.h"
#include "xscugic.h"
#include "xil_io.h"

/* 定时器寄存器 */
#define GT_BASEADDR     GLOBAL_TMR_BASEADDR
#define GT_CNT_REG0     GT_BASEADDR + GTIMER_COUNTER_LOWER_OFFSET
#define GT_CNT_REG1     GT_BASEADDR + GTIMER_COUNTER_UPPER_OFFSET
#define GT_CTRL_REG     GT_BASEADDR + GTIMER_CONTROL_OFFSET
/* 中断寄存器 */
#define GT_INTR         XPAR_GLOBAL_TMR_INTR
/* 中断状态寄存器 */
#define GT_INTR_STAT_REG    GT_BASEADDR + 0x0CU
/* 比较器 */
#define COMP_VAL_REG0       GT_BASEADDR + 0x10U
#define COMP_VAL_REG1       GT_BASEADDR + 0x14U
/* 自动递增寄存器 */
#define AUTO_INC_REG        GT_BASEADDR + 0x18U

#define AUTO_INC_BIT        0x08
#define IRQ_ENABLE_BIT      0x04
#define COMP_ENABLE_BIT     0x02
#define TMR_ENABLE_BIT      0x01

#define CLK_3x2x    333333333

#define GT_WR_REG   Xil_Out32

void GtStart(void);
int GtIntrInit(XScuGic *GtInstancePtr, 
                uint64_t Value, void(* CallBack)(void *));
void gt_tic(void);
double gt_toc(void);
float get_time_s(void);

#endif
