/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

#ifndef USO_PORT_H
#define USO_PORT_H

#include "uso/list.h"

/** @defgroup port port.h
 *
 * Inter process communication.
 * @{
 */

/*------------- Representation ------------------------------------------*/

/*
 * Port struct.
 *
 * Private.
 */
struct USO_port
{
    /*
     * Threads send blocked 
     */
    USO_list_t send;

    /*
     * Treads receive blocked 
     */
    USO_list_t receive;

    /*
     * Threads reply blocked 
     */
    USO_list_t reply;

};

/** 
 * Port type.
 *
 */
typedef struct USO_port USO_port_t;

/*------------------------------------------------------------------------*/

/*-------------- Interface -----------------------------------------------*/

/**
 * Initialize a port.
 *
 */
extern void USO_port_init (USO_port_t * port);


/**
 * Send a message to a port.
 *
 * Block send and block reply.
 *
 * @param port : Pointer to port.
 * @param message : Message.
 * @return Reply Message.
 */
extern void * USO_send (USO_port_t * port, void * message);

/**
 * Receive a message from a port.
 *
 * Block receive.
 *
 * @param port : Pointer to port.
 * @return Message.
 */
extern void * USO_receive (USO_port_t * port);

/**
 * Reply a message to a port.
 *
 * @param port : Pointer to port.
 * @param message : Reply message.
 */
extern void USO_reply (USO_port_t * port, void * message);


/*------------------------------------------------------------------------*/

/** @}
 */

#endif
