/**
 * Copyright (c) 2022-2023，HelloAlpha
 *
 * Change Logs:
 * Date           Author       Notes
 */
#ifndef __IIC_IO_H__
#define __IIC_IO_H__

#include "xgpiops.h"

#define SCL_PIN           54
#define SDA_PIN           55

#define GPIO_MODEL_INPUT        0
#define GPIO_MODEL_OUTPUT       1

#define GPIO_OUTPUT_DISABLE     0
#define GPIO_OUTPUT_ENABLE      1

#define GPIO_RESET              0
#define GPIO_SET                1

void SCL_OUT(void);
void SCL_HIGH(void);
void SCL_LOW(void);

void SDA_IN(void);
uint32_t SDA_READ(void);

void SDA_OUT(void);
void SDA_HIGH(void);
void SDA_LOW(void);

int IIC_Init(void);

#endif
