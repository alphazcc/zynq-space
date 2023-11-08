/**
 * Copyright (c) 2022-2023，HelloAlpha
 * 
 * Change Logs:
 * Date           Author       Notes
 */
#include "tcp_config.h"
#ifdef USING_ETHERNET_TCP

#include "lwip/err.h"
#include <string.h>

#define USING_ULOG
#include "ulog.h"

#ifdef USING_ETHERNET0
static tcp_node_t *tcp0_node = &g_tcp0_node;
#endif
#ifdef USING_ETHERNET1
static tcp_node_t *tcp1_node = &g_tcp1_node;
#endif

/**
 * @brief TCP data sending function
 * 
 * @return err_t 
 */
err_t tcp_transfer_data(struct tcp_pcb *tpcb, const uint8_t *pData, int plen)
{
    err_t err;
    static struct pbuf *tcp_pbuf = NULL;
    static int tcp_len = 0;

    /* If parameter length bigger than tcp_len, reallocate memory space */
    if(plen > tcp_len)
    {
        if(tcp_pbuf)
        {
            /* free pbuf */
            pbuf_free(tcp_pbuf);
        }
        tcp_len = plen;
        /* allocate memory space to pbuf */
        tcp_pbuf = pbuf_alloc(PBUF_TRANSPORT, tcp_len, PBUF_RAM);
        if(!tcp_pbuf)
        {
            ULOG("pbuf_alloc %d fail!\r\n", tcp_len);
            tcp_len = 0;
            return ERR_BUF;
        }
    }
    /* copy data to pbuf payload */
    memcpy(tcp_pbuf->payload, pData, plen);
    tcp_pbuf->len = plen;
    tcp_pbuf->tot_len = plen;
    /* Start to send udp data */
    err = tcp_write(tpcb, (tcp_pbuf->payload), (tcp_pbuf->len), 
                        TCP_WRITE_FLAG_COPY | TCP_WRITE_FLAG_MORE);
    if (err != ERR_OK)
    {
        ULOG("Error on tcp_write: %d!\r\n", err);
        return err;
    }
    err = tcp_output(tpcb);
    if (err != ERR_OK)
    {
        ULOG("Error on tcp_output: %d!\r\n", err);
        return err;
    }
    return err;
}

/**
 * @brief TCP data sending function
 * 
 * @return err_t 
 */
err_t tcp_send(tcp_node_t *tcp_node, tcp_msg_t *tcp_msg)
{
    err_t err;
    if(!tcp_node)
    {
        ULOG("Empty TCP node!\r\n");
        return ERR_ARG;
    }
    else if(!tcp_msg)
    {
        ULOG("Empty TCP message!\r\n");
        return ERR_ARG;
    }
    else if(!tcp_node->connected_flag)
    {
        ULOG("TCP%d is not currently connected!\r\n",
                (tcp_node->local_port == DEFAULT_PORT0) ? 0 : 1);
        tcp_msg->tx_vflag = 0;
        return ERR_CONN;
    }
    else if(!tcp_msg->tx_vflag)
    {
        ULOG("Flag indicates message is not ready!\r\n");
        return ERR_ARG;
    }
    else
    {
        err = tcp_transfer_data(tcp_node->pcb_obj,
                    (uint8_t *)tcp_msg->tx_pdata, tcp_msg->tx_plen);
        tcp_msg->tx_vflag = 0;
    }
    return err;
}

/**
 * @brief close the TCP connection
 *
 * @return int
 */
static int tcp_connection_close(struct tcp_pcb *tpcb)
{
    tcp_close(tpcb);
    tcp_arg(tpcb, NULL);
    tcp_recv(tpcb, NULL);
    tcp_sent(tpcb, NULL);
    tcp_poll(tpcb, NULL, 0);
    tcp_err(tpcb, NULL);

#ifdef USING_ETHERNET0
    if(tpcb->local_port == DEFAULT_PORT0)
    {
        if(tcp0_node->connected_flag == 1)
        {
            /* reset the connection flag */
            tcp0_node->connected_flag = 0;
        }
    }
#endif

#ifdef USING_ETHERNET1
    if (tpcb->local_port == DEFAULT_PORT1)
    {
        if(tcp1_node->connected_flag == 1)
        {
            /* reset the connection flag */
            tcp1_node->connected_flag = 0;
        }
    }
#endif
    return ERR_OK;
}

