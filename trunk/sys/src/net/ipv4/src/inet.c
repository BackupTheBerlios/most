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

/** Like the name says... */
#define SWAP_BYTES_IN_WORD(w) ((w & 0xff) << 8) | ((w & 0xff00) >> 8)
/** Split an u32_t in two u16_ts and add them up */
#define FOLD_U32T(u)          ((u >> 16) + (u & 0x0000ffffUL))


#define CHECKSUM_1 1
#define CHECKSUM_2 0
#define CHECKSUM_3 0

#if CHECKSUM_1
/**
 * lwip checksum
 *
 * @param dataptr points to start of data to be summed at any boundary
 * @param len length of data to be summed
 * @return host order (!) lwip checksum (non-inverted Internet sum) 
 *
 * @note accumulator size limits summable length to 64k
 * @note host endianess is irrelevant (p3 RFC1071)
 */
static ACE_u16_t
chksum(void *dataptr, ACE_u16_t len)
{
  ACE_u32_t acc;
  ACE_u16_t src;
  ACE_u8_t *octetptr;

  acc = 0;
  /* dataptr may be at odd or even addresses */
  octetptr = (ACE_u8_t*)dataptr;
  while (len > 1) {
    /* declare first octet as most significant
       thus assume network order, ignoring host order */
    src = (*octetptr) << 8;
    octetptr++;
    /* declare second octet as least significant */
    src |= (*octetptr);
    octetptr++;
    acc += src;
    len -= 2;
  }
  if (len > 0) {
    /* accumulate remaining octet */
    src = (*octetptr) << 8;
    acc += src;
  }
  /* add deferred carry bits */
  acc = (acc >> 16) + (acc & 0x0000ffffUL);
  if ((acc & 0xffff0000) != 0) {
    acc = (acc >> 16) + (acc & 0x0000ffffUL);
  }
  /* This maybe a little confusing: reorder sum using htons()
     instead of ntohs() since it has a little less call overhead.
     The caller must invert bits for Internet sum ! */
  return ACE_htons((ACE_u16_t)acc);
}
#endif

#if CHECKSUM_2
/*
 * Curt McDowell
 * Broadcom Corp.
 * csm@broadcom.com
 *
 * IP checksum two bytes at a time with support for
 * unaligned buffer.
 * Works for len up to and including 0x20000.
 * by Curt McDowell, Broadcom Corp. 12/08/2005
 *
 * @param dataptr points to start of data to be summed at any boundary
 * @param len length of data to be summed
 * @return host order (!) lwip checksum (non-inverted Internet sum) 
 */

static ACE_u16_t
chksum(void *dataptr, int len)
{
  ACE_u8_t *pb = dataptr;
  ACE_u16_t *ps, t = 0;
  ACE_u32_t sum = 0;
  int odd = ((ACE_u32_t)pb & 1);

  /* Get aligned to u16_t */
  if (odd && len > 0) {
    ((u8_t *)&t)[1] = *pb++;
    len--;
  }

  /* Add the bulk of the data */
  ps = (ACE_u16_t *)pb;
  while (len > 1) {
    sum += *ps++;
    len -= 2;
  }

  /* Consume left-over byte, if any */
  if (len > 0) {
    ((ACE_u8_t *)&t)[0] = *(ACE_u8_t *)ps;;
  }

  /* Add end bytes */
  sum += t;

  /* Fold 32-bit sum to 16 bits
     calling this twice is propably faster than if statements... */
  sum = FOLD_U32T(sum);
  sum = FOLD_U32T(sum);

  /* Swap if alignment was odd */
  if (odd) {
    sum = SWAP_BYTES_IN_WORD(sum);
  }

  return sum;
}
#endif

#if CHECKSUM_3
/**
 * An optimized checksum routine. Basically, it uses loop-unrolling on
 * the checksum loop, treating the head and tail bytes specially, whereas
 * the inner loop acts on 8 bytes at a time. 
 *
 * @arg start of buffer to be checksummed. May be an odd byte address.
 * @len number of bytes in the buffer to be checksummed.
 * @return host order (!) lwip checksum (non-inverted Internet sum) 
 * 
 * by Curt McDowell, Broadcom Corp. December 8th, 2005
 */

