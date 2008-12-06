/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

#include <ace/stddef.h>
#include <dev/arch/cpu.h>
#include <net/arch/at91_emac.h>
#include <net/netif.h>
#include <net/err.h>
#include <net/ethernet.h>
#include <net/ip.h>
#include <net/loopif.h>
#include <net/udp.h>
#include <net/netbuf.h>
#include <net/stats.h>

#include "arch/digio.h"
#include "arch/eth.h"

NET_netif_t SAM_lo;
NET_netif_t SAM_eth0;

static NET_ip_addr_t ip_lo;
static NET_ip_addr_t netmask_lo;

static NET_ip_addr_t ip_eth0;
static NET_ip_addr_t netmask_eth0;
static NET_ip_addr_t gateway_eth0;

static NET_ethif_t ethif0;
static NET_at91_emac_t mac;

struct NET_eth_addr SAM_eth_addr = {{0xa0, 0xba, 0xd0, 0xca, 0xfe, 0x02}};

static void
eth_power_on (void)
{
    DEV_digout_clear (&SAM_phy_power_down);
    DEV_cpudelay (ACE_USEC_2_LOOPS(1000));
}

extern void
SAM_eth_init (void)
{
	NET_stats_init ();	
    NET_netbuf_init ();
	NET_eth_init ();
	NET_ip_init();
    NET_udp_init ();

    NET_netif_init (&SAM_lo, "lo");
    NET_ip4_addr (&ip_lo, 127, 0, 0, 1);
    NET_ip4_addr (&netmask_lo, 255, 0, 0, 0);
	NET_netif_set_ipaddr (&SAM_lo, &ip_lo);
	NET_netif_set_netmask (&SAM_lo, &netmask_lo);
    NET_loopif_init (&SAM_lo);

    NET_netif_init (&SAM_eth0, "eth0");
    NET_ip4_addr (&ip_eth0, 192, 168, 2, 102);
    NET_ip4_addr (&netmask_eth0, 255, 255, 255, 0);
    NET_ip4_addr (&gateway_eth0, 192, 168, 2, 2);
	NET_netif_set_ipaddr (&SAM_eth0, &ip_eth0);
	NET_netif_set_netmask (&SAM_eth0, &netmask_eth0);
	NET_netif_set_gateway (&SAM_eth0, &gateway_eth0);
    NET_ethif_init (&SAM_eth0, &ethif0, &SAM_eth_addr);

	NET_netif_set_default (&SAM_eth0);
    eth_power_on ();
    NET_at91_emac_init (&ethif0, &mac);
}

extern void
SAM_eth_start (void)
{
    NET_ethif_start (&ethif0);
}

extern void
SAM_eth_interrupt (void)
{
   	NET_at91_emac_interrupt (&mac);
}
