/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

#include <ace/stddef.h>
#include <ace/string.h>
#include <uso/stack.h>
#include <uso/heap.h>
#include <uso/log.h>
#include <net/udp.h>
#include <cli/commands.h>

#include "arch/eth.h"
#include "net_test.h"

static CLI_exec_t net_test;

static NET_udp_socket_t udp_sock;
static NET_ip_addr_t local_addr;
static NET_ip_addr_t remote_addr;

#define LOCAL_PORT 1778
#define REMOTE_PORT 1777

static USO_buf_pool_t test_pool;
struct test_packet
{
    NET_netbuf_t buf_hdr;
    char data[16];
    long trans_nr;
} _PACKED_;

static struct test_packet test_packets[2];

static void
test_packet_init (struct test_packet *p)
{
    USO_buf_pool_init (&test_pool, test_packets,
                       1, sizeof (struct test_packet));
}

static void
net_test_exec (char *nix)
{
    long tn = 0;

    NET_udp_socket_init (&udp_sock);
    NET_ip4_addr (&local_addr, 192, 168, 1, 255);
    // NET_ip4_addr(&local_addr, 192, 168, 1, 11);
    NET_ip4_addr (&remote_addr, 192, 168, 1, 10);
    NET_udp_bind (&udp_sock, &local_addr, LOCAL_PORT);
    NET_udp_connect (&udp_sock, &remote_addr, REMOTE_PORT);
    NET_udp_socket_open (&udp_sock);
    for (;;)
    {
        NET_netbuf_t *buf, *p;
        buf = NET_udp_recv (&udp_sock, NULL, NULL);
        USO_kputs (USO_LL_INFO, "USER received udp.\n");
        NET_netbuf_free (buf);
        p = NET_netbuf_alloc (&test_pool, 0, NULL);
        memcpy (((struct test_packet *)p)->data, "**Maik Threads**", 16);
        ((struct test_packet *)p)->trans_nr = tn++;
        if ((NET_udp_send (&udp_sock, p)) < 0)
        {
            USO_kputs (USO_LL_INFO, "USER send error.\n");
            NET_netbuf_free (p);
        }
        else
        {
            USO_kputs (USO_LL_INFO, "USER send udp.\n");
        }
    }
}

extern void
net_test_install(MFS_descriptor_t *test)
{
    test_packet_init (&test_packets[1]);
    CLI_exec_init (test, &net_test, "nett", "Net Test", net_test_exec);
}
