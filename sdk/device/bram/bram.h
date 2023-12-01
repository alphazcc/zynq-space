/**
 * Copyright (c) 2022-2023，HelloAlpha
 * 
 * Change Logs:
 * Date           Author       Notes
 */
#ifndef __BRAM_H__
#define __BRAM_H__

#include "xbram.h"
#include "pl_bram_rd.h"

#define PL_BRAM_BASE        XPAR_PL_BRAM_RD_0_S00_AXI_BASEADDR  // PL_RAM_RD 基地址
#define PL_BRAM_START       PL_BRAM_RD_S00_AXI_SLV_REG0_OFFSET  // RAM 读开始寄存器地址
#define PL_BRAM_START_ADDR  PL_BRAM_RD_S00_AXI_SLV_REG1_OFFSET  // RAM 起始寄存器地址
#define PL_BRAM_LEN         PL_BRAM_RD_S00_AXI_SLV_REG2_OFFSET  // PL 读 RAM 的深度
#define PL_BRAM_RD_FREQ     PL_BRAM_RD_S00_AXI_SLV_REG3_OFFSET	// PL 读 RAM 的频率

#define BRAM_BASE           XPAR_BRAM_0_BASEADDR
#define BRAM_HIGH           XPAR_BRAM_0_HIGHADDR

#define START_ADDR          0  // RAM 起始地址 范围：0 ~ 4095
#define BRAM_DATA_BYTE      4  // BRAM 数据字节个数

//函数声明
int BramPsWrite_uint32(uint32_t *pdata, uint32_t offset, uint32_t length);
int BramPsRead_uint32(uint32_t *pbuff, uint32_t offset, uint32_t length);
int BramPlReadSet(uint32_t length, uint32_t freq);
int BramPlReadStart(void);
int BramPlReadStop(void);

#endif
