/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

/*-----------------------------------------------------------------------------------*/
/*
 * inet.c Functions common to all TCP/IP modules, such as the Internet
 * checksum and the byte order functions. 
 */
/*-----------------------------------------------------------------------------------*/
#include <ace/stddef.h>
#include <ace/stdlib.h>

#include "net/debug.h"
#include "net/inet.h"


/*-----------------------------------------------------------------------------------*/
/*
 * chksum: Sums up all 16 bit words in a memory portion. Also includes
 * any odd byte. This function is used by the other checksum functions.
 * For now, this is not optimized. Must be optimized for the particular
 * processor arcitecture on which it is to run. Preferebly coded in
 * assembler. 
 */
/*-----------------------------------------------------------------------------------*/

static u32_t
chksum (u16_t * dataptr, int len)
{
    u32_t acc;

    for (acc = 0; len > 1; len -= 2)
    {
        acc += *dataptr++;
    }

    /*
     * add up any odd byte 
     */
    if (len > 0)
    {
        acc += ((u16_t) ((*(u8_t *) dataptr) & 0xff)) << 8;
        DEBUGF (NET_INET_DEBUG, ("\nInet: odd byte %d.", *(u8_t *) dataptr));
    }

    return acc;
}

static u32_t
chksum_buf (NET_netbuf_t * p)
{
    u32_t acc;
    NET_netbuf_t *q;
    u8_t swapped;

    acc = 0;
    swapped = 0;
    for (q = p; q != NULL; q = q->next)
    {
        acc += chksum ((u16_t *) (q->index), q->len);
        while (acc >> 16)
        {
            acc = (acc & 0xffff) + (acc >> 16);
        }
        if ( (q->len % 2) != 0)
        {
            swapped = 1 - swapped;
            acc = (acc & 0xff << 8) | (acc & 0xff00 >> 8);
        }
    }

    if (swapped)
    {
        acc = ((acc & 0xff) << 8) | ((acc & 0xff00) >> 8);
    }
    return acc;
}

/*-----------------------------------------------------------------------------------*/
/*
 * inet_chksum: Calculates the Internet checksum over a portion of
 * memory. Used primarely for IP and ICMP. 
 */
/*-----------------------------------------------------------------------------------*/

extern u16_t
NET_inet_chksum (void *dataptr, u16_t len)
{
    u32_t acc;

    acc = chksum (dataptr, len);
    while (acc >> 16)
    {
        acc = (acc & 0xffff) + (acc >> 16);
    }
    return ~(acc & 0xffff);
}

/*-----------------------------------------------------------------------------------*/


extern u16_t
NET_inet_chksum_buf (NET_netbuf_t * p)
{
    u32_t acc = chksum_buf (p);
    return ~(acc & 0xffff);
}

/*-----------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------------*/
/*
 * inet_chksum_pseudo: Calculates the pseudo Internet checksum used by
 * TCP and UDP for a pbuf chain. 
 */
/*-----------------------------------------------------------------------------------*/

extern u16_t
NET_inet_chksum_pseudo (NET_netbuf_t * p,
                        NET_ip_addr_t * src,
                        NET_ip_addr_t * dest, u8_t proto, u16_t proto_len)
{
    u32_t acc = chksum_buf (p);
    acc += (src->addr & 0xffff);
    acc += ((src->addr >> 16) & 0xffff);
    acc += (dest->addr & 0xffff);
    acc += ((dest->addr >> 16) & 0xffff);
    acc += (u32_t) htons ((u16_t) proto);
    acc += (u32_t) htons (proto_len);

    while (acc >> 16)
    {
        acc = (acc & 0xffff) + (acc >> 16);
    }
    return ~(acc & 0xffff);
}
