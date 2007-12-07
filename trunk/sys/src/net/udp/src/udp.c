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
#include "net/opt.h"
#include "net/icmp.h"
#include "net/stats.h"

extern void
NET_udp_init (void)
{
    NET_udp_sock_init();
}

#if NET_UDP_HEAD_DEBUG
static void
udp_head_debug (struct NET_udp_hdr *udphdr)
{
    DEBUGF (NET_UDP_HEAD_DEBUG, ("\nUDP header:\n"));
    DEBUGF (NET_UDP_HEAD_DEBUG, ("+-------------------------------+\n"));
    DEBUGF (NET_UDP_HEAD_DEBUG,
            ("|     %5d     |     %5d     | (src port, dest port)\n",
             ntohs (udphdr->src), ntohs (udphdr->dest)));
    DEBUGF (NET_UDP_HEAD_DEBUG, ("+-------------------------------+\n"));
    DEBUGF (NET_UDP_HEAD_DEBUG,
            ("|     %5d     |     0x%04x    | (len, chksum)\n",
             ntohs (udphdr->len), ntohs (udphdr->chksum)));
    DEBUGF (NET_UDP_HEAD_DEBUG, ("+-------------------------------+"));
}
#endif

extern NET_err_t
NET_udp_input (NET_netif_t * inp, NET_netbuf_t * p)
{
    struct NET_ip_hdr *iphdr;
    struct NET_udp_hdr *udphdr;
    NET_udp_socket_t *sock = NULL;
    u16_t src, dest;

    ++NET_stats.udp.rx;

    iphdr = (struct NET_ip_hdr *)p->index;
    NET_netbuf_index_inc (p, sizeof (struct NET_ip_hdr));
    udphdr = (struct NET_udp_hdr *)p->index;

    DEBUGF (NET_UDP_DEBUG, ("\nUdp: rx datagram of length %d.",
                            p->len - sizeof (struct NET_udp_hdr)));

    src = ntohs (udphdr->src);
    dest = ntohs (udphdr->dest);

#if NET_UDP_HEAD_DEBUG_RX
    udp_head_debug (udphdr);
#endif

	sock = NET_find_exact_sock(inp, iphdr, src, dest);
 	if (sock == NULL)
    {
    	sock = NET_find_sock(iphdr, src, dest);
    }
 
    if (sock != NULL)
    {
        DEBUGF (NET_UDP_DEBUG, ("\nUdp: rx calculating checksum."));

        // ip unterstuezt nur udp ??? 
        if (NET_IPH_PROTO (iphdr) == NET_IP_PROTO_UDPLITE)
        {
        	
#if 1        	
            if (NET_inet_chksum_pseudo
                (p, (NET_ip_addr_t *) & (iphdr->src),
                 (NET_ip_addr_t *) & (iphdr->dest), NET_IP_PROTO_UDPLITE,
                 ntohs (udphdr->len)) != 0)
            {
                DEBUGF (NET_UDP_DEBUG,
                        ("\nUdp: udp lite datagram discarded due to failing checksum."));
                ++NET_stats.udp.rx_drop;
                return NET_ERR_BAD;
            }
#endif            
            
        }
        else
        {
            if (udphdr->chksum != 0)
            {
 
 #if 0
                if (NET_inet_chksum_pseudo
                    (p, (NET_ip_addr_t *) & (iphdr->src),
                     (NET_ip_addr_t *) & (iphdr->dest),
                     NET_IP_PROTO_UDP, p->len) != 0)
                {
                    DEBUGF (NET_UDP_DEBUG,
                            ("\nUdp: udp datagram discarded due to failing checksum."));

                    ++stats.udp.rx_drop;
                    return NET_ERR_BAD;
                }
#endif

            }
        }
        USO_post (&sock->rx_que, (USO_node_t *) p);
    }
    else
    {
        DEBUGF (NET_UDP_DEBUG, ("\nUdp: not for us."));

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
        }

        ++NET_stats.udp.rx_drop;
        return NET_ERR_BAD;
    }

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

    udphdr = (struct NET_udp_hdr *)p->index;
    udphdr->src = htons (sock->local_port);
    udphdr->dest = htons (sock->remote_port);
    udphdr->chksum = 0x0000;

    if ((netif = NET_ip_route (&(sock->remote_ip))) == NULL)
    {
        DEBUGF (NET_UDP_DEBUG,
                ("\nUdp: no route to 0x%lx.", sock->remote_ip.addr));
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
            ("\nUdp: tx datagram of length %d.",
             tot_len - sizeof (struct NET_udp_hdr)));

    if (sock->flags & NET_UDP_FLAGS_UDPLITE)
    {
        udphdr->len = htons (sock->chksum_len);
        /*
         * calculate checksum 
         */
        udphdr->chksum = 0x0000;
        
#if 1        
        udphdr->chksum = NET_inet_chksum_pseudo (p, src_ip, &(sock->remote_ip),
                                    NET_IP_PROTO_UDP, sock->chksum_len);
        if (udphdr->chksum == 0x0000)
        {
            udphdr->chksum = 0xffff;
        }
#endif

#if NET_UDP_HEAD_DEBUG_TX
        udp_head_debug (udphdr);
#endif

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
            
#if 0
            udphdr->chksum = NET_inet_chksum_pseudo (p, src_ip, &sock->remote_ip,
                                        NET_IP_PROTO_UDP, tot_len);
            if (udphdr->chksum == 0x0000)
            {
                udphdr->chksum = 0xffff;
            }
#endif

        }
#if NET_UDP_HEAD_DEBUG_TX
        udp_head_debug (udphdr);
#endif

        err = NET_ip_output_if (p, src_ip, &sock->remote_ip,
                                NET_UDP_TTL, NET_IP_PROTO_UDP, netif);
    }

    ++NET_stats.udp.tx;
    return err;
}
