/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

#include <ace/stddef.h>
#include <uso/mutex.h>
#include <dev/timer.h>

#include "net/debug.h"
#include "net/inet.h"
#include "net/arp.h"
#include "net/ethernet.h"
#include "net/ip.h"

#define ARP_TABLE_SIZE 10

#define ARP_MAXAGE 120          /* 120 * 10 seconds = 20 minutes. */

#define HWTYPE_ETHERNET 1

#define ARP_REQUEST 1
#define ARP_REPLY 2

/*
 * MUST be compiled with "pack structs" or equivalent! 
 */
struct arp_hdr
{
    struct NET_eth_hdr ethhdr;
    ACE_u16_t hwtype;
    ACE_u16_t proto;
    ACE_u16_t hwlen_protolen;
    ACE_u16_t opcode;
    struct NET_eth_addr shwaddr;
    NET_ip_addr_t sipaddr;
    struct NET_eth_addr dhwaddr;
    NET_ip_addr_t dipaddr;
} ACE_PACKED_;

#define ARPH_HWLEN(hdr) (ACE_ntohs((hdr)->hwlen_protolen) >> 8)
#define ARPH_PROTOLEN(hdr) (ACE_ntohs((hdr)->hwlen_protolen) & 0xff)


#define ARPH_HWLEN_SET(hdr, len) (hdr)->hwlen_protolen = ACE_htons(ARPH_PROTOLEN(hdr) | ((len) << 8))
#define ARPH_PROTOLEN_SET(hdr, len) (hdr)->hwlen_protolen = ACE_htons((len) | (ARPH_HWLEN(hdr) << 8))

struct ethip_hdr
{
    struct NET_eth_hdr eth;
    struct NET_ip_hdr ip;
} ACE_PACKED_;

struct arp_entry
{
    NET_ip_addr_t ipaddr;
    struct NET_eth_addr ethaddr;
    ACE_u8_t ctime;
};

static struct arp_entry arp_table[ARP_TABLE_SIZE];
static ACE_u8_t ctime;
static DEV_timer_t clear_entry;
static USO_mutex_t lock_arp_table;

/*
 * The arp_tmr() function should be called every ARP_TMR_INTERVAL
 * microseconds (10 seconds). This function is responsible for expiring
 * old entries in the ARP table. 
 */
static void
arp_tmr (void *nix)
{
    ACE_u8_t i;

    USO_lock (&lock_arp_table);
    ++ctime;
    for (i = 0; i < ARP_TABLE_SIZE; ++i)
    {
        if (!NET_ip_addr_isany (&arp_table[i].ipaddr) && ctime - arp_table[i].ctime >= ARP_MAXAGE)
        {
            DEBUGF (NET_ARP_DEBUG, ("Arp: timer expired entry %d.\n", i));
            NET_ip_addr_set (&(arp_table[i].ipaddr), &NET_ip_addr_any);
        }
    }
    USO_unlock (&lock_arp_table);
    DEV_timer_start (&clear_entry, ARP_TMR_INTERVAL);
}

extern void
NET_arp_init (void)
{
    ACE_u8_t i;

    for (i = 0; i < ARP_TABLE_SIZE; ++i)
    {
        NET_ip_addr_set (&(arp_table[i].ipaddr), &NET_ip_addr_any);
    }

    USO_mutex_init (&lock_arp_table);
    DEV_timer_init (&clear_entry, arp_tmr, NULL, DEV_TIMER_THREAD);
    DEV_timer_install (&clear_entry, "arp");
    DEV_timer_start (&clear_entry, ARP_TMR_INTERVAL);
}

