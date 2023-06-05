/**
 * Copyright (c) 2022-2023，HelloAlpha
 * 
 * Change Logs:
 * Date           Author       Notes
 */
#include "xprintf.h"

#ifdef STDOUT_BASEADDRESS_UART1
void xprint(const char8 *ptr)
{
#ifdef STDOUT_BASEADDRESS_UART1
    while (*ptr != (char8)0) {
        xoutbyte (*ptr);
        ptr++;
    }
#else
    (void)ptr;
#endif
}
#endif
