/**
 * Copyright (c) 2022-2023，HelloAlpha
 * 
 * Change Logs:
 * Date           Author       Notes
 */
#ifndef __TCP_CONFIG_H__
#define __TCP_CONFIG_H__

#include "xparameters.h"

/**
 * TCP config begin
 * @{
 */
#define USING_ETHERNET_TCP

#if defined USING_ETHERNET_TCP
#define USING_ETHERNET0
#define USING_ETHERNET1
#define USING_TCP_STATUS
//#define USING_TCP_LOOKBACK
//#define USING_TCP_PRINT_MSG
//#define USING_TCP_TEST_CMD
#endif

#ifdef USING_ETHERNET0
#define PLATFORM_EMAC_BASEADDR XPAR_XEMACPS_0_BASEADDR
#endif

#ifdef USING_ETHERNET1
#define PLATFORM_EMAC1_BASEADDR XPAR_XEMACPS_1_BASEADDR
#endif
/**@}*/

#ifdef USING_ETHERNET_TCP

#include "lwip/tcp.h"

/* default IP port */
#define DEFAULT_PORT0       7
#define DEFAULT_PORT1       8

#define MAX_SEND_LEN 		64
#define TCP_MSG_MSG_LEN     256

struct tcp_msg
{
    char rx_pdata_buf[TCP_MSG_MSG_LEN];
    char *rx_pdata;
    uint16_t rx_plen;
    uint8_t rx_vflag;
    char tx_pdata_buf[TCP_MSG_MSG_LEN];
    char *tx_pdata;
    uint16_t tx_plen;
    uint8_t tx_vflag;
};

typedef struct tcp_msg tcp_msg_t;

struct tcp_node
{
    struct tcp_pcb *pcb_obj;
    struct ip_addr local_addr;
    uint16_t local_port;
    struct ip_addr target_addr;
    uint16_t remote_port;
    uint8_t connected_flag;
    struct tcp_msg *msg;
    uint32_t connected_count;
};

typedef struct tcp_node tcp_node_t;

#ifdef USING_ETHERNET0
struct netif server_netif0;
/* definition TCP node */
tcp_node_t g_tcp0_node;
/* definition TCP message node*/
tcp_msg_t g_tcp0_msg;
#endif

#ifdef USING_ETHERNET1
struct netif server_netif1;
/* definition TCP node */
tcp_node_t g_tcp1_node;
/* definition TCP message node*/
tcp_msg_t g_tcp1_msg;
#endif

err_t tcp_transfer_data(struct tcp_pcb *tpcb, const uint8_t *pData, int plen);
err_t tcp_send(tcp_node_t *tcp_node, tcp_msg_t *tcp_msg);
void tcp_node_printf(char *tcp_node_name, tcp_node_t *tcp_node);
int lwip_tcp_init(void);

#endif
#endif
