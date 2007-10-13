/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

#include <ace/stddef.h>
#include <dev/arch/cpu.h>
#include <net/arch/smc_91c94.h>
#include <net/netif.h>
#include <net/err.h>
#include <net/ip.h>
#include <net/loopif.h>
#include <net/icmp.h>
#include <net/udp.h>
#include <net/netbuf.h>

#include "arch/digio.h"
#include "arch/eth.h"
#include "init/config.h"

#define SMC_IOADDR 0xA00000L

NET_netif_t MDC_lo;
NET_netif_t MDC_eth0;

static NET_ip_addr_t local;
static NET_ip_addr_t netmask_lo;

static NET_ethif_t ethif0;
static NET_smc91c94_t smc;

static void
eth_reset (void)
{
    DEV_digout_set (&MDC_eth_reset);
    DEV_cpudelay (ACE_USEC_2_LOOPS(100));
    DEV_digout_clear (&MDC_eth_reset);
    DEV_cpudelay (ACE_USEC_2_LOOPS(1000));
}

extern void
MDC_eth_init (void)
{
    NET_ip4_addr (&local, 127, 0, 0, 1);
    NET_ip4_addr (&netmask_lo, 255, 0, 0, 0);
    NET_netbuf_init ();
    NET_netif_init (&MDC_lo, "lo", NET_ip_input);
	NET_netif_set_ipaddr (&MDC_lo, &local);
	NET_netif_set_netmask (&MDC_lo, &netmask_lo);
    NET_loopif_init (&MDC_lo);
    NET_netif_init (&MDC_eth0, "eth0", NET_ip_input);
	NET_ip_init();
    NET_udp_init ();
    NET_smc_init (&smc, MDC_get_eth0_mac_addr (), SMC_IOADDR);
    NET_ethif_init (&ethif0, &MDC_eth0, MDC_get_eth0_mac_addr (), (NET_ethdev_t *) &smc);
    eth_reset ();
    ((NET_ethdev_t *) & smc)->start ((NET_ethdev_t *) & smc);
}

extern void
MDC_eth_start (void)
{
    NET_ethif_start (&MDC_eth0);
}

extern _INTERRUPT_
     void MDC_IRQ7_ISR (void)
{
    ((NET_ethdev_t *) & smc)->interrupt ((NET_ethdev_t *) & smc);
}
