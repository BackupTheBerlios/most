/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 *  Problems:
 *  
 *  checksum calculation for odd byte fails
 *  -> checksum not calculated
 */

#include <ace/stddef.h>
#include <ace/string.h>
#include <ace/stdlib.h>

#include "net/debug.h"
#include "net/ethif.h"
#include "net/netif.h"
#include "net/inet.h"
#include "net/udp.h"
#include "net/udp_socks.h"
#include "net/opt.h"
#include "net/icmp.h"
#include "net/stats.h"

extern void
NET_udp_init (void)
{
    NET_udp_socks_init();
}

#if NET_UDP_HEAD_DEBUG
#define UDP_HEAD_DEBUG(udphdr)	udp_head_debug (udphdr)
static void
udp_head_debug (struct NET_udp_hdr *udphdr)
{
    DEBUGF (NET_UDP_HEAD_DEBUG, ("UDP header:\n"));
    DEBUGF (NET_UDP_HEAD_DEBUG, ("+-------------------------------+\n"));
    DEBUGF (NET_UDP_HEAD_DEBUG,
            ("|     %5d     |     %5d     | (src port, dest port)\n",
             ntohs (udphdr->src), ntohs (udphdr->dest)));
    DEBUGF (NET_UDP_HEAD_DEBUG, ("+-------------------------------+\n"));
    DEBUGF (NET_UDP_HEAD_DEBUG,
            ("|     %5d     |     0x%04x    | (len, chksum)\n",
             ntohs (udphdr->len), ntohs (udphdr->chksum)));
    DEBUGF (NET_UDP_HEAD_DEBUG, ("+-------------------------------+\n"));
}
#else
#define UDP_HEAD_DEBUG(udphdr)
#endif

extern NET_err_t
NET_udp_input (NET_netif_t * inp, NET_netbuf_t * p)
{
    struct NET_ip_hdr *iphdr;
    struct NET_udp_hdr *udphdr;
    NET_udp_socket_t *sock = NULL;
    u16_t src, dest;

    ++NET_stats.udp.rx;

    iphdr = (struct NET_ip_hdr *)NET_netbuf_index(p);
    NET_netbuf_index_inc (p, sizeof (struct NET_ip_hdr));
    udphdr = (struct NET_udp_hdr *)NET_netbuf_index(p);

    DEBUGF (NET_UDP_DEBUG, ("Udp: rx datagram of length %d.\n",
                            NET_netbuf_len(p) - sizeof (struct NET_udp_hdr)));

    src = ntohs (udphdr->src);
    dest = ntohs (udphdr->dest);

    UDP_HEAD_DEBUG (udphdr);

	NET_udp_socks_lock ();
		
 	sock = NET_udp_find_sock(inp, iphdr, src, dest);
 
    if (sock == NULL)
    {
		NET_udp_socks_unlock ();
        DEBUGF (NET_UDP_DEBUG, ("Udp: not for us.\n"));

        /*
         * No match was found, send ICMP destination port unreachable
         * unless destination address was broadcast/multicast. 
         */

        if (!NET_ip_addr_isbroadcast (&iphdr->dest, &inp->netmask) &&
            !NET_ip_addr_ismulticast (&iphdr->dest))
        {

            /*
             * deconvert from host to network byte order 
             */
            udphdr->src = htons (udphdr->src);
            udphdr->dest = htons (udphdr->dest);

            /*
             * adjust pbuf pointer 
             */
            NET_netbuf_index_inc (p, -sizeof (struct NET_ip_hdr));
            NET_icmp_dest_unreach (p, NET_ICMP_DUR_PORT);
        } else {
			NET_netbuf_free (p);
        }
        ++NET_stats.udp.rx_drop;
        return NET_ERR_BAD;
    }

	DEBUGF (NET_UDP_DEBUG, ("Udp: rx calculating checksum.\n"));

    // ip unterstuezt nur udp ??? 
    if (NET_IPH_PROTO (iphdr) == NET_IP_PROTO_UDPLITE)
	{
        if (NET_inet_chksum_pseudo
            (p, (NET_ip_addr_t *) & (iphdr->src),
             (NET_ip_addr_t *) & (iphdr->dest), NET_IP_PROTO_UDPLITE,
             ntohs (udphdr->len)) != 0)
        {
            DEBUGF (NET_UDP_DEBUG,
                    ("Udp: udp lite datagram discarded due to failing checksum.\n"));
            ++NET_stats.udp.rx_drop;
	        NET_netbuf_free (p);
			NET_udp_socks_unlock ();
            return NET_ERR_BAD;
        }
    }
    else
    {
        if (udphdr->chksum != 0)
        {
            if (NET_inet_chksum_pseudo
                (p, (NET_ip_addr_t *) & (iphdr->src),
                 (NET_ip_addr_t *) & (iphdr->dest),
                 NET_IP_PROTO_UDP, NET_netbuf_len(p)) != 0)
            {
                DEBUGF (NET_UDP_DEBUG,
                        ("Udp: udp datagram discarded due to failing checksum.\n"));

                ++NET_stats.udp.rx_drop;
                NET_netbuf_free (p);
				NET_udp_socks_unlock ();
				return NET_ERR_BAD;
            }
        }
    }
    USO_post (&sock->rx_que, (USO_node_t *) p);
    NET_udp_socks_unlock ();
	return NET_ERR_OK;
}

