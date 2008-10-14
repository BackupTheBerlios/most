/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

#include <ace/stddef.h>
#include <ace/string.h>
#include <uso/buf_pool.h>
#include <uso/list.h>

#include <mfs/descriptor.h>
#include <mfs/directory.h>
#include <mfs/sysfs.h>

#include "net/debug.h"
#include "net/ip.h"
#include "net/inet.h"
#include "net/netif.h"
#include "net/icmp.h"
#include "net/udp.h"
// #include "net/tcp.h"
#include "net/stats.h"
#include "net/opt.h"

#if NET_IP_HEAD_DEBUG
#define IP_HEAD_DEBUG(iphdr)	ip_head_debug (iphdr)
static void
ip_head_debug (struct NET_ip_hdr *iphdr)
{
    DEBUGF (NET_IP_HEAD_DEBUG, ("IP header:\n"));
    DEBUGF (NET_IP_HEAD_DEBUG, ("+-------------------------------+\n"));
    DEBUGF (NET_IP_DEBUG,
            ("|%2d |%2d |   %2d  |      %4d     | (v, hl, tos, len)\n",
             NET_IPH_V (iphdr), NET_IPH_HL (iphdr), NET_IPH_TOS (iphdr),
             ntohs (NET_IPH_LEN (iphdr))));
    DEBUGF (NET_IP_HEAD_DEBUG, ("+-------------------------------+\n"));
    DEBUGF (NET_IP_HEAD_DEBUG,
            ("|    %5d      |%d%d%d|    %4d   | (id, flags, offset)\n",
             ntohs (NET_IPH_ID (iphdr)),
             ntohs (NET_IPH_OFFSET (iphdr)) >> 15 & 1,
             ntohs (NET_IPH_OFFSET (iphdr)) >> 14 & 1,
             ntohs (NET_IPH_OFFSET (iphdr)) >> 13 & 1,
             ntohs (NET_IPH_OFFSET (iphdr)) & NET_IP_OFFMASK));
    DEBUGF (NET_IP_HEAD_DEBUG, ("+-------------------------------+\n"));
    DEBUGF (NET_IP_HEAD_DEBUG,
            ("|   %2d  |   %2d  |    0x%04x     | (ttl, proto, chksum)\n",
             NET_IPH_TTL (iphdr), NET_IPH_PROTO (iphdr),
             ntohs (NET_IPH_CHKSUM (iphdr))));
    DEBUGF (NET_IP_HEAD_DEBUG, ("+-------------------------------+\n"));
    DEBUGF (NET_IP_HEAD_DEBUG,
            ("|  %3ld  |  %3ld  |  %3ld  |  %3ld  | (src)\n",
             ntohl (iphdr->src.addr) >> 24 & 0xff,
             ntohl (iphdr->src.addr) >> 16 & 0xff,
             ntohl (iphdr->src.addr) >> 8 & 0xff,
             ntohl (iphdr->src.addr) & 0xff));
    DEBUGF (NET_IP_HEAD_DEBUG, ("+-------------------------------+\n"));
    DEBUGF (NET_IP_HEAD_DEBUG,
            ("|  %3ld  |  %3ld  |  %3ld  |  %3ld  | (dest)\n",
             ntohl (iphdr->dest.addr) >> 24 & 0xff,
             ntohl (iphdr->dest.addr) >> 16 & 0xff,
             ntohl (iphdr->dest.addr) >> 8 & 0xff,
             ntohl (iphdr->dest.addr) & 0xff));
    DEBUGF (NET_IP_HEAD_DEBUG, ("+-------------------------------+\n"));
}
#else
#define IP_HEAD_DEBUG(iphdr)
#endif

/*-----------------------------------------------------------------------------------*/
/*
 * ip_init: Initializes the IP layer. 
 */
/*-----------------------------------------------------------------------------------*/

static MFS_descriptor_t *net_interfaces;
//static NET_netif_t *netif_default = NULL;

extern void
NET_ip_init (void)
{
    net_interfaces = MFS_sysfs_netif();
}

static NET_netif_t *
find_route(NET_ip_addr_t *dest)
{
	MFS_descriptor_t *desc = NULL;
	NET_netif_t *netif = NULL;
	while ( (desc = MFS_next_entry(net_interfaces, desc)) != NULL) {
		netif = (NET_netif_t *)desc->entry;
    	if (NET_ip_addr_maskcmp (dest, &(netif->ip_addr), &(netif->netmask)))
	    {
   			return netif;	   	
    	}
  	}    
	return NULL;
}

