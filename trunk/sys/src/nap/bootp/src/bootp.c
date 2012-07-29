/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

#include <ace/string.h>
#include <ace/stddef.h>
#include <ace/stdio.h>
#include <uso/sleep.h>
#include <uso/buf_pool.h>
#include <uso/log.h>
#include <uso/heap.h>
#include <net/udp_sock.h>
#include <net/ethif.h>
#include <net/ethaddr.h>

#include "nap/bootp.h"
#include "nap/debug.h"

/******************************************************************************** 
 * Declarations & Defines
 ********************************************************************************/

#define REQUEST    1            /* packet op codes */
#define REPLY      2

#define ETHHWTYPE      1

#define HW_ADDRESS_SIZE   16
#define FILE_NAME_SIZE   128
#define SERVER_NAME_SIZE  64
#define VEND_SIZE         64

#define BOOTP_CLIENT_PORT 68
#define BOOTP_SERVER_PORT 67

#define TRANSACTION_ID 0xaffe   /* an arbitrary number */

#define REPLY_TIMEOUT USO_MSEC_2_TICKS(3000)    /* 3 sec */
#define RETRY_TIME    USO_MSEC_2_TICKS(2000)    /* 2 sec */
#define RETRIES   5

struct bootp_packet
{
    unsigned char op;           /* request or reply */
    unsigned char htype;        /* hardware type */
    unsigned char hlen;         /* hardware address length */
    unsigned char hops;         /* set to zero */
    unsigned long xid;          /* transaction id */
    unsigned short secs;        /* time client has been trying */
    unsigned short flags;       /* unused */
    unsigned long ciaddr;       /* client IP address */
    unsigned long yiaddr;       /* your (client) IP address */
    unsigned long siaddr;       /* server IP address */
    unsigned long giaddr;       /* gateway IP address */
    unsigned char chaddr[HW_ADDRESS_SIZE];      /* client hardware address */
    unsigned char sname[SERVER_NAME_SIZE];      /* server host name */
    unsigned char file[FILE_NAME_SIZE]; /* boot file name */
    unsigned char vend[VEND_SIZE];
} ACE_PACKED_;

/********************************************************************************
 * Definitions
 ********************************************************************************/

NAP_bootp_data_t NAP_bootp_data;

static struct bootp_packet *request_data;
static struct bootp_packet *reply_data;
static struct NET_eth_addr *hw_address;

static void
bootp_init_request (void)
{
    memset (request_data, 0, sizeof (struct bootp_packet));
    request_data->op = REQUEST;
    request_data->htype = ETHHWTYPE;
    request_data->hlen = NET_ETH_ADDR_SIZE;
    request_data->xid = ACE_htonl (TRANSACTION_ID);
    memcpy (request_data->chaddr, hw_address->addr, NET_ETH_ADDR_SIZE);
}

static int
bootp_check_reply (void)
{
    if (reply_data->op != REPLY)
    {
        return -1;
    }
    if (reply_data->xid != ACE_htonl (TRANSACTION_ID))
    {
        return -2;
    }
    if (reply_data->yiaddr == 0)
    {
        return -3;
    }
    if (reply_data->siaddr == 0)
    {
        return -4;
    }
    if (memcmp (reply_data->chaddr, hw_address->addr, NET_ETH_ADDR_SIZE))
    {
        return -6;
    }
    return 0;
}

/******************************************************************************** 
 * BOOTP
 ********************************************************************************/
static struct bootp_packet *bootp_packets = NULL;

extern int
NAP_bootp (struct NET_eth_addr *hwaddr)
{
    int err = -1;
    static NET_udp_socket_t sock;
    static NET_ip_addr_t server_addr;
    static NET_ip_addr_t client_addr;
    int i;

    bootp_packets = ACE_malloc (sizeof (struct bootp_packet));
    if (bootp_packets == NULL)
    {
        USO_log_puts (USO_LL_ERROR, "BOOTP mem error\n");
        return err;
    }
    hw_address = hwaddr;
    NET_udp_socket_init (&sock);
    NET_ip4_addr (&server_addr, 255, 255, 255, 255);
    NET_ip4_addr (&client_addr, 0, 0, 0, 0);
    NET_udp_bind (&sock, &client_addr, BOOTP_CLIENT_PORT);
    NET_udp_connect (&sock, &server_addr, BOOTP_SERVER_PORT);
    NET_udp_socket_open (&sock);
    NET_udp_recv_timeout (&sock, REPLY_TIMEOUT);
    for (i = 0; i < RETRIES; ++i)
    {
        request_data = bootp_packets;
        bootp_init_request ();
        if (NET_udp_send (&sock, (char *)request_data, sizeof (struct bootp_packet)) < 0)
        {
            USO_log_puts (USO_LL_ERROR, "BOOTP send error\n");
            USO_sleep (RETRY_TIME);
        }
        else
        {
            long len;
            ACE_u16_t server_port;
            DEBUGF (NAP_BOOTP_DEBUG, ("BOOTP request\n"));
            reply_data = bootp_packets;
            len = NET_udp_recv (&sock, NULL, &server_port,
                                (char *)reply_data, sizeof (struct bootp_packet));
            if (len >= sizeof (struct bootp_packet))
            {
                if (server_port == BOOTP_SERVER_PORT)
                {
                    int check_reply_err;
                    check_reply_err = bootp_check_reply ();
                    if (check_reply_err >= 0)
                    {
                        NAP_bootp_data.ip_addr.addr = reply_data->yiaddr;
                        NAP_bootp_data.server.addr = reply_data->siaddr;
                        NAP_bootp_data.gateway.addr = reply_data->giaddr;
                        memcpy (NAP_bootp_data.filename,
                                reply_data->file, sizeof (NAP_bootp_data.filename));
                        NAP_bootp_data.filename[sizeof (NAP_bootp_data.filename) - 1] = '\0';
                        USO_log_puts (USO_LL_INFO, "BOOTP done\n");
                        err = 0;
                        break;
                    }
                    else
                    {
                        USO_log_printf (USO_LL_WARNING, "BOOTP wrong data: %d\n", check_reply_err);
                    }
                }
                else
                {
                    USO_log_puts (USO_LL_WARNING, "BOOTP wrong server port\n");
                }
            }
            else
            {
                if (len == 0)
                    USO_log_puts (USO_LL_WARNING, "BOOTP timeout\n");
                else
                    USO_log_puts (USO_LL_WARNING, "BOOTP invalid len\n");
            }
        }
    }

    NET_udp_socket_close (&sock);
    DEBUGF (NAP_BOOTP_DEBUG, ("BOOTP sock closed\n"));
    ACE_free (bootp_packets);
    return err;
}