static void
add_arp_entry (NET_ip_addr_t * ipaddr, struct NET_eth_addr *ethaddr)
{
    ACE_u8_t i, j, k;
    ACE_u8_t maxtime;

    USO_lock (&lock_arp_table);

    /* Walk through the ARP mapping table and try to find an entry to
     * update. If none is found, the IP -> MAC address mapping is inserted 
     * in the ARP table. 
     */
    for (i = 0; i < ARP_TABLE_SIZE; ++i)
    {

        /* Only check those entries that are actually in use. */
        if (!NET_ip_addr_isany (&arp_table[i].ipaddr))
        {
            /* Check if the source IP address of the incoming packet
             * matches the IP address in this ARP table entry. 
             */
            if (NET_ip_addr_cmp (ipaddr, &arp_table[i].ipaddr))
            {
                /* An old entry found, update this and return. */
                for (k = 0; k < NET_ETH_ADDR_SIZE; ++k)
                {
                    arp_table[i].ethaddr.addr[k] = ethaddr->addr[k];
                }
                arp_table[i].ctime = ctime;
                USO_unlock (&lock_arp_table);
                return;
            }
        }
    }

    /*If we get here, no existing ARP table entry was found, so we create one. */

    /* First, we try to find an unused entry in the ARP table. */
    for (i = 0; i < ARP_TABLE_SIZE; ++i)
    {
        if (NET_ip_addr_isany (&arp_table[i].ipaddr))
        {
            break;
        }
    }

    /* If no unused entry is found, we try to find the oldest entry and throw it away. */
    if (i == ARP_TABLE_SIZE)
    {
        maxtime = 0;
        j = 0;
        for (i = 0; i < ARP_TABLE_SIZE; ++i)
        {
            if (ctime - arp_table[i].ctime > maxtime)
            {
                maxtime = ctime - arp_table[i].ctime;
                j = i;
            }
        }
        i = j;
    }

    /* Now, i is the ARP table entry which we will fill with the new information. */
    NET_ip_addr_set (&arp_table[i].ipaddr, ipaddr);
    for (k = 0; k < NET_ETH_ADDR_SIZE; ++k)
    {
        arp_table[i].ethaddr.addr[k] = ethaddr->addr[k];
    }
    arp_table[i].ctime = ctime;

    USO_unlock (&lock_arp_table);
    return;
}

extern void
NET_arp_ip_input (NET_netif_t * netif, NET_netbuf_t * p)
{
    struct ethip_hdr *hdr;

    if (NET_netbuf_len (p) < sizeof (struct ethip_hdr))
    {
        DEBUGF (NET_ARP_DEBUG,
                ("Arp: ip packet too short (%d/%d).\n",
                 NET_netbuf_len (p), sizeof (struct ethip_hdr)));
        NET_netbuf_free (p);
        return;
    }

    hdr = (struct ethip_hdr *)NET_netbuf_index (p);

    /* Only insert/update an entry if the source IP address of the
     * incoming IP packet comes from a host on the local network.
     */
    if (!NET_ip_addr_maskcmp (&(hdr->ip.src), &(netif->ip_addr), &(netif->netmask)))
    {
        return;
    }
    DEBUGF (NET_ARP_DEBUG, ("Arp: ip updating arp table.\n"));
    add_arp_entry (&(hdr->ip.src), &(hdr->eth.src));
}

