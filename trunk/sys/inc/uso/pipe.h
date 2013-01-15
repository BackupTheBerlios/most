/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

#ifndef USO_PIPE_H
#define USO_PIPE_H

#include <ace/stddef.h>

/** @addtogroup uso
 * @{
 */

/** @defgroup pipe pipe.h
 *
 * This is a ring buffer.
 * But i like it more to see it as an pipe.
 * On one end you can write data (fill the pipe) as long there is place in the buffer (pipe).
 * On the other side you can read data as long there is data in the buffer (pipe).
 *
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
    char *start;                /* Pointer to the beginn of buffer */
    char *end;                  /* Pointer to the end of buffer */
    char *read;                 /* Read pointer into ther buffer */
    char *write;                /* Write pointer into the buffer */

};

/** 
 * Pipe type.
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
extern void USO_pipe_init (USO_pipe_t * pipe, char *buf, ACE_size_t size);

/**
 * Creates an instance of a pipe.
 *
 * @param size : Size of buffer.
 */
extern USO_pipe_t *USO_pipe_new (ACE_size_t size);

/**
 * Deletes an instance of a pipe.
 *
 * @param pipe : Pointer to pipe.
 */
extern void USO_pipe_del (USO_pipe_t * pipe);

/**
 * Read data from pipe.
 * Not synchronized.
 *
 * @param pipe : Pointer to pipe.
 * @param buf : Pointer to buffer where the data will be written.
 * @param len : Length of data which should be read.
 * @return Length of data which is read.
 */
extern ACE_size_t USO_pipe_read_ns (USO_pipe_t * pipe, char *buf, ACE_size_t len);

/**
 * Write data to pipe.
 * Not synchronized.
 *
 * @param pipe : Pointer to pipe.
 * @param buf : Pointer to buffer where the data is read.
 * @param len : Length of data which should be written.
 * @return Length of data which is written.
 */
extern ACE_size_t USO_pipe_write_ns (USO_pipe_t * pipe, const char *buf, ACE_size_t len);

/*------------------------------------------------------------------------*/

/** @}
 */

/** @}
 */

#endif
