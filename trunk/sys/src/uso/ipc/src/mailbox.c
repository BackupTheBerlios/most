/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

#include "uso/mailbox.h"
#include "uso/list.h"
#include "uso/semaphore.h"
#include "uso/arch/cpu.h"
#include "uso/heap.h"

/*------------- Implementation ------------------------------------------*/

/*
 * Invariant: Semaphore "fetch_sem" has nonnegative count n
 * if n mails are waiting in the mailbox
 * it has negative count -n
 * if n threads are waiting for mails.
 */

extern void
USO_mailbox_init (USO_mailbox_t * mailbox, int max)
{
    USO_list_init (&mailbox->mails);
    USO_semaphore_init (&mailbox->post_sem, max);
    USO_semaphore_init (&mailbox->fetch_sem, 0);
}

extern void
USO_post (USO_mailbox_t * mailbox, USO_node_t * mail)
{
    USO_cpu_status_t ps = USO_disable ();
    USO_wait (&mailbox->post_sem);
    USO_enqueue (&mailbox->mails, mail);
    USO_signal (&mailbox->fetch_sem);
    USO_restore (ps);
}

extern int
USO_post_would_block (USO_mailbox_t * mailbox)
{
    return USO_sem_count (&mailbox->post_sem);
}

extern USO_node_t *
USO_fetch (USO_mailbox_t * mailbox)
{
    USO_node_t *mail;
    USO_cpu_status_t ps = USO_disable ();
    USO_wait (&mailbox->fetch_sem);
    mail = USO_dequeue (&mailbox->mails);
    if (mail != NULL)
        USO_signal (&mailbox->post_sem);
    USO_restore (ps);
    return mail;
}

extern int
USO_fetch_would_block (USO_mailbox_t * mailbox)
{
    return USO_sem_count (&mailbox->fetch_sem);
}

extern void
USO_fetch_abort (USO_mailbox_t * mailbox)
{
    USO_cpu_status_t ps = USO_disable ();
    if (USO_sem_count (&mailbox->fetch_sem) < 0)
        USO_signal (&mailbox->fetch_sem);
    USO_restore (ps);
}

/*------------------------------------------------------------------------*/
