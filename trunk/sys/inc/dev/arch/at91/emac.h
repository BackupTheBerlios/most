#ifndef DEV_AT91_EMAC_H
#define DEV_AT91_EMAC_H

#include <uso/semaphore.h>
#include <uso/mutex.h>
#include "net/ethif.h"

struct DEV_at91_emac
{
    unsigned char mac_addr[NET_ETH_ADDR_SIZE];
    unsigned long rx_packets;
    unsigned short rx_overruns;
    unsigned short rx_dropped;
    unsigned short rx_badlen;
    unsigned long tx_packets;
    unsigned short tx_alloc_failed;
    unsigned short link_down_count;
    ACE_bool_t link_is_up;
    USO_semaphore_t rx_sem;
    USO_mutex_t lock;
};

typedef struct DEV_at91_emac DEV_at91_emac_t;


extern void DEV_at91_emac_interrupt(DEV_at91_emac_t* mac);

extern void
DEV_at91_emac_init (NET_ethif_t* ethif,
		     	  	DEV_at91_emac_t* mac);

#endif