/*-----------------------------------------------------------------------------------*/
/*
 * ip_route: Finds the appropriate network interface for a given IP
 * address. It searches the list of network interfaces linearly. A match
 * is found if the masked IP address of the network interface equals the
 * masked IP address given to the function. 
 */
/*-----------------------------------------------------------------------------------*/

extern NET_netif_t *
NET_ip_route (NET_ip_addr_t * dest)
{
    NET_netif_t *netif;

    netif = find_route(dest);

    if (netif != NULL)
        return netif;
    else
    	return NET_netif_default;
}

static NET_netif_t *
find_netif(struct NET_ip_hdr *iphdr)
{
	MFS_descriptor_t *desc = NULL;
	NET_netif_t *netif = NULL;
	while ( (desc = MFS_next_entry(net_interfaces, desc)) != NULL) {
		netif = (NET_netif_t *)desc->entry;
	    if (NET_ip_addr_isany (&(netif->ip_addr)) ||
    	    NET_ip_addr_cmp (&(iphdr->dest), &(netif->ip_addr)) ||
        	(NET_ip_addr_isbroadcast (&(iphdr->dest), &(netif->netmask)) &&
        	 NET_ip_addr_maskcmp (&(iphdr->dest), &(netif->ip_addr), &(netif->netmask))) ||
        	NET_ip_addr_cmp (&(iphdr->dest), &NET_ip_addr_broadcast))
	    {
   			break;	   	
    	}
  	}    
	return netif;
}


/*-----------------------------------------------------------------------------------*/
/*
 * ip_input: This function is called by the network interface device
 * driver when an IP packet is received. The function does the basic
 * checks of the IP header such as packet size being at least larger than
 * the header size etc. If the packet was not destined for us, the packet
 * is forwarded (using ip_forward). The IP checksum is always checked.
 * Finally, the packet is sent to the upper layer protocol input function. 
 */
/*-----------------------------------------------------------------------------------*/

extern NET_err_t
NET_ip_input (NET_netif_t * inp, NET_netbuf_t * p)
{
    static struct NET_ip_hdr *iphdr;
    static NET_netif_t *netif;
    static u8_t hl;

    ++NET_stats.ip.rx;
    iphdr = (struct NET_ip_hdr *)NET_netbuf_index(p);
    DEBUGF (NET_IP_DEBUG, ("Ip: rx len %d.\n", NET_netbuf_tot_len(p)));
    IP_HEAD_DEBUG (iphdr);

    /*
     * identify the IP header 
     */
    if (NET_IPH_V (iphdr) != 4)
    {
        DEBUGF (NET_IP_DEBUG,
                ("Ip: packet dropped due to bad version number %d.\n",
                 NET_IPH_V (iphdr)));
        ++NET_stats.ip.rx_drop;
        NET_netbuf_free (p);
        return NET_ERR_BAD;
    }

    hl = NET_IPH_HL (iphdr);

    if (hl * 4 > NET_netbuf_len(p))
    {
        DEBUGF (NET_IP_DEBUG,
                ("Ip: packet dropped due to too short packet %d.\n", NET_netbuf_len(p)));
        ++NET_stats.ip.rx_drop;
        NET_netbuf_free (p);
        return NET_ERR_BAD;
    }

    /*
     * verify checksum 
     */
    if (NET_inet_chksum (iphdr, hl * 4) != 0)
    {
        DEBUGF (NET_IP_DEBUG,
                ("Ip: packet dropped due to failing checksum 0x%x.\n",
                 NET_inet_chksum (iphdr, hl * 4)));
        ++NET_stats.ip.rx_drop;
        NET_netbuf_free (p);
        return NET_ERR_BAD;
    }

    netif = find_netif (iphdr);

    if (netif == NULL)
    {
        /*
         * packet not for us, route or discard 
         */
        DEBUGF (NET_IP_DEBUG, ("Ip: packet not for us.\n"));

        if (!NET_ip_addr_isbroadcast (&(iphdr->dest), &(inp->netmask)))
        {
        	//ip_forward (p, iphdr, inp);
        }
        NET_netbuf_free (p);
        return NET_ERR_BAD;
    }

    if ((NET_IPH_OFFSET (iphdr) & htons (NET_IP_OFFMASK | NET_IP_MF)) != 0)
    {
        DEBUGF (NET_IP_DEBUG,
                ("Ip: packet dropped since it was fragmented (0x%x).\n",
                 ntohs (NET_IPH_OFFSET (iphdr))));
        ++NET_stats.ip.rx_drop;
        NET_netbuf_free (p);
        return NET_ERR_BAD;
    }

    if (hl * 4 > NET_IP_HLEN)
    {
        DEBUGF (NET_IP_DEBUG,
                ("Ip: packet dropped since there were IP options.\n"));
        ++NET_stats.ip.rx_drop;
        NET_netbuf_free (p);
        return NET_ERR_BAD;
    }

    /*
     * send to upper layers 
     */
    switch (NET_IPH_PROTO (iphdr))
    {
    case NET_IP_PROTO_UDP:
        return NET_udp_input (inp, p);
    case NET_IP_PROTO_TCP:
        // tcp_input(p, inp);
        DEBUGF (NET_IP_DEBUG, ("Ip: tcp not supported.\n"));
        break;
    case NET_IP_PROTO_ICMP:
        return NET_icmp_input (p, inp);
    default:
        /*
         * send ICMP destination protocol unreachable unless it was a
         * broadcast 
         */
        if (!NET_ip_addr_isbroadcast (&(iphdr->dest), &(inp->netmask)) &&
            !NET_ip_addr_ismulticast (&(iphdr->dest)))
        {
            NET_icmp_dest_unreach (p, NET_ICMP_DUR_PROTO);
        }
        DEBUGF (NET_IP_DEBUG,
                ("Ip: unsupported transportation protocol %d.\n",
                 NET_IPH_PROTO (iphdr)));

        ++NET_stats.ip.rx_drop;
        break;
    }
	NET_netbuf_free (p);
    return NET_ERR_BAD;
}

