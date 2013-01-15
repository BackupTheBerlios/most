/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

#include <ace/stddef.h>
#include <dev/cpu.h>
#include <dev/chips/smc_91c94.h>
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
#include "init/config.h"

#define SMC_IOADDR 0xA00000L

NET_netif_t MDC_lo;
NET_netif_t MDC_eth0;

static NET_ip_addr_t ip_lo;
static NET_ip_addr_t netmask_lo;

static NET_ethif_t ethif0;
static DEV_smc91c94_t smc;

static void
eth_reset (void)
{
    DEV_digout_set (&MDC_eth_reset);
    DEV_cpudelay (DEV_USEC_2_LOOPS (100));
    DEV_digout_clear (&MDC_eth_reset);
    DEV_cpudelay (DEV_USEC_2_LOOPS (1000));
}

extern void
MDC_eth_init (void)
{
    NET_stats_init ();
    NET_netbuf_init ();
    NET_eth_init ();
    NET_ip_init ();
    NET_udp_init ();

    NET_netif_init (&MDC_lo, "lo");
    NET_ip4_addr (&ip_lo, 127, 0, 0, 1);
    NET_ip4_addr (&netmask_lo, 255, 0, 0, 0);
    NET_netif_set_ipaddr (&MDC_lo, &ip_lo);
    NET_netif_set_netmask (&MDC_lo, &netmask_lo);
    NET_loopif_init (&MDC_lo);

    NET_netif_init (&MDC_eth0, "eth0");
    MDC_config_ip ();
    NET_ethif_init (&MDC_eth0, &ethif0, &MDC_config.eth_addr, "eth0");

    NET_netif_set_default (&MDC_eth0);
    eth_reset ();
    DEV_smc_init (&ethif0, &smc, SMC_IOADDR);
}

extern void
MDC_eth_start (void)
{
    NET_ethif_start (&ethif0);
}

extern ACE_INTERRUPT_ void
MDC_IRQ7_ISR (void) ACE_SECTION_ (".unref");

extern ACE_INTERRUPT_ void
MDC_IRQ7_ISR (void)
{
    DEV_smc_interrupt (&smc);
}
