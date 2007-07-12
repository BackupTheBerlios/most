/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

#ifndef USO_MAILBOX_H
#define USO_MAILBOX_H

#include "uso/list.h"
#include "uso/semaphore.h"

/** @defgroup mailbox mailbox.h
 *
 * Inter process communication.
 * @{
 */

/*------------- Representation ------------------------------------------*/

/*
 * Mailbox struct.
 *
 * Private.
 */
struct USO_mailbox
{
    /*
     * Messages in the mailbox 
     */
    USO_list_t mails;

    /*
     * Semaphore on which sender will block if mailbox is full 
     */
    USO_semaphore_t post_sem;

    /*
     * Semaphore on which receiver will block if mailbox is empty 
     */
    USO_semaphore_t fetch_sem;
};

/** 
 * Mailbox type.
 *
 */
typedef struct USO_mailbox USO_mailbox_t;

/*------------------------------------------------------------------------*/

/*-------------- Interface -----------------------------------------------*/

/**
 * Initialize a mailbox.
 *
 * @param mailbox : Pointer to mailbox.
 * @param max : Max mails the mailbox can contain befor post blocks.
 */
extern void USO_mailbox_init (USO_mailbox_t * mailbox, int max);

/**
 * Post a mail to a mailbox.
 *
 * Block if the mailbox is full.
 *
 * @param mailbox : Pointer to mailbox.
 * @param mail : Mail.
 */
extern void USO_post (USO_mailbox_t * mailbox, USO_node_t * mail);

/**
 * Fetch a mail from a mailbox.
 *
 * Block if the mailbox is empty.
 *
 * @param mailbox : Pointer to mailbox.
 * @return mail or NULL if aborted.
 */
extern USO_node_t *USO_fetch (USO_mailbox_t * mailbox);

/**
 * Abort fetching a mail from a mailbox.
 *
 * Do to timeouts start a timer which calls this function.
 *
 * @param mailbox : Pointer to mailbox.
 */
extern void USO_fetch_abort (USO_mailbox_t * mailbox);

/*------------------------------------------------------------------------*/

/** @}
 */

#endif
