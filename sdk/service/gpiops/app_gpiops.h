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

enum EMIO_PIN
{
    LED1_PIN = 54,
    LED2_PIN,
    LED3_PIN,
    LED4_PIN,
    KEY1_PIN,
    KEY2_PIN,
    KEY3_PIN,
    KEY4_PIN,
};

XGpioPs GpioPs;

uint32_t gpiops_read(uint32_t pin);
int gpiops_write(uint32_t pin, uint32_t status);
void gpiops_function_always(void);
int app_gpiops_init(void);

#endif
