/**
 * Copyright (c) 2022-2023，HelloAlpha
 * 
 * Change Logs:
 * Date           Author       Notes
 */
#include "spi_io.h"

extern XGpioPs GpioPs;

static void SET_PIN_OUT(uint32_t PIN)
{
    XGpioPs_SetDirectionPin(&GpioPs, PIN, GPIO_MODEL_OUTPUT);
    XGpioPs_SetOutputEnablePin(&GpioPs, PIN, GPIO_OUTPUT_ENABLE);
}

void MOSI_H(void)
{
    SET_PIN_OUT(SPI_MOSI_PIN);
    XGpioPs_WritePin(&GpioPs, SPI_MOSI_PIN, GPIO_SET);
}
 
void MOSI_L(void)
{
    SET_PIN_OUT(SPI_MOSI_PIN);
    XGpioPs_WritePin(&GpioPs, SPI_MOSI_PIN, GPIO_RESET);
}
void SCK_H(void)
{
    SET_PIN_OUT(SPI_SCK_PIN);
    XGpioPs_WritePin(&GpioPs, SPI_SCK_PIN, GPIO_SET);
}
void SCK_L(void)
{
    SET_PIN_OUT(SPI_SCK_PIN);
    XGpioPs_WritePin(&GpioPs, SPI_SCK_PIN, GPIO_RESET);
}
uint32_t MISO(void)
{
    XGpioPs_SetDirectionPin(&GpioPs, SPI_MISO_PIN, GPIO_MODEL_INPUT);
    return XGpioPs_ReadPin(&GpioPs, SPI_MISO_PIN);
}
void NSS_H(void)
{
    SET_PIN_OUT(SPI_NSS_PIN);
    XGpioPs_WritePin(&GpioPs, SPI_NSS_PIN, GPIO_SET);
}
void NSS_L(void)
{
    SET_PIN_OUT(SPI_NSS_PIN);
    XGpioPs_WritePin(&GpioPs, SPI_NSS_PIN, GPIO_RESET);
}
