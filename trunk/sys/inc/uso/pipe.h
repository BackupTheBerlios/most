/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

#ifndef USO_PIPE_H
#define USO_PIPE_H

#include <ace/stddef.h>

/** @defgroup pipe pipe.h
 *
 * Inter process communication.
 * @{
 */


enum USO_pipe_state
{
    USO_PIPE_EMPTY,
    USO_PIPE_DATA,
    USO_PIPE_FULL
};

/*------------- Representation ------------------------------------------*/

/*
 * Pipe struct.
 *
 * Private.
 */
struct USO_pipe
{

	enum USO_pipe_state state;
    /*
     * Pointer to the beginn of buffer 
     */
    char *start;

    /*
     * Pointer to the end of buffer 
     */
    char *end;

    /*
     * Read pointer into ther buffer 
     */
    char *read;

    /*
     * Write pointer into the buffer 
     */
    char *write;

};

/** 
 * Pipe type.
 *
 */
typedef struct USO_pipe USO_pipe_t;

/*------------------------------------------------------------------------*/

/*-------------- Interface -----------------------------------------------*/

/**
 * Initialize a pipe.
 *
 * @param pipe : Pointer to pipe.
 * @param buf : Pointer to buffer.
 * @param size : Size of buffer.
 */
extern void USO_pipe_init (USO_pipe_t * pipe,
                           char *buf, size_t size);

/**
 * Creates an instance of a pipe.
 *
 * @param size : Size of buffer.
 */
extern USO_pipe_t *USO_pipe_new (size_t size);

/**
 * Deletes an instance of a pipe.
 *
 * @param pipe : Pointer to pipe.
 */
extern void USO_pipe_del (USO_pipe_t * pipe);

extern size_t USO_pipe_read_ns (USO_pipe_t * pipe, char *buf, size_t len);
extern size_t USO_pipe_write_ns (USO_pipe_t * pipe, const char *buf, size_t len);

/*------------------------------------------------------------------------*/

/** @}
 */

#endif
