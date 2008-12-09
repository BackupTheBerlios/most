/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

#ifndef NET_INET_H
#define NET_INET_H

#include <ace/stddef.h>

#include "net/netbuf.h"
#include "net/ip_addr.h"

/** @defgroup inet inet.h
 *
 * Internet checksum.
 *
 * @{
 */

/**
 * ?
 */
extern ACE_u16_t NET_inet_chksum (void *, ACE_u16_t);

/**
 * ?
 */
extern ACE_u16_t NET_inet_chksum_buf (NET_netbuf_t *);

/**
 * ?
 */
extern ACE_u16_t NET_inet_chksum_pseudo (NET_netbuf_t *,
                                     NET_ip_addr_t *,
                                     NET_ip_addr_t *, ACE_u8_t, ACE_u16_t);

/** @}
 */

#endif
