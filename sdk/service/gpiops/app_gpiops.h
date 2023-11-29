/**
 * Copyright (c) 2022-2023，HelloAlpha
 * 
 * Change Logs:
 * Date           Author       Notes
 */
#ifndef __APP_GPIOPS_H__
#define __APP_GPIOPS_H__

#include "gpiops.h"

#define USING_GPIO_INTR

#ifdef USING_GPIO_INTR
#if 0
#define USING_BANK_INTR
#else
#define USING_PIN_INTR
#endif
#endif

#ifdef USING_BANK_INTR
/* GPIO Bank */
#define GPIOPS_BANK	XGPIOPS_BANK2
#endif

enum UserGpioPs
{
    LED1 = 54,
    LED2,
    LED3,
    LED4,
    KEY1,
    KEY2,
    KEY3,
    KEY4,
};

XGpioPs GpioPs;

uint32_t gpiops_read(uint32_t pin);
int gpiops_write(uint32_t pin, uint32_t status);
void gpiops_function_always(void);
int app_gpiops_init(void);

#endif