NET_err_t
NET_udp_output (NET_udp_socket_t * sock, NET_netbuf_t * p)
{
    struct NET_udp_hdr *udphdr;
    NET_netif_t *netif;
    NET_ip_addr_t *src_ip;
    NET_err_t err;
    short tot_len;

    NET_netbuf_index_inc (p, -sizeof(struct NET_udp_hdr));

    tot_len = (unsigned short)NET_netbuf_tot_len (p);

    udphdr = (struct NET_udp_hdr *)NET_netbuf_index(p);
    udphdr->src = htons (sock->local_port);
    udphdr->dest = htons (sock->remote_port);
    udphdr->chksum = 0x0000;

    if ((netif = NET_ip_route (&(sock->remote_ip))) == NULL)
    {
        DEBUGF (NET_UDP_DEBUG,
                ("Udp: no route to 0x%lx.\n", sock->remote_ip.addr));
		NET_netbuf_free (p);
        return NET_ERR_RTE;
    }

    if (NET_ip_addr_isany (&sock->local_ip))
    {
        src_ip = &(netif->ip_addr);
    }
    else
    {
        src_ip = &(sock->local_ip);
    }

    DEBUGF (NET_UDP_DEBUG,
            ("Udp: tx datagram of length %d.\n",
             tot_len - sizeof (struct NET_udp_hdr)));

    if (sock->flags & NET_UDP_FLAGS_UDPLITE)
    {
        udphdr->len = htons (sock->chksum_len);
        /*
         * calculate checksum 
         */
        udphdr->chksum = 0x0000;
        
        udphdr->chksum = NET_inet_chksum_pseudo (p, src_ip, &(sock->remote_ip),
                                    NET_IP_PROTO_UDP, sock->chksum_len);
        if (udphdr->chksum == 0x0000)
        {
            udphdr->chksum = 0xffff;
        }

        UDP_HEAD_DEBUG (udphdr);

        err = NET_ip_output_if (p, src_ip, &sock->remote_ip, NET_UDP_TTL,
                                NET_IP_PROTO_UDPLITE, netif);
    }
    else
    {
        udphdr->len = htons (tot_len);
        /*
         * calculate checksum 
         */
        if ((sock->flags & NET_UDP_FLAGS_NOCHKSUM) == 0)
        {
            udphdr->chksum = 0x0000;
            
            udphdr->chksum = NET_inet_chksum_pseudo (p, src_ip, &sock->remote_ip,
                                        NET_IP_PROTO_UDP, tot_len);
            if (udphdr->chksum == 0x0000)
            {
                udphdr->chksum = 0xffff;
            }
        }

        UDP_HEAD_DEBUG (udphdr);

        err = NET_ip_output_if (p, src_ip, &sock->remote_ip,
                                NET_UDP_TTL, NET_IP_PROTO_UDP, netif);
    }

    ++NET_stats.udp.tx;
    return err;
}