/**
 * @brief TCP data receiving callback function
 * 
 * @return err_t 
 */
static err_t recv_callback(void *arg, struct tcp_pcb *tpcb,
                               struct pbuf *p_rx, err_t err)
{
    /* do not read the packet if we are not in ESTABLISHED state */
    if (!p_rx)
    {
        tcp_connection_close(tpcb);
        return ERR_OK;
    }

    /* indicate that the packet has been received */
    tcp_recved(tpcb, p_rx->len);

    /* the data is processed here. */
    {
#ifndef USING_TCP_LOOKBACK

#ifdef USING_ETHERNET0
        static tcp_msg_t *tcp0_msg = &g_tcp0_msg;

        if(tpcb->local_port == DEFAULT_PORT0)
        {
            if(!tcp0_msg->rx_vflag)
            {
                tcp0_msg->rx_pdata = tcp0_msg->rx_pdata_buf;
                memcpy(tcp0_msg->rx_pdata, (char *) p_rx->payload, p_rx->len);
                tcp0_msg->rx_plen = p_rx->len;
                tcp0_msg->rx_vflag = 1;
            }
        }
#endif

#ifdef USING_ETHERNET1
        static tcp_msg_t *tcp1_msg = &g_tcp1_msg;

        if(tpcb->local_port == DEFAULT_PORT1)
        {
            if(!tcp1_msg->rx_vflag)
            {
                tcp1_msg->rx_pdata = tcp1_msg->rx_pdata_buf;
                memcpy(tcp1_msg->rx_pdata, (char *) p_rx->payload, p_rx->len);
                tcp1_msg->rx_plen = p_rx->len;
                tcp1_msg->rx_vflag = 1;
            }
        }
#endif

#else
        char TcpSend[] = "> LookBack Test Running... \r\n> Rec Data: ";

        tcp_transfer_data(tpcb, (uint8_t *)TcpSend, strlen(TcpSend));

        /* echo back the payload */
        /* in this case, we assume that the payload is < TCP_SND_BUF */
        if (tcp_sndbuf(tpcb) > p_rx->len)
        {
            err = tcp_write(tpcb, p_rx->payload, p_rx->len, 1);
        }
        else
        {
            ULOG("No space in tcp_sndbuf...\n\r");
        }
#endif
    }

    /* free the received pbuf */
    pbuf_free(p_rx);

    return ERR_OK;
}

/**
 * @brief TCP Callback function for successful TCP data sending
 *
 * @return err_t
 */
static err_t tcp_sent_callback(void *arg, struct tcp_pcb *tpcb, u16_t len)
{
#ifdef USING_ETHERNET0
    static uint32_t tcp0_trans_cnt = 0;
    if(tpcb->local_port == DEFAULT_PORT0)
    {
        ULOG("TCP0 Send Success Count: %d \r\n", tcp0_trans_cnt++);
    }
#endif

#ifdef USING_ETHERNET1
    static uint32_t tcp1_trans_cnt = 0;
    if (tpcb->local_port == DEFAULT_PORT1)
    {
        ULOG("TCP1 Send Success Count: %d \r\n", tcp1_trans_cnt++);
    }
#endif

    return ERR_OK;
}

static err_t tcp_poll_callback(void *arg, struct tcp_pcb *tpcb)
{
    /* it is executed every 500ms */
#ifdef USING_ETHERNET0
    if(tpcb->local_port == DEFAULT_PORT0)
    {
        tcp0_node->connected_count++;
    }
#endif

#ifdef USING_ETHERNET1
    if(tpcb->local_port == DEFAULT_PORT1)
    {
        tcp1_node->connected_count++;
    }
#endif

    return ERR_OK;
}

static void tcp_err_callback(void *arg, err_t err)
{
    LWIP_UNUSED_ARG(err);
    ULOG("TCP error: %x \r\n", (uint32_t)arg);

    if(NULL != arg)
    {

    }
}

