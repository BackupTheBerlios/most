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

static USO_list_t udp_socks;

static USO_buf_pool_t udp_pool;
struct udp_packet
{
    NET_netbuf_t buf_hdr;
    struct NET_eth_hdr eth_hdr;
    struct NET_ip_hdr ip_hdr;
    struct NET_udp_hdr udp_hdr;
} _PACKED_;

static struct udp_packet udp_packets[NET_ETH_RX_TX_QUE_SIZE];

extern void
NET_udp_init (void)
{
    USO_list_init (&udp_socks);
    USO_buf_pool_init (&udp_pool, udp_packets,
                       NET_ETH_RX_TX_QUE_SIZE, sizeof (struct udp_packet));

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

    ++stats.udp.rx;

    iphdr = (struct NET_ip_hdr *)p->index;
    NET_netbuf_index_inc (p, sizeof (struct NET_ip_hdr));
    udphdr = (struct NET_udp_hdr *)p->index;

    DEBUGF (NET_UDP_DEBUG, ("\nUdp: rx datagram of length %d.",
                            p->len - sizeof (struct NET_udp_hdr)));

    // NET_netbuf_len_adjust(p, udphdr->len); 

    src = ntohs (udphdr->src);
    dest = ntohs (udphdr->dest);

#if NET_UDP_HEAD_DEBUG_RX
    udp_head_debug (udphdr);
#endif

    while ((sock = (NET_udp_socket_t *) USO_next_element (&udp_socks, (USO_node_t*)sock)) != NULL)
    {
        if (sock->remote_port == src && sock->local_port == dest
            &&
            (NET_ip_addr_isbroadcast ((&sock->remote_ip), &(inp->netmask))
             || NET_ip_addr_isany (&sock->remote_ip)
             || NET_ip_addr_cmp (&(sock->remote_ip), &(iphdr->src)))
            && (NET_ip_addr_isany (&sock->local_ip)
                || NET_ip_addr_cmp (&(sock->local_ip), &(iphdr->dest))))
        {
            DEBUGF (NET_UDP_DEBUG, ("\nUdp: found exact sock."));
            break;
        }
    }

    if (sock == NULL)
    {
        while ((sock =
                (NET_udp_socket_t *) USO_next_element (&udp_socks, (USO_node_t*)sock)) != NULL)
        {
            if (sock->local_port == dest
                && (NET_ip_addr_isany (&sock->remote_ip)
                    || NET_ip_addr_cmp (&(sock->remote_ip),
                                        &(iphdr->src)))
                && (NET_ip_addr_isany (&sock->local_ip)
                    || NET_ip_addr_cmp (&(sock->local_ip), &(iphdr->dest))))
            {
                DEBUGF (NET_UDP_DEBUG, ("\nUdp: found sock."));
                break;
            }
        }
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
                ++stats.udp.rx_drop;
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

        ++stats.udp.rx_drop;
        return NET_ERR_BAD;
    }

    return NET_ERR_OK;
}




extern void
NET_udp_socket_init (NET_udp_socket_t * sock)
{
    bzero (sock, sizeof (NET_udp_socket_t));
    USO_mailbox_init (&sock->rx_que, NET_ETH_RX_TX_QUE_SIZE);
    sock->rx_timeout = FALSE;
}

extern NET_udp_socket_t *
NET_udp_socket_new (void)
{
    NET_udp_socket_t *sock;
    sock = malloc (sizeof (NET_udp_socket_t));
    if (sock != NULL)
    {
        NET_udp_socket_init (sock);
    }
    return sock;
}

static NET_udp_socket_t *
find_sock (NET_udp_socket_t * sock)
{
	NET_udp_socket_t *isock = NULL;
	while ( (isock = (NET_udp_socket_t*)USO_next_element(&udp_socks, (USO_node_t*)isock))
			!= NULL){
	    if (sock == isock){
   			break;	   	
    	}		   		
	}    
	return isock;
}

