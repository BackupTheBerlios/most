/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

/*
 * Some ICMP messages should be passed to the transport protocols. This is 
 * not implemented. 
 */

#include <ace/stddef.h>
#include "ace/string.h"

#include "net/debug.h"
#include "net/icmp.h"
#include "net/inet.h"
#include "net/ip.h"
#include "net/stats.h"
#include "net/ethif.h"

/*-----------------------------------------------------------------------------------*/

extern NET_err_t
NET_icmp_input (NET_netbuf_t * p, NET_netif_t * inp)
{
    unsigned char type;
    struct NET_icmp_echo_hdr *iecho;
    struct NET_ip_hdr *iphdr;
    NET_ip_addr_t tmpaddr;
    long hlen;

    ++NET_stats.icmp.rx;
    DEBUGF (NET_ICMP_DEBUG, ("Icmp: rx.\n"));

    iphdr = (struct NET_ip_hdr *)NET_netbuf_index(p);
    hlen = NET_IPH_HL (iphdr) * 4 / sizeof (u8_t);
    NET_netbuf_index_inc (p, hlen);
    type = *((unsigned char *)NET_netbuf_index(p));

    switch (type)
    {
    case NET_ICMP_ECHO:
        DEBUGF (NET_ICMP_DEBUG, ("Icmp: echo.\n"));
        if (NET_ip_addr_isbroadcast (&iphdr->dest, &inp->netmask) ||
            NET_ip_addr_ismulticast (&iphdr->dest))
        {
            DEBUGF (NET_ICMP_DEBUG, ("Icmp: Smurf.\n"));
            ++NET_stats.icmp.rx_drop;
            NET_netbuf_free (p);
            return NET_ERR_BAD;
        }
        DEBUGF (NET_ICMP_DEBUG, ("Icmp: ping.\n"));
        if (NET_netbuf_tot_len (p) < sizeof (struct NET_icmp_echo_hdr))
        {
            DEBUGF (NET_ICMP_DEBUG, ("Icmp: bad ICMP echo received.\n"));
            ++NET_stats.icmp.rx_drop;
            NET_netbuf_free (p);
            return NET_ERR_BAD;
        }
        iecho = (struct NET_icmp_echo_hdr *)NET_netbuf_index(p);
        if (NET_inet_chksum_buf (p) != 0)
        {
            DEBUGF (NET_ICMP_DEBUG,
                    ("Icmp: checksum failed for received ICMP echo.\n"));
            ++NET_stats.icmp.rx_drop;
            NET_netbuf_free (p);
            return NET_ERR_BAD;
        }
        tmpaddr.addr = iphdr->src.addr;
        iphdr->src.addr = iphdr->dest.addr;
        iphdr->dest.addr = tmpaddr.addr;
        NET_ICMPH_TYPE_SET (iecho, NET_ICMP_ER);
        /*
         * adjust the checksum 
         */
        if (iecho->chksum >= htons (0xffff - (NET_ICMP_ECHO << 8)))
        {
            iecho->chksum += htons (NET_ICMP_ECHO << 8) + 1;
        }
        else
        {
            iecho->chksum += htons (NET_ICMP_ECHO << 8);
        }
        ++NET_stats.icmp.tx;

        NET_netbuf_index_inc (p, -hlen);
        NET_ip_output_if (p, &(iphdr->src), NET_IP_HDRINCL,
                          NET_IPH_TTL (iphdr), NET_IP_PROTO_ICMP, inp);
        return NET_ERR_OK;

    default:
        DEBUGF (NET_ICMP_DEBUG, ("Icmp: type not supported.\n"));
        ++NET_stats.icmp.rx_drop;
        NET_netbuf_free (p);
        return NET_ERR_BAD;
    }
}

extern void
NET_icmp_dest_unreach (NET_netbuf_t * p, enum NET_icmp_dur_type t)
{
    NET_netbuf_t *q;
    struct NET_ip_hdr *iphdr;
    struct NET_icmp_dur_hdr *idur;

    DEBUGF (NET_ICMP_DEBUG, ("Icmp: dest_unreach.\n"));

    q = NET_netbuf_alloc_trans ();
    if (NET_netbuf_index_inc (q, -(sizeof(struct NET_ip_hdr))+8+8) == FALSE){
	    DEBUGF (NET_ICMP_DEBUG, ("Icmp: trans buf size to small.\n"));
        NET_netbuf_free (q);
    } else {
    	iphdr = (struct NET_ip_hdr *)NET_netbuf_index(p);
    	idur = (struct NET_icmp_dur_hdr *)NET_netbuf_index(q);
    	NET_ICMPH_TYPE_SET (idur, NET_ICMP_DUR);
    	NET_ICMPH_CODE_SET (idur, t);
    	bcopy (iphdr, (char*)idur + 8, sizeof (struct NET_ip_hdr) + 8);
    	idur->chksum = 0;
    	idur->chksum = NET_inet_chksum (idur, NET_netbuf_len(q));
    	++NET_stats.icmp.tx;
		NET_ip_output (q, NULL, &(iphdr->src), NET_ICMP_TTL, NET_IP_PROTO_ICMP);
    }
    NET_netbuf_free (p);
}

#if 0
extern void
NET_icmp_time_exceeded (NET_netbuf_t * p, enum NET_icmp_te_type t)
{
    NET_netbuf_t *q;
    struct NET_ip_hdr *iphdr;
    struct NET_icmp_te_hdr *tehdr;

    q = NET_netbuf_alloc (&icmp_pool, 0, NULL);

    NET_netbuf_index_inc (q, sizeof (struct NET_eth_hdr) +
                          sizeof (struct NET_ip_hdr));

    iphdr = (struct NET_ip_hdr *)p->index;

#if NET_ICMP_DEBUG
    DEBUGF (NET_ICMP_DEBUG, ("Icmp:_time_exceeded from\n"));
    /*
     * ip_addr_debug_print(&(iphdr->src)); 
     */
    /*
     * DEBUGF(NET_ICMP_DEBUG, (" to ")); 
     */
    /*
     * ip_addr_debug_print(&(iphdr->dest)); 
     */
    /*
     * DEBUGF(NET_ICMP_DEBUG, ("\n")); 
     */
#endif /* ICMP_DEBNUG */

    tehdr = (struct NET_icmp_te_hdr *)q->index;
    NET_ICMPH_TYPE_SET (tehdr, NET_ICMP_TE);
    NET_ICMPH_CODE_SET (tehdr, t);

    /*
     * copy fields from original packet 
     */
    bcopy ((char *)p->index, (char *)q->index + 8,
           sizeof (struct NET_ip_hdr) + 8);

    /*
     * calculate checksum 
     */
    tehdr->chksum = 0;
    tehdr->chksum = NET_inet_chksum (tehdr, q->len);
    ++NET_stats.icmp.tx;
    NET_ip_output (q, NULL, &(iphdr->src), NET_ICMP_TTL, NET_IP_PROTO_ICMP);
    NET_netbuf_free (q);
}
#endif
