#ifndef __PLATFORM_CONFIG_H_
#define __PLATFORM_CONFIG_H_

/**
 * UART config begin
 * @{
 */
#define USING_UART

#ifdef USING_UART
#define USING_UART0
#define USING_UART1
#endif
/**@}*/

/**
 * SD CARD config begin
 * @{
 */
#define USING_SD_CARD
#define USING_SD_CARD_TEST
/**@}*/

#endif
