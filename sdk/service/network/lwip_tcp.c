/**
 * Copyright (c) 2022-2023，HelloAlpha
 * 
 * Change Logs:
 * Date           Author       Notes
 */
#include "tcp_config.h"
#ifdef USING_ETHERNET_TCP

#include "lwip/init.h"
#include "lwip/err.h"
#include "netif/xadapter.h"

#define USING_ULOG
#include "ulog.h"

err_t accept_callback(void *arg, struct tcp_pcb *newpcb, err_t err);

static void print_ip(char *msg, struct ip_addr *ip)
{
    kprintf(msg);
    kprintf("%d.%d.%d.%d\r\n", ip4_addr1(ip), ip4_addr2(ip),
                            ip4_addr3(ip), ip4_addr4(ip));
}

void tcp_node_printf(char *tcp_node_name, tcp_node_t *tcp_node)
{
    kprintf("------------- %s -------------\r\n", tcp_node_name);
    print_ip("local addr: ", &(tcp_node->local_addr));
    kprintf("local port: %d \r\n" ,tcp_node->local_port);
    print_ip("target addr: ", &(tcp_node->target_addr));
    kprintf("remote port: %d \r\n", tcp_node->remote_port);
    kprintf("-------------------------------------\r\n");
}

static void print_app_header()
{
    kprintf("\n\r----- lwIP TCP ------\r\n");
}

/**
 * @brief
 *
 * @return err_t
 */
err_t start_tcp_application(tcp_node_t *tcp_node)
{
    err_t err;

   if(!tcp_node)
   {
       ULOG("TCP node is empty!\r\n");
       return ERR_ARG;
   }
   /* create pcb */
    tcp_node->pcb_obj = tcp_new();
    if (!tcp_node->pcb_obj)
    {
        ULOG("Error creating PCB. Out of Memory!\r\n");
        return ERR_MEM;
    }
    /* bind to specified @port */
    err = tcp_bind(tcp_node->pcb_obj, &(tcp_node->local_addr), tcp_node->local_port);
    if (err != ERR_OK)
    {
        ULOG("Unable to bind to port %d: err = %d!\r\n", tcp_node->local_port, err);
        return ERR_ABRT;
    }
    /* we do not need any arguments to callback functions */
    tcp_arg(tcp_node->pcb_obj, NULL);
    /* listen for connections */
    tcp_node->pcb_obj = tcp_listen(tcp_node->pcb_obj);
    if (!tcp_node->pcb_obj)
    {
        ULOG("Out of memory while tcp_listen!\r\n");
        return ERR_MEM;
    }
    /* specify callback to use for incoming connections */
    tcp_accept(tcp_node->pcb_obj, accept_callback);
    return err;
}

#ifdef USING_ETHERNET0
static int ethernet0_init(void)
{
    err_t err;
    struct netif *echo_netif0 = &server_netif0;
    struct ip_addr ipaddr0, netmask0, gw0;
    /* the mac address of the board. this should be unique per board */
    unsigned char mac_ethernet_address0[] = { 0x00, 0x0a, 0x35, 0x00, 0x01, 0x02 };

    /* initliaze IP addresses to be used */
    IP4_ADDR(&ipaddr0,  192, 168,   6, 10);
    IP4_ADDR(&netmask0, 255, 255, 255,  0);
    IP4_ADDR(&gw0,      192, 168,   1,  1);

    /* Add network interface to the netif_list, and set it as default */
    if (!xemac_add(echo_netif0, &ipaddr0, &netmask0,
                        &gw0, mac_ethernet_address0,
                        PLATFORM_EMAC_BASEADDR))
    {
        kprintf("ENET0 Error adding N/W interface!\r\n");
        return -1;
    }
    netif_set_default(echo_netif0);
    /* specify that the network if is up */
    netif_set_up(echo_netif0);

    /* initialize TCP0 node */
    tcp_node_t *tcp0_node = &g_tcp0_node;
    /* local IP */
     tcp0_node->local_addr.addr = ipaddr0.addr;
    /* local port */
    tcp0_node->local_port = DEFAULT_PORT0;
    /* target IP */
    IP4_ADDR(&(tcp0_node->target_addr), 192,168,6,81);
    /* target port */
    tcp0_node->remote_port = 8080;
    /* clear the connection flag */
    tcp0_node->connected_flag = 0;

    /* start the application */
    err = start_tcp_application(tcp0_node);
    if(err != ERR_OK)
    {
        kprintf("Start TCP0 application failed!\r\n");
        return -1;
    }
    else
    {
        tcp_node_printf("TCP0 Node", tcp0_node);
    }

    return 0;
}
#endif

#ifdef USING_ETHERNET1
static int ethernet1_init(void)
{
    err_t err;
    struct netif *echo_netif1 = &server_netif1;
    struct ip_addr ipaddr1, netmask1, gw1;
    /* the mac address of the board. this should be unique per board */
    unsigned char mac_ethernet_address1[] =	{ 0x00, 0x0a, 0x35, 0x00, 0x01, 0x03 };

    /* initliaze IP addresses to be used */
    IP4_ADDR(&ipaddr1,  192, 168,   6, 20);
    IP4_ADDR(&netmask1, 255, 255, 255,  0);
    IP4_ADDR(&gw1,      192, 168,   1,  1);

    if (!xemac_add(echo_netif1, &ipaddr1, &netmask1,
                        &gw1, mac_ethernet_address1,
                        PLATFORM_EMAC1_BASEADDR))
    {
        kprintf("ENET1 Error adding N/W interface!\r\n");
        return -1;
    }
    netif_set_default(echo_netif1);
    /* specify that the network if is up */
    netif_set_up(echo_netif1);

    /* initialize TCP1 node */
    tcp_node_t *tcp1_node = &g_tcp1_node;
    /* local IP */
     tcp1_node->local_addr.addr = ipaddr1.addr;
    /* local port */
    tcp1_node->local_port = DEFAULT_PORT1;
    /* target IP */
    IP4_ADDR(&(tcp1_node->target_addr), 192,168,6,81);
    /* target port */
    tcp1_node->remote_port = 8081;
    /* clear the connection flag */
    tcp1_node->connected_flag = 0;

    /* start the application */
    err = start_tcp_application(tcp1_node);
    if(err != ERR_OK)
    {
        kprintf("Start TCP1 application failed!\r\n");
        return -1;
    }
    else
    {
        tcp_node_printf("TCP1 Node", tcp1_node);
    }

    return 0;
}
#endif

int lwip_tcp_init(void)
{
    print_app_header();
    lwip_init();

#ifdef USING_ETHERNET0
    ethernet0_init();
#endif

#ifdef USING_ETHERNET1
    ethernet1_init();
#endif

    return 0;
}

#endif
