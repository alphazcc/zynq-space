/**
 * Copyright (c) 2022-2023，HelloAlpha
 * 
 * Change Logs:
 * Date           Author       Notes
 */
#include "tcp_config.h"
#ifdef USING_ETHERNET_TCP

#include "netif/xadapter.h"

#define USING_ULOG
#include "ulog.h"

/* defined by each RAW mode application */
void tcp_fasttmr(void);
void tcp_slowtmr(void);

#ifdef USING_TCP_STATUS
    int tcp_check_connect_status(void);
#endif

#ifdef USING_TCP_PRINT_MSG
void tcp_print_msg(void)
{
#ifdef USING_ETHERNET0
    static tcp_msg_t *tcp0_msg = &g_tcp0_msg;

    if(tcp0_msg->rx_vflag == 1)
    {
        kprintf("> Eth0 Rec: \r\n");
        kprintf("> Data: %s\r\n", tcp0_msg->rx_pdata);
        kprintf("> Size: %d\r\n", tcp0_msg->rx_plen);
        tcp0_msg->rx_vflag = 0;
    }
#endif

#ifdef USING_ETHERNET1
    static tcp_msg_t *tcp1_msg = &g_tcp1_msg;

    if(tcp1_msg->rx_vflag == 1)
    {
        kprintf("> Eth1 Rec: \r\n");
        kprintf("> Data: %s\r\n", tcp1_msg->rx_pdata);
        kprintf("> Size: %d\r\n", tcp1_msg->rx_plen);
        tcp1_msg->rx_vflag = 0;
    }
#endif
}
#endif

#ifdef USING_TCP_TEST_CMD
int tcp_test_cmd(void)
{
    static uint16_t count = 0;
    static char tcp_buff[32];

    count++;
    sprintf(tcp_buff, "Hello! TCP Count: %d \r\n", count);
    kprintf("size: %d  data: %s", strlen(tcp_buff), tcp_buff);

#ifdef USING_ETHERNET0
    static tcp_node_t *tcp0_node = &g_tcp0_node;
    static tcp_msg_t *tcp0_msg = &g_tcp0_msg;
    if(tcp0_msg->tx_vflag == 0)
    {
        tcp0_msg->tx_pdata = tcp_buff;
        tcp0_msg->tx_plen = strlen(tcp_buff);
        tcp0_msg->tx_vflag = 1;
        kprintf("TCP0 tcp_send: %d \r\n", tcp_send(tcp0_node, tcp0_msg));
    }
#endif

#ifdef USING_ETHERNET1
    static tcp_node_t *tcp1_node = &g_tcp1_node;
    static tcp_msg_t *tcp1_msg = &g_tcp1_msg;
    if(tcp1_msg->tx_vflag == 0)
    {
        tcp1_msg->tx_pdata = tcp_buff;
        tcp1_msg->tx_plen = strlen(tcp_buff);
        tcp1_msg->tx_vflag = 1;
        kprintf("TCP1 tcp_send: %d \r\n", tcp_send(tcp1_node, tcp1_msg));
    }
#endif

    return 0;
}
#endif

int tcp_func_per_500ms(void)
{
    tcp_slowtmr();
    return 0;
}

int tcp_func_per_250ms(void)
{
    tcp_fasttmr();
    return 0;
}

int tcp_func_per_1s(void)
{
#ifdef USING_TCP_STATUS
    tcp_check_connect_status();
#endif

#ifdef USING_TCP_TEST_CMD
    tcp_test_cmd();
#endif
    return 0;
}

int tcp_func_always(void)
{
    /* receive and process packets */
#ifdef USING_ETHERNET0
    static struct netif *echo_netif0 = &server_netif0;
    xemacif_input(echo_netif0);
#endif

#ifdef USING_ETHERNET1
    static struct netif *echo_netif1 = &server_netif1;
    xemacif_input(echo_netif1);
#endif

#ifdef USING_TCP_PRINT_MSG
    tcp_print_msg();
#endif

    return 0;
}

#endif