/*-----------------------------------------------------------------------------------*/
/*
 * ip_output_if: Sends an IP packet on a network interface. This function 
 * constructs the IP header and calculates the IP header checksum. If the
 * source IP address is NULL, the IP address of the outgoing network
 * interface is filled in as source address. 
 */
/*-----------------------------------------------------------------------------------*/

extern NET_err_t
NET_ip_output_if (NET_netbuf_t * p,
                  NET_ip_addr_t * src,
                  NET_ip_addr_t * dest,
                  u8_t ttl, u8_t proto, NET_netif_t * netif)
{
    static struct NET_ip_hdr *iphdr;
    static u16_t ip_id = 0;

    if (dest != NET_IP_HDRINCL)
    {
        NET_netbuf_index_inc (p, -sizeof (struct NET_ip_hdr));
        iphdr = (struct NET_ip_hdr *)NET_netbuf_index(p);
		
        NET_IPH_TTL_SET (iphdr, ttl);
        NET_IPH_PROTO_SET (iphdr, proto);

        NET_ip_addr_set (&(iphdr->dest), dest);

        NET_IPH_VHLTOS_SET (iphdr, 4, NET_IP_HLEN / 4, 0);
        NET_IPH_LEN_SET (iphdr, htons ((unsigned short)NET_netbuf_tot_len (p)));
        NET_IPH_OFFSET_SET (iphdr, htons (NET_IP_DF));
        NET_IPH_ID_SET (iphdr, htons (++ip_id));

        if (NET_ip_addr_isany (src))
        {
            NET_ip_addr_set (&(iphdr->src), &(netif->ip_addr));
        }
        else
        {
            NET_ip_addr_set (&(iphdr->src), src);
        }

        NET_IPH_CHKSUM_SET (iphdr, 0);
        NET_IPH_CHKSUM_SET (iphdr, NET_inet_chksum (iphdr, NET_IP_HLEN));
    }
    else
    {
        iphdr = (struct NET_ip_hdr *)NET_netbuf_index(p);
        dest = &(iphdr->dest);
    }
    ++NET_stats.ip.tx;
   // DEBUGF (NET_IP_DEBUG, ("Ip: output if %s.\n", netif->name));
    DEBUGF (NET_IP_DEBUG, ("Ip: tx len %d.\n", NET_netbuf_tot_len(p)));
    IP_HEAD_DEBUG (iphdr);
    return netif->output (netif, p, dest);
}

/*-----------------------------------------------------------------------------------*/
/*
 * ip_output: Simple interface to ip_output_if. It finds the outgoing
 * network interface and calls upon ip_output_if to do the actual work. 
 */
/*-----------------------------------------------------------------------------------*/

NET_err_t
NET_ip_output (NET_netbuf_t * p,
               NET_ip_addr_t * src, NET_ip_addr_t * dest, u8_t ttl, u8_t proto)
{
    static NET_netif_t *netif;

    if ((netif = NET_ip_route (dest)) == NULL)
    {
        DEBUGF (NET_IP_DEBUG, ("Ip: no route to 0x%lx.\n", dest->addr));

        NET_netbuf_free (p);
        return NET_ERR_RTE;
    }

    return NET_ip_output_if (p, src, dest, ttl, proto, netif);
}