static ACE_u16_t
chksum(void *dataptr, int len)
{
  ACE_u8_t *pb = dataptr;
  ACE_u16_t *ps, t = 0;
  ACE_u32_t *pl;
  ACE_u32_t sum = 0, tmp;
  /* starts at odd byte address? */
  int odd = ((ACE_u32_t)pb & 1);

  if (odd && len > 0) {
    ((ACE_u8_t *)&t)[1] = *pb++;
    len--;
  }

  ps = (ACE_u16_t *)pb;

  if (((ACE_u32_t)ps & 3) && len > 1) {
    sum += *ps++;
    len -= 2;
  }

  pl = (ACE_u32_t *)ps;

  while (len > 7)  {
    tmp = sum + *pl++;          /* ping */
    if (tmp < sum) {
      tmp++;                    /* add back carry */
    }

    sum = tmp + *pl++;          /* pong */
    if (sum < tmp) {
      sum++;                    /* add back carry */
    }

    len -= 8;
  }

  /* make room in upper bits */
  sum = FOLD_U32T(sum);

  ps = (ACE_u16_t *)pl;

  /* 16-bit aligned word remaining? */
  while (len > 1) {
    sum += *ps++;
    len -= 2;
  }

  /* dangling tail byte remaining? */
  if (len > 0) {                /* include odd byte */
    ((ACE_u8_t *)&t)[0] = *(ACE_u8_t *)ps;
  }

  sum += t;                     /* add end bytes */

  /* Fold 32-bit sum to 16 bits
     calling this twice is propably faster than if statements... */
  sum = FOLD_U32T(sum);
  sum = FOLD_U32T(sum);

  if (odd) {
    sum = SWAP_BYTES_IN_WORD(sum);
  }

  return sum;
}
#endif


/*-----------------------------------------------------------------------------------*/
/*
 * inet_chksum: Calculates the Internet checksum over a portion of
 * memory. Used primarely for IP and ICMP. 
 */
/*-----------------------------------------------------------------------------------*/

extern ACE_u16_t
NET_inet_chksum (void *dataptr, ACE_u16_t len)
{
	return ~chksum(dataptr, len);
}


/**
 * Calculate a checksum over a chain of pbufs (without pseudo-header, much like
 * inet_chksum only pbufs are used).
 *
 * @param p pbuf chain over that the checksum should be calculated
 * @return checksum (as u16_t) to be saved directly in the protocol header
 */

static ACE_u16_t
chksum_buf(NET_netbuf_t* p)
{
	ACE_u32_t acc;
  	NET_netbuf_t *q;
  	ACE_u8_t swapped;

	acc = 0;
  	swapped = 0;
    for (q = p; q != NULL; q = NET_netbuf_next(q))
    {
    	int len = NET_netbuf_len(q);
	    acc += chksum((ACE_u16_t *)NET_netbuf_index(q), len);
    	acc = FOLD_U32T(acc);
   		if ( len % 2 != 0) {
      		swapped = 1 - swapped;
      		acc = SWAP_BYTES_IN_WORD(acc);
    	}
  	}

  	if (swapped) {
    	acc = SWAP_BYTES_IN_WORD(acc);
  	}
  	return (ACE_u16_t) (acc & 0xffffUL);
}


extern ACE_u16_t
NET_inet_chksum_buf(NET_netbuf_t* p)
{
	return ~chksum_buf(p);
}

/* inet_chksum_pseudo:
 *
 * Calculates the pseudo Internet checksum used by TCP and UDP for a pbuf chain.
 * IP addresses are expected to be in network byte order.
 *
 * @param p chain of pbufs over that a checksum should be calculated (ip data part)
 * @param src source ip address (used for checksum of pseudo header)
 * @param dst destination ip address (used for checksum of pseudo header)
 * @param proto ip protocol (used for checksum of pseudo header)
 * @param proto_len length of the ip data part (used for checksum of pseudo header)
 * @return checksum (as u16_t) to be saved directly in the protocol header
 */

extern ACE_u16_t
NET_inet_chksum_pseudo (NET_netbuf_t * p,
                        NET_ip_addr_t * src,
                        NET_ip_addr_t * dest,
                        ACE_u8_t proto, ACE_u16_t proto_len)
{
	ACE_u32_t acc;
  	acc = chksum_buf(p);

  	acc += (src->addr & 0xffffUL);
  	acc += ((src->addr >> 16) & 0xffffUL);
  	acc += (dest->addr & 0xffffUL);
  	acc += ((dest->addr >> 16) & 0xffffUL);
  	acc += (ACE_u32_t)ACE_htons((ACE_u16_t)proto);
  	acc += (ACE_u32_t)ACE_htons(proto_len);

  	acc = FOLD_U32T(acc);
  	acc = FOLD_U32T(acc);
  	return (ACE_u16_t)~(acc & 0xffffUL);
}
