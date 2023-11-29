/**
 * Copyright (c) 2022-2023，HelloAlpha
 * 
 * Change Logs:
 * Date           Author       Notes
 */
#include "app_gpiops.h"
#include "xil_printf.h"

#include "sleep.h"

extern XScuGic IntcInstPtr;

static int intr_flag = 0;

uint32_t gpiops_read(uint32_t pin)
{
    return GpioPsRead(&GpioPs, pin);
}

int gpiops_write(uint32_t pin, uint32_t status)
{
    GpioPsWrite(&GpioPs, pin, status);
    return XST_SUCCESS;
}

#ifdef USING_BANK_INTR
static void gpio_bank_handler(void *CallBackRef, uint32_t Bank, uint32_t Status)
{
    XGpioPs *GpioInstancePtr = (XGpioPs *)CallBackRef;

    static int i = 0;

    if(XGpioPs_IntrGetStatus(GpioInstancePtr, GPIOPS_BANK))
    {
        xil_printf("bank interrupt! count : %d \r\n", i++);
        intr_flag = 1;
        XGpioPs_IntrDisable(GpioInstancePtr, GPIOPS_BANK, (1 << 4));
    }
}
#endif

#ifdef USING_PIN_INTR
static void gpio_pin_handler(void *CallBackRef)
{
    XGpioPs *GpioInstancePtr = (XGpioPs *)CallBackRef;

    static int i = 0;
    xil_printf("interrupt! count : %d \r\n", i++);

    if(XGpioPs_IntrGetStatusPin(GpioInstancePtr, KEY1))
    {
        intr_flag = 1;
        XGpioPs_IntrDisablePin(GpioInstancePtr, KEY1);
    }
}
#endif

void gpiops_function_always(void)
{
    static int led_status = 0;

    if(intr_flag)
    {
        usleep(20000);
        if(gpiops_read(KEY1) == 0)
        {
            led_status = ~led_status;
            gpiops_write(LED1, led_status);
            gpiops_write(LED2, led_status);
            gpiops_write(LED3, led_status);
            gpiops_write(LED4, led_status);
        }
        intr_flag = 0;

#ifdef USING_BANK_INTR
        XGpioPs_IntrClear(&GpioPs, GPIOPS_BANK, (1 << 4));
        XGpioPs_IntrEnable(&GpioPs, GPIOPS_BANK, (1 << 4));
#endif

#ifdef USING_PIN_INTR
        XGpioPs_IntrClearPin(&GpioPs, KEY1);
        XGpioPs_IntrEnablePin(&GpioPs, KEY1);
#endif
    }
}

int app_gpiops_init(void)
{
    int status;

    status = GpioPsInit(&GpioPs, GPIO_DEVICE_ID);
    if (status != XST_SUCCESS) {
        return XST_FAILURE;
    }

#ifdef USING_BANK_INTR
     GpioPsIntrInitBank(&IntcInstPtr, &GpioPs,
                 GPIO_IRPT_INTR, GPIOPS_BANK, XGPIOPS_IRQ_TYPE_EDGE_FALLING, 0x0,
                 0x0, gpio_bank_handler, (1 << 4));
#endif

#ifdef USING_PIN_INTR
     GpioPsIntrInitPin(&IntcInstPtr, &GpioPs, GPIO_IRPT_INTR, gpio_pin_handler,
                 KEY1, XGPIOPS_IRQ_TYPE_EDGE_FALLING);
#endif

    return status;
}
