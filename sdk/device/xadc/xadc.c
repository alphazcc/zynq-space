/**
 * Copyright (c) 2022-2023，HelloAlpha
 * 
 * Change Logs:
 * Date           Author       Notes
 */
#include "xadc.h"

#ifdef USING_XADC
int XadcPs_Init(XAdcPs *XadcInstancePtr, uint16_t DeviceId)
{
    int Status;
    XAdcPs_Config *XadcConfigPtr;

    XadcConfigPtr = XAdcPs_LookupConfig(DeviceId);
    if (NULL == XadcConfigPtr) {
        return XST_FAILURE;
    }

    Status = XAdcPs_CfgInitialize(XadcInstancePtr, XadcConfigPtr, 
                        XadcConfigPtr->BaseAddress);
    if (Status != XST_SUCCESS) {
        return XST_FAILURE;
    }

    XAdcPs_SetSequencerMode(XadcInstancePtr, XADCPS_SEQ_MODE_SAFE);

    return Status;
}

void XadcPs_GetData(XAdcPs *XadcInstancePtr, XadcData_t *XadcData)
{
    XadcData->raw_temp = XAdcPs_GetAdcData(XadcInstancePtr, XADCPS_CH_TEMP);
    XadcData->temp = XAdcPs_RawToTemperature(XadcData->raw_temp);

    XadcData->raw_vccint = XAdcPs_GetAdcData(XadcInstancePtr, XADCPS_CH_VCCINT);
    XadcData->vccint = XAdcPs_RawToVoltage(XadcData->raw_vccint);

    XadcData->raw_vccaux = XAdcPs_GetAdcData(XadcInstancePtr, XADCPS_CH_VCCAUX);
    XadcData->vccaux = XAdcPs_RawToVoltage(XadcData->raw_vccaux);

    XadcData->raw_vccbram = XAdcPs_GetAdcData(XadcInstancePtr, XADCPS_CH_VBRAM);
    XadcData->vccbram = XAdcPs_RawToVoltage(XadcData->raw_vccbram);

    XadcData->raw_vccpint = XAdcPs_GetAdcData(XadcInstancePtr, XADCPS_CH_VCCPINT);
    XadcData->vccpint = XAdcPs_RawToVoltage(XadcData->raw_vccpint);

    XadcData->raw_vccpaux = XAdcPs_GetAdcData(XadcInstancePtr, XADCPS_CH_VCCPAUX);
    XadcData->vccpaux = XAdcPs_RawToVoltage(XadcData->raw_vccpaux);

    XadcData->raw_vccpdro = XAdcPs_GetAdcData(XadcInstancePtr, XADCPS_CH_VCCPDRO);
    XadcData->vccpdro = XAdcPs_RawToVoltage(XadcData->raw_vccpdro);
}
#endif
