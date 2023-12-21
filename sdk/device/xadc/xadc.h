/**
 * Copyright (c) 2022-2023，HelloAlpha
 * 
 * Change Logs:
 * Date           Author       Notes
 */
#ifndef __XADC_H__
#define __XADC_H__

#define USING_XADC

#ifdef USING_XADC
#include "xparameters.h"
#include "xadcps.h"

#define XADC_DEVICE_ID	XPAR_XADCPS_0_DEVICE_ID

struct Xadc_Data
{
    float temp;     // Temperature
    float vccint;   // PL kernel voltage
    float vccaux;   // PL auxiliary voltage
    float vccbram;  // PL BRAM voltage
    float vccpint;  // PS kernel voltage
    float vccpaux;  // PS auxiliary voltage
    float vccpdro;  // PS DDR voltage

    uint16_t raw_temp;
    uint16_t raw_vccint;
    uint16_t raw_vccaux;
    uint16_t raw_vccbram;
    uint16_t raw_vccpint;
    uint16_t raw_vccpaux;
    uint16_t raw_vccpdro;
};

typedef struct Xadc_Data XadcData_t;

int XadcPs_Init(XAdcPs *XadcInstancePtr, uint16_t DeviceId);
void XadcPs_GetData(XAdcPs *XadcInstancePtr, XadcData_t *XadcData);

#endif
#endif