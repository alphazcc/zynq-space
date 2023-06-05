/**
 * Copyright (c) 2022-2023，HelloAlpha
 *
 * Change Logs:
 * Date           Author       Notes
 */
#ifndef __I2C_CTRL_H__
#define __I2C_CTRL_H__

#include "iic_io.h"

void IIC_Start(void);
void IIC_Stop(void);
uint8_t IIC_Wait_Ack(void);
void IIC_Ack(void);	    
void IIC_NAck(void);
void IIC_Write_Byte(uint8_t value );
uint8_t IIC_Read_Byte(uint8_t addr);
void IIC_Write_UINT16(uint8_t dev_addr, uint8_t wr_addr, uint16_t data);
uint16_t IIC_Read_UINT16(uint8_t dev_addr, uint8_t rd_addr);

#endif
