/**
 * Copyright (c) 2022-2023，HelloAlpha
 * 
 * Change Logs:
 * Date           Author       Notes
 */
#include "iic_io.h"

extern XGpioPs GpioPs;

void SCL_OUT(void)
{
    XGpioPs_SetDirectionPin(&GpioPs, SCL_PIN, GPIO_MODE_OUTPUT);
    XGpioPs_SetOutputEnablePin(&GpioPs, SCL_PIN, GPIO_OUTPUT_ENABLE);
}

void SCL_HIGH(void)
{
    XGpioPs_WritePin(&GpioPs, SCL_PIN, GPIO_SET);
}

void SCL_LOW(void)
{
    XGpioPs_WritePin(&GpioPs, SCL_PIN, GPIO_RESET);
}

void SDA_IN(void)
{
    XGpioPs_SetDirectionPin(&GpioPs, SDA_PIN, GPIO_MODE_INPUT);
}

uint32_t SDA_READ(void)
{
    return XGpioPs_ReadPin(&GpioPs, SDA_PIN);
}

void SDA_OUT(void)
{
    XGpioPs_SetDirectionPin(&GpioPs, SDA_PIN, GPIO_MODE_OUTPUT);
    XGpioPs_SetOutputEnablePin(&GpioPs, SDA_PIN, GPIO_OUTPUT_ENABLE);
}

void SDA_HIGH(void)
{
    XGpioPs_WritePin(&GpioPs, SDA_PIN, GPIO_SET);
}

void SDA_LOW(void)
{
    XGpioPs_WritePin(&GpioPs, SDA_PIN, GPIO_RESET);
}

int IIC_Init(void)
{
    SCL_OUT();
    SCL_HIGH();
    SDA_OUT();
    SDA_HIGH();

    return 0;
}
