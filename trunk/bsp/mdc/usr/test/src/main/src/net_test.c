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
#include "init/config.h"
#include "net_test.h"

#define LOCAL_PORT 1778
#define REMOTE_PORT 1777

#define PACK_BUF_MAX 2

struct test_packet
{
    char name[16];
    long trans_nr;
} ACE_PACKED_;

static const char* name = "Maik Threads";

static CLI_exec_t net_test;
static NET_udp_socket_t udp_sock;
static USO_buf_pool_t test_pool;
static struct test_packet test_packets[PACK_BUF_MAX];

static void
test_packet_init (void)
{
	USO_kputs (USO_LL_INFO, "NT init.\n");
    USO_buf_pool_init (&test_pool, test_packets,
                       PACK_BUF_MAX, sizeof (struct test_packet));
}

static void
net_test_exec (char *nix)
{
	int i;

    NET_udp_socket_init (&udp_sock);
    NET_udp_bind (&udp_sock, &MDC_ee_config.ip_addr, LOCAL_PORT);
    NET_udp_connect (&udp_sock, &MDC_ee_config.server, REMOTE_PORT);
    NET_udp_socket_open (&udp_sock);
	USO_kputs (USO_LL_INFO, "NT sock open.\n");
    for (i = 0; i < 10; ++i)
    {
        struct test_packet* data;
        NET_netbuf_t *p;
	    ACE_u16_t port;
        p = NET_netbuf_alloc_pool (&test_pool);
      	data = (struct test_packet*)NET_netbuf_index(p);
      	memset (data->name, 0x0, sizeof(data->name));
      	memcpy (data->name, name,
      		ACE_MIN(sizeof(data->name) - 1, ACE_strlen(name)) );
        data->trans_nr = i;
        if ((NET_udp_send_netbuf (&udp_sock, p)) < 0)
        {
            USO_kputs (USO_LL_INFO, "NT send error.\n");
        } else {
            USO_kputs (USO_LL_INFO, "NT send udp.\n");
        }

		NET_udp_recv_timeout (&udp_sock, 1000);	
        p = NET_udp_recv_netbuf (&udp_sock, NULL, &port);
        if (p != NULL){
        	USO_kputs (USO_LL_INFO, "NT receive udp.\n");
        	NET_netbuf_free (p);
        } else {
        	USO_kputs (USO_LL_INFO, "NT receive error.\n");
        }
    }
    NET_udp_socket_close (&udp_sock);
    USO_kputs (USO_LL_INFO, "NT sock closed.\n");
}

extern void
net_test_install(MFS_descriptor_t *test)
{
    test_packet_init ();
    CLI_exec_init (test, &net_test, "netT", "Net Test", net_test_exec);
}
