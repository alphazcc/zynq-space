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

    if(XGpioPs_IntrGetStatusPin(GpioInstancePtr, KEY4_PIN))
    {
        xil_printf("KEY4 Press! \r\n");
        intr_flag = 1;
        XGpioPs_IntrDisablePin(GpioInstancePtr, KEY4_PIN);
    }
}
#endif

void gpiops_function_always(void)
{
    static int count = 0;
    static int led_status = 0;
    static int intr_led_status = 0;

    if(intr_flag)
    {
        usleep(20000);
        if(gpiops_read(KEY4_PIN) == 0)
        {
            intr_led_status = ~intr_led_status;
            gpiops_write(LED4_PIN, intr_led_status);
        }
        intr_flag = 0;

#ifdef USING_BANK_INTR
        XGpioPs_IntrClear(&GpioPs, GPIOPS_BANK, (1 << 4));
        XGpioPs_IntrEnable(&GpioPs, GPIOPS_BANK, (1 << 4));
#endif

#ifdef USING_PIN_INTR
        XGpioPs_IntrClearPin(&GpioPs, KEY4_PIN);
        XGpioPs_IntrEnablePin(&GpioPs, KEY4_PIN);
#endif
    }

    usleep(20000);
    if(count++ == 10)
    {
        count = 0;
        led_status = ~led_status;
        gpiops_write(LED2_PIN, led_status);
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
             KEY4_PIN, XGPIOPS_IRQ_TYPE_EDGE_FALLING);
#endif

    return status;
}
