/**
 * Copyright (c) 2022-2023，HelloAlpha
 * 
 * Change Logs:
 * Date           Author       Notes
 */
#include "xadc.h"

#ifdef USING_XADC
#include "stdio.h"
#define kprintf printf

/**
 * xil_printf does not support printing floating point numbers
*/

static XAdcPs Xadc;
static XadcData_t xadc_data;

int xadc_read_data(void)
{
    XadcPs_GetData(&Xadc, &xadc_data);

    kprintf("Temperature:           %f C    \r\n", xadc_data.temp);
    kprintf("PL kernel voltage:     %f V    \r\n", xadc_data.vccint);
    kprintf("PL auxiliary voltage:  %f V    \r\n", xadc_data.vccaux);
    kprintf("PL BRAM voltage:       %f V    \r\n", xadc_data.vccbram);
    kprintf("PS kernel voltage:     %f V    \r\n", xadc_data.vccpint);
    kprintf("PS auxiliary voltage:  %f V    \r\n", xadc_data.vccpaux);
    kprintf("PS DDR voltage:        %f V    \r\n", xadc_data.vccpdro);

    return 0;
}

int app_xadc_init(void)
{
    int Status = XST_SUCCESS;
    Status = XadcPs_Init(&Xadc, XADC_DEVICE_ID);
    if (Status != XST_SUCCESS) {
        return XST_FAILURE;
    }

    return Status;
}
#endif