/**
 * @brief When TCP is in LISTEN state, listen for a new connection 
 * 		  and execute this function
 *
 * @return err_t 
 */
err_t accept_callback(void *arg, struct tcp_pcb *newpcb, err_t err)
{
    static int connection = 1;

    /* set the receive callback for this connection */
    tcp_recv(newpcb, recv_callback);

    /* set the sent callback for this connection */
    tcp_sent(newpcb, tcp_sent_callback);

    tcp_poll(newpcb, tcp_poll_callback, 1);

    tcp_err(newpcb, tcp_err_callback);

    {
        ULOG("\r\n----- TCP Connection -----\r\n");
        ULOG(" Local IP: %d.%d.%d.%d, Local Port: %d \r\n",
            (newpcb->local_ip.addr) & 0xff, (newpcb->local_ip.addr >> 8) & 0xff,
            (newpcb->local_ip.addr >> 16) & 0xff, (newpcb->local_ip.addr >> 24) & 0xff,
            newpcb->local_port);
        ULOG(" Remote IP: %d.%d.%d.%d, Remote Port: %d \r\n",
            (newpcb->remote_ip.addr) & 0xff, (newpcb->remote_ip.addr >> 8) & 0xff,
            (newpcb->remote_ip.addr >> 16) & 0xff, (newpcb->remote_ip.addr >> 24) & 0xff,
            newpcb->remote_port);

#ifdef USING_ETHERNET0
        if(newpcb->local_port == DEFAULT_PORT0)
        {
            if(tcp0_node->connected_flag == 0)
            {
                /* set the connection flag */
                tcp0_node->connected_flag = 1;
                tcp0_node->pcb_obj = newpcb;
            }
        }
#endif

#ifdef USING_ETHERNET1
        if (newpcb->local_port == DEFAULT_PORT1)
        {
            if(tcp1_node->connected_flag == 0)
            {
                /* set the connection flag */
                tcp1_node->connected_flag = 1;
                tcp1_node->pcb_obj = newpcb;
            }
        }
#endif
    }

    /* just use an integer number indicating the connection id as the
       callback argument */
    tcp_arg(newpcb, (void*)(UINTPTR)connection);

    /* increment for subsequent accepted connections */
    connection++;

    return ERR_OK;
}

#ifdef USING_TCP_STATUS

#ifdef USING_ETHERNET0
static int tcp0_check_connect_status(void)
{
    static uint32_t last_tcp0_count = 0;
    static uint8_t tcp0_status = 0;

    if(tcp0_node->connected_count > last_tcp0_count)
    {
        if(tcp0_status == 0)
        {
            tcp0_status = 1;
            ULOG("Tcp0 Connected!\r\n");
        }

        if(tcp0_node->connected_count > 65530)
        {
            tcp0_node->connected_count = 1;
        }
        last_tcp0_count = tcp0_node->connected_count;
    }
    else
    {
        tcp0_node->connected_flag = 0;
        if(tcp0_status == 1)
        {
            tcp0_status = 0;
            ULOG("Tcp0 Disconnected!\r\n");
        }
    }

    return 0;
}
#endif

#ifdef USING_ETHERNET1
static int tcp1_check_connect_status(void)
{
    static uint32_t last_tcp1_count = 0;
    static uint8_t tcp1_status = 0;

    if(tcp1_node->connected_count > last_tcp1_count)
    {
        if(tcp1_status == 0)
        {
            tcp1_status = 1;
            ULOG("Tcp1 Connected!\r\n");
        }

        if(tcp1_node->connected_count > 65530)
        {
            tcp1_node->connected_count = 1;
        }
        last_tcp1_count = tcp1_node->connected_count;
    }
    else
    {
        tcp1_node->connected_flag = 0;
        if(tcp1_status == 1)
        {
            tcp1_status = 0;
            ULOG("Tcp1 Disconnected!\r\n");
        }
    }

    return 0;
}
#endif

/**
 * @brief Check TCP connect status
 *
 */
int tcp_check_connect_status(void)
{
#ifdef USING_ETHERNET0
    tcp0_check_connect_status();
#endif

#ifdef USING_ETHERNET1
    tcp1_check_connect_status();
#endif

    return 0;
}

#endif

#endif
