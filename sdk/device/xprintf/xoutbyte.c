/**
 * Copyright (c) 2022-2023，HelloAlpha
 * 
 * Change Logs:
 * Date           Author       Notes
 */
#include "xprintf.h"
#include "xuartps_hw.h"

#ifdef __cplusplus
extern "C" {
#endif
void xoutbyte(char c);

#ifdef __cplusplus
}
#endif 

#ifdef STDOUT_BASEADDRESS_UART1
void xoutbyte(char c)
{
    XUartPs_SendByte(STDOUT_BASEADDRESS_UART1, c);
}
#endif