extern NET_netbuf_t *
NET_arp_arp_input (NET_netif_t * netif, struct NET_eth_addr *ethaddr, NET_netbuf_t * p)
{
    struct arp_hdr *hdr;
    ACE_u8_t i;

    if (NET_netbuf_len (p) < sizeof (struct arp_hdr))
    {
        DEBUGF (NET_ARP_DEBUG,
                ("Arp: arp packet too short (%d/%d).\n",
                 NET_netbuf_len (p), sizeof (struct arp_hdr)));
        NET_netbuf_free (p);
        return NULL;
    }

    hdr = (struct arp_hdr *)NET_netbuf_index (p);

    switch (ACE_htons (hdr->opcode))
    {
    case ARP_REQUEST:
        /* ARP request. If it asked for our address, we send out a reply. */
        DEBUGF (NET_ARP_DEBUG, ("Arp: arp request.\n"));
        if (NET_ip_addr_cmp (&(hdr->dipaddr), &(netif->ip_addr)))
        {
            hdr->opcode = ACE_htons (ARP_REPLY);

            NET_ip_addr_set (&(hdr->dipaddr), &(hdr->sipaddr));
            NET_ip_addr_set (&(hdr->sipaddr), &(netif->ip_addr));

            for (i = 0; i < NET_ETH_ADDR_SIZE; ++i)
            {
                hdr->dhwaddr.addr[i] = hdr->shwaddr.addr[i];
                hdr->shwaddr.addr[i] = ethaddr->addr[i];
                hdr->ethhdr.dest.addr[i] = hdr->dhwaddr.addr[i];
                hdr->ethhdr.src.addr[i] = ethaddr->addr[i];
            }

            hdr->hwtype = ACE_htons (HWTYPE_ETHERNET);
            ARPH_HWLEN_SET (hdr, 6);

            hdr->proto = ACE_htons (NET_ETH_TYPE_IP);
            ARPH_PROTOLEN_SET (hdr, sizeof (NET_ip_addr_t));

            hdr->ethhdr.type = ACE_htons (NET_ETH_TYPE_ARP);
            return p;
        }
        break;
    case ARP_REPLY:
        /* ARP reply. We insert or update the ARP table. */
        DEBUGF (NET_ARP_DEBUG, ("Arp: arp reply.\n"));
        if (NET_ip_addr_cmp (&(hdr->dipaddr), &(netif->ip_addr)))
        {
            add_arp_entry (&(hdr->sipaddr), &(hdr->shwaddr));
        }
        break;
    default:
        DEBUGF (NET_ARP_DEBUG, ("Arp: unknown type %d.\n", ACE_htons (hdr->opcode)));
        break;
    }

    NET_netbuf_free (p);
    return NULL;
}

extern struct NET_eth_addr *
NET_arp_lookup (NET_ip_addr_t * ipaddr)
{
    ACE_u8_t i;

    USO_lock (&lock_arp_table);

    for (i = 0; i < ARP_TABLE_SIZE; ++i)
    {
        if (NET_ip_addr_cmp (ipaddr, &arp_table[i].ipaddr))
        {
            USO_unlock (&lock_arp_table);
            return &arp_table[i].ethaddr;
        }
    }
    USO_unlock (&lock_arp_table);
    return NULL;
}

extern NET_netbuf_t *
NET_arp_query (NET_netif_t * netif, struct NET_eth_addr *ethaddr, NET_ip_addr_t * ipaddr)
{
    struct arp_hdr *hdr;
    NET_netbuf_t *p;
    ACE_u8_t i;

    p = NET_netbuf_alloc_trans ();
    if (p == NULL)
    {
        return NULL;
    }
    if (NET_netbuf_index_inc (p, -sizeof (struct arp_hdr)))
    {
        hdr = (struct arp_hdr *)NET_netbuf_index (p);
        hdr->opcode = ACE_htons (ARP_REQUEST);
        for (i = 0; i < NET_ETH_ADDR_SIZE; ++i)
        {
            hdr->dhwaddr.addr[i] = 0x00;
            hdr->shwaddr.addr[i] = ethaddr->addr[i];
        }
        NET_ip_addr_set (&(hdr->dipaddr), ipaddr);
        NET_ip_addr_set (&(hdr->sipaddr), &(netif->ip_addr));
        hdr->hwtype = ACE_htons (HWTYPE_ETHERNET);
        ARPH_HWLEN_SET (hdr, NET_ETH_ADDR_SIZE);
        hdr->proto = ACE_htons (NET_ETH_TYPE_IP);
        ARPH_PROTOLEN_SET (hdr, sizeof (NET_ip_addr_t));

        for (i = 0; i < NET_ETH_ADDR_SIZE; ++i)
        {
            hdr->ethhdr.dest.addr[i] = 0xff;
            hdr->ethhdr.src.addr[i] = ethaddr->addr[i];
        }

        hdr->ethhdr.type = ACE_htons (NET_ETH_TYPE_ARP);
    }
    else
    {
        NET_netbuf_free (p);
        p = NULL;
    }
    return p;
}
