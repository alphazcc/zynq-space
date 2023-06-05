/**
 * Copyright (c) 2022-2023，HelloAlpha
 * 
 * Change Logs:
 * Date           Author       Notes
 */
#include "spi.h"

int SpiInit(XSpiPs *SpiInstance, uint8_t SpiDeviceId)
{
    int Status;
    XSpiPs_Config *SpiConfig;

    /*
     * Initialize the SPI driver so that it's ready to use
     */
    SpiConfig = XSpiPs_LookupConfig(SpiDeviceId);
    if (NULL == SpiConfig) {
        return XST_FAILURE;
    }

    Status = XSpiPs_CfgInitialize((SpiInstance), SpiConfig,
                    SpiConfig->BaseAddress);
    if (Status != XST_SUCCESS) {
        return XST_FAILURE;
    }

    /*
     * Perform a self-test to check hardware build.
     */
    Status = XSpiPs_SelfTest(SpiInstance);
    if (Status != XST_SUCCESS) {
        return XST_FAILURE;
    }

    /*
     * The SPI device is a slave by default and the clock phase
     * have to be set according to its master. In this example, CPOL is set
     * to quiescent high and CPHA is set to 1.
     */
    Status = XSpiPs_SetOptions((SpiInstance),  XSPIPS_MASTER_OPTION | XSPIPS_CR_CPHA_MASK | XSPIPS_FORCE_SSELECT_OPTION);
    if (Status != XST_SUCCESS) {
        return XST_FAILURE;
    }

    Status = XSpiPs_SetClkPrescaler(SpiInstance, XSPIPS_CLK_PRESCALE_64);
    if (Status != XST_SUCCESS) {
        return XST_FAILURE;
    }

    /*
     * Enable the device.
     */
    XSpiPs_Enable(SpiInstance);

    return XST_SUCCESS;
}

void SpiRead(XSpiPs *SpiInstance, uint8_t *ReadBuffer, uint8_t ByteCount)
{
    int Count;
    uint32_t StatusReg;

    do{
        StatusReg = XSpiPs_ReadReg(SpiInstance->Config.BaseAddress,
                    XSPIPS_SR_OFFSET);
    } while(!(StatusReg & XSPIPS_IXR_RXNEMPTY_MASK));

    /*
     * Reading the Rx Buffer
     */
    for(Count = 0; Count < ByteCount; Count++) {
        ReadBuffer[Count] = SpiPs_RecvByte(
                SpiInstance->Config.BaseAddress);
    }
}

void SpiWrite(XSpiPs *SpiInstance, uint8_t *SendBuffer, uint8_t ByteCount)
{
    uint32_t StatusReg;
    int TransCount = 0;

    /*
     * Fill the TXFIFO with as many bytes as it will take (or as
     * many as we have to send).
     */
    while ((ByteCount > 0) &&
        (TransCount < XSPIPS_FIFO_DEPTH)) {
        SpiPs_SendByte(SpiInstance->Config.BaseAddress,
                *SendBuffer);
        SendBuffer++;
        ++TransCount;
        ByteCount--;
    }

    /*
     * Wait for the transfer to finish by polling Tx fifo status.
     */
    do {
        StatusReg = XSpiPs_ReadReg(
                SpiInstance->Config.BaseAddress,
                    XSPIPS_SR_OFFSET);
    } while ((StatusReg & XSPIPS_IXR_TXOW_MASK) == 0);
}
