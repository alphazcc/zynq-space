/**
 * Copyright (c) 2022-2023，HelloAlpha
 * 
 * Change Logs:
 * Date           Author       Notes
 */
#include "iic.h"

int IicWriteReg8(XIicPs *InstancePtr, uint16_t IIC_ADDR, uint8_t Addr, uint8_t Data)
{
    int Status;
    uint8_t SendBuffer[2];

    SendBuffer[0] = Addr;
    SendBuffer[1] = Data;
    Status = XIicPs_MasterSendPolled(InstancePtr, SendBuffer, 2, IIC_ADDR);
    while (XIicPs_BusIsBusy(InstancePtr));

    return Status;
}

uint8_t IicReadReg8(XIicPs *InstancePtr, uint16_t IIC_ADDR, uint8_t Addr)
{
    uint8_t wr_data, rd_data;

    wr_data = Addr;
    XIicPs_MasterSendPolled(InstancePtr, &wr_data, 1, IIC_ADDR);
    XIicPs_MasterRecvPolled(InstancePtr, &rd_data, 1, IIC_ADDR);
    while (XIicPs_BusIsBusy(InstancePtr));
    return rd_data;
}

int IicWriteReg16(XIicPs *InstancePtr, uint16_t IIC_ADDR, uint16_t Addr, uint8_t Data)
{
    int Status;
    uint8_t SendBuffer[3];

    SendBuffer[0] = Addr>>8;
    SendBuffer[1] = Addr;
    SendBuffer[2] = Data;
    Status = XIicPs_MasterSendPolled(InstancePtr, SendBuffer, 3, IIC_ADDR);
    while (XIicPs_BusIsBusy(InstancePtr));

    return Status;
}

uint8_t IicReadReg16(XIicPs *InstancePtr, uint16_t IIC_ADDR, uint16_t Addr)
{
    uint8_t rd_data;
    uint8_t SendBuffer[2];

    SendBuffer[0] = Addr>>8;
    SendBuffer[1] = Addr;
    XIicPs_MasterSendPolled(InstancePtr, SendBuffer, 2, IIC_ADDR);
    XIicPs_MasterRecvPolled(InstancePtr, &rd_data, 1, IIC_ADDR);
    while (XIicPs_BusIsBusy(InstancePtr));
    return rd_data;
}

int IicInit(XIicPs *InstancePtr, uint16_t DeviceID, uint32_t IIC_SCLK_RATE)
{
    XIicPs_Config *Config;
    int Status;

    Config = XIicPs_LookupConfig(DeviceID);
    if (NULL == Config) {
        return XST_FAILURE;
    }

    Status = XIicPs_CfgInitialize(InstancePtr, Config, Config->BaseAddress);
    if (Status != XST_SUCCESS) {
        return XST_FAILURE;
    }

    Status = XIicPs_SelfTest(InstancePtr);
    if (Status != XST_SUCCESS) {
        return XST_FAILURE;
    }

    XIicPs_SetSClk(InstancePtr, IIC_SCLK_RATE);
    if (Status != XST_SUCCESS) {
        return XST_FAILURE;
    }
    
    while (XIicPs_BusIsBusy(InstancePtr));
    return XST_SUCCESS;
}
