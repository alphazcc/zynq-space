/**
 * Copyright (c) 2022-2023，HelloAlpha
 * 
 * Change Logs:
 * Date           Author       Notes
 */
#include "bram.h"

/* 将字符串写入 BRAM */
int BramPsWrite_uint32(uint32_t *pdata, uint32_t offset, uint32_t length)
{
    uint32_t i = 0, wr_cnt = 0;

    /* 循环向 BRAM 中写入 */
    for(i = BRAM_DATA_BYTE * (START_ADDR + offset) ; i < BRAM_DATA_BYTE * (START_ADDR + offset + length) ;
            i += BRAM_DATA_BYTE) {
        XBram_WriteReg(BRAM_BASE, i, pdata[wr_cnt]) ;
        wr_cnt++;
    }

    return 0;
}

/* 从 BRAM 中读出数据 */
int BramPsRead_uint32(uint32_t *pbuff, uint32_t offset, uint32_t length)
{
	uint32_t rd_cnt = 0, i = 0;

    /* 循环从 BRAM 中读出数据 */
    for(i = BRAM_DATA_BYTE * (START_ADDR + offset); i < BRAM_DATA_BYTE * (START_ADDR + offset + length) ;
            i += BRAM_DATA_BYTE) {
        pbuff[rd_cnt] = XBram_ReadReg(BRAM_BASE, i) ;
        rd_cnt++;
    }

    return 0;
}

int BramPlReadSet(uint32_t length, uint32_t freq)
{
    /* 设置 BRAM 读出的起始地址 */
    PL_BRAM_RD_mWriteReg(PL_BRAM_BASE, PL_BRAM_START_ADDR, BRAM_DATA_BYTE * START_ADDR) ;
    /* 设置 BRAM 读出的字符串长度 */
    PL_BRAM_RD_mWriteReg(PL_BRAM_BASE, PL_BRAM_LEN, BRAM_DATA_BYTE * length) ;
    /* 设置 BRAM 读数据频率 */
    PL_BRAM_RD_mWriteReg(PL_BRAM_BASE, PL_BRAM_RD_FREQ , freq) ;

    return 0;
}

int BramPlReadStart(void)
{
    /* 拉高 BRAM 读开始信号 */
    PL_BRAM_RD_mWriteReg(PL_BRAM_BASE, PL_BRAM_START , 1) ;

    return 0;
}

int BramPlReadStop(void)
{
    /* 拉低 BRAM 读开始信号 */
    PL_BRAM_RD_mWriteReg(PL_BRAM_BASE, PL_BRAM_START , 0) ;

    return 0;
}
