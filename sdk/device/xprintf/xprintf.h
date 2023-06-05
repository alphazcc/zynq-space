 /**
 * Copyright (c) 2022-2023，HelloAlphaelloAlpha
 * 
 * Change Logs:
 * Date           Author       Notes
 */
#ifndef __XPRINTF_H__
#define __XPRINTF_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <ctype.h>
#include <string.h>
#include <stdarg.h>
#include "xil_types.h"
#include "xparameters.h"

#ifdef XPAR_PS7_UART_1_BASEADDR
#define STDOUT_BASEADDRESS_UART1 XPAR_PS7_UART_1_BASEADDR
#endif

struct params_s;

typedef char8* charptr;
typedef s32 (*func_ptr)(int c);

void xprintf( const char8 *ctrl1, ...);
void xprint(const char8 *ptr);
extern void xoutbyte(char c);

#ifdef __cplusplus
}
#endif

#endif
