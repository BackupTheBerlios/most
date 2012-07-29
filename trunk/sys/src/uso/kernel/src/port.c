/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

#include "uso/port.h"
#include "uso/list.h"
#include "uso/arch/cpu.h"
#include "uso/thread.h"
#include "uso/scheduler.h"

/*------------- Implementation ------------------------------------------*/

extern void
USO_port_init (USO_port_t * port)
{
    USO_list_init (&port->send);
    USO_list_init (&port->receive);
    USO_list_init (&port->reply);
}

extern void *
USO_send (USO_port_t * port, void *message)
{
    void *reply_message;
    USO_cpu_status_t ps = USO_disable ();
    USO_thread_t *receiver = ((USO_thread_t *) USO_dequeue (&port->receive));
    USO_thread_t *sender = USO_current ();
    if (receiver != NULL)
    {
        receiver->message = message;
        USO_ready (receiver);
    }
    else
    {
        sender->message = message;
        sender->state = USO_BLOCKED_SEND;
        USO_enqueue (&port->send, (USO_node_t *) sender);
        USO_schedule (USO_next2run ());
    }
    sender->state = USO_BLOCKED_REPLY;
    USO_enqueue (&port->reply, (USO_node_t *) sender);
    USO_schedule (USO_next2run ());
    reply_message = sender->message;
    USO_restore (ps);
    return reply_message;
}

extern void *
USO_receive (USO_port_t * port)
{
    void *message;
    USO_cpu_status_t ps = USO_disable ();
    USO_thread_t *sender = ((USO_thread_t *) USO_dequeue (&port->send));
    USO_thread_t *receiver = USO_current ();
    if (sender != NULL)
    {
        message = sender->message;
        USO_ready (sender);
    }
    else
    {
        receiver->state = USO_BLOCKED_RECEIVE;
        USO_enqueue (&port->receive, (USO_node_t *) receiver);
        USO_schedule (USO_next2run ());
        message = USO_current ()->message;
    }
    USO_restore (ps);
    return message;
}

extern void
USO_reply (USO_port_t * port, void *message)
{
    USO_cpu_status_t ps = USO_disable ();
    USO_thread_t *to_reply = ((USO_thread_t *) USO_dequeue (&port->reply));
    if (to_reply != NULL)
    {
        to_reply->message = message;
        USO_ready (to_reply);
    }
    else
    {
        // error reply without receive 
    }
    USO_restore (ps);
}

/*------------------------------------------------------------------------*/
