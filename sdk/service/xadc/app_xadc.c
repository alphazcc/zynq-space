/**
 * Copyright (c) 2022-2026, HelloAlpha
 *
 * Change Logs:
 * Date           Author       Notes
 */
#include "xadc.h"

#ifdef USING_XADC

/**
 * xil_printf does not support printing floating point numbers
 */
#include "stdio.h"
#define kprintf printf

static XAdcPs Xadc;
static XadcData_t xadc_data;

int xadc_read_data(void)
{
    XadcPs_GetData(&Xadc, &xadc_data);

    kprintf("On Chip Temperature:   %f C    \r\n", xadc_data.temp);
    kprintf("PL Kernel Voltage:     %f V    \r\n", xadc_data.vccint);
    kprintf("PL Auxiliary Voltage:  %f V    \r\n", xadc_data.vccaux);
    kprintf("PL BRAM Voltage:       %f V    \r\n", xadc_data.vccbram);
    kprintf("PS Kernel Voltage:     %f V    \r\n", xadc_data.vccpint);
    kprintf("PS Auxiliary Voltage:  %f V    \r\n", xadc_data.vccpaux);
    kprintf("PS DDR Voltage:        %f V    \r\n", xadc_data.vccpdro);

    return 0;
}

int app_xadc_init(void)
{
    int Status = XST_SUCCESS;
    Status = XadcPs_Init(&Xadc, XADC_DEVICE_ID);
    if (Status != XST_SUCCESS)
    {
        return XST_FAILURE;
    }

    return Status;
}
#endif