extern void
NET_udp_socket_open (NET_udp_socket_t * sock)
{
    NET_udp_socket_t *isock;
    isock = find_sock(sock);
    if (isock == NULL)
    {
        USO_enqueue (&udp_socks, (USO_node_t *) sock);
    }
}

extern void
NET_udp_socket_close (NET_udp_socket_t * sock)
{
    NET_udp_socket_t *isock;
    NET_netbuf_t *packet;
    NET_ip_addr_t addr;
    u16_t port;
    isock = find_sock(sock);
    if (isock != NULL)
    {
        USO_remove (&udp_socks, (USO_node_t *) sock);
        NET_udp_recv_timeout (sock, 1);
        while ((packet = NET_udp_recv (sock, &addr, &port)) != NULL)
            NET_netbuf_free (packet);
        NET_udp_recv_timeout (sock, 0);
    }
}

extern void
NET_udp_bind (NET_udp_socket_t * sock, NET_ip_addr_t * ipaddr, u16_t port)
{
    NET_ip_addr_set (&sock->local_ip, ipaddr);
    sock->local_port = port;
    DEBUGF (NET_UDP_DEBUG, ("\nUdp: bind port %d.", port));
}

extern void
NET_udp_connect (NET_udp_socket_t * sock, NET_ip_addr_t * ipaddr, u16_t port)
{
    NET_ip_addr_set (&sock->remote_ip, ipaddr);
    sock->remote_port = port;
    DEBUGF (NET_UDP_DEBUG, ("\nUdp: connect port %d.", port));
}

static void
rx_timeout (void *rx_que)
{
    USO_fetch_abort ((USO_mailbox_t *) rx_que);
}

extern void
NET_udp_recv_timeout (NET_udp_socket_t * sock, unsigned long timeout)
{
    if (timeout > 0)
    {
        DEV_timer_init (&sock->rx_timer, rx_timeout, &sock->rx_que, timeout);
        sock->rx_timeout = TRUE;
    }
    else
    {
        sock->rx_timeout = FALSE;
    }
}

/*
 * timeout has to be implemented 
 */
extern NET_netbuf_t *
NET_udp_recv (NET_udp_socket_t * sock, NET_ip_addr_t * addr, u16_t * port)
{
    NET_netbuf_t *p;
    struct NET_udp_hdr *udphdr;
    struct NET_ip_hdr *iphdr;
    if (sock->rx_timeout == TRUE)
        DEV_timer_start (&sock->rx_timer);
    p = (NET_netbuf_t *) USO_fetch (&sock->rx_que);
    if (p != NULL)
    {
        if (sock->rx_timeout == TRUE)
            DEV_timer_stop (&sock->rx_timer);
        iphdr = (struct NET_ip_hdr *)(p->index - sizeof (struct NET_ip_hdr));
        udphdr = (struct NET_udp_hdr *)p->index;
        NET_netbuf_index_inc (p, sizeof (struct NET_udp_hdr));
        if (addr != NULL)
            *addr = iphdr->src;
        if (port != NULL)
            *port = ntohs (udphdr->src);
    }
    return p;
}

NET_err_t
NET_udp_send (NET_udp_socket_t * sock, NET_netbuf_t * p)
{
    struct NET_udp_hdr *udphdr;
    NET_netif_t *netif;
    NET_ip_addr_t *src_ip;
    NET_err_t err;
    NET_netbuf_t *q;
    short tot_len;


    q = NET_netbuf_alloc (&udp_pool, 0, NULL);
    p = NET_netbuf_chain (q, p);

    NET_netbuf_index_inc (p, sizeof (struct NET_eth_hdr) +
                          sizeof (struct NET_ip_hdr));

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

    ++stats.udp.tx;
 	if (err != NET_ERR_OK){
		NET_netbuf_free (p);
 	} 	
    return err;
}
