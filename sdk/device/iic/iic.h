/**
 * Copyright (c) 2022-2023，HelloAlpha
 * 
 * Change Logs:
 * Date           Author       Notes
 */
#ifndef __IICPS_H__
#define __IICPS_H__

#include "xiicps.h"

#define IIC_DEVICE_ID   XPAR_XIICPS_0_DEVICE_ID

int IicWriteReg8(XIicPs *InstancePtr, uint16_t IIC_ADDR, uint8_t Addr, uint8_t Data);
uint8_t IicReadReg8(XIicPs *InstancePtr, uint16_t IIC_ADDR, uint8_t Addr);
int IicWriteReg16(XIicPs *InstancePtr, uint16_t IIC_ADDR, uint16_t Addr, uint8_t Data);
uint8_t IicReadReg16(XIicPs *InstancePtr, uint16_t IIC_ADDR, uint16_t Addr);
int IicInit(XIicPs *Iic, uint16_t DeviceID, uint32_t IIC_SCLK_RATE);

#endif
