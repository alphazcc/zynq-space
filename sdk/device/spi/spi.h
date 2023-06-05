/**
 * Copyright (c) 2022-2023，HelloAlpha
 * 
 * Change Logs:
 * Date           Author       Notes
 */
#ifndef __SPIPS_H__
#define __SPIPS_H__

#include "xspips.h"         /* SPI device driver */
#include "xscugic.h"

#ifndef INTC_DEVICE_ID
#define INTC_DEVICE_ID      XPAR_SCUGIC_SINGLE_DEVICE_ID
#endif

#define SPI_DEVICE_ID       XPAR_XSPIPS_0_DEVICE_ID

#define MAX_DATA            100
#define SpiPs_RecvByte(BaseAddress) \
            (uint8_t)XSpiPs_In32((BaseAddress) + XSPIPS_RXD_OFFSET)
#define SpiPs_SendByte(BaseAddress, Data) \
            XSpiPs_Out32((BaseAddress) + XSPIPS_TXD_OFFSET, (Data))

void SpiRead(XSpiPs *SpiInstance, uint8_t *ReadBuffer, uint8_t ByteCount);
void SpiWrite(XSpiPs *SpiInstance, uint8_t *SendBuffer, uint8_t ByteCount);
int SpiInit(XSpiPs *SpiInstance, uint8_t SpiDeviceId);

#endif
