/*
 * pipe.h
 *
 *  Created on: 02.01.2013
 *      Author: maik
 */

#ifndef CLI_PIPE_H_
#define CLI_PIPE_H_

#include <ace/stddef.h>
#include <uso/pipe.h>
#include <uso/monitor.h>
#include <mfs/vfs.h>
#include <mfs/stream.h>


/** @addtogroup cli
 * @{
 */

/** @defgroup pipe pipe.h
 *
 * Pipe.
 * @{
 */

enum CLI_pipe_state
{
    CLI_PIPE_INIT,      /**< pipe is initialized. */
    CLI_PIPE_OPEN,      /**< pipe is open. */
    CLI_PIPE_CLOSE,     /**< pipe is closed. */
};


/*
 * Pipe Struct
 *
 * Private.
 */
struct CLI_pipe
{
    USO_pipe_t *pipe;
    USO_mutex_t sync;
    USO_mutex_t r_lock;
    USO_mutex_t w_lock;
    USO_barrier_t cond_full;
    USO_barrier_t cond_empty;
    MFS_descriptor_t *desc;
    enum CLI_pipe_state state;
};

/** Pipe type. */
typedef struct CLI_pipe CLI_pipe_t;


extern void CLI_pipe_init (CLI_pipe_t * pipe, MFS_descriptor_t *dir, char *name, USO_pipe_t * pipe_ns);

extern CLI_pipe_t *CLI_pipe_new (ACE_size_t size, MFS_descriptor_t *dir, char *name);

extern void CLI_pipe_del (MFS_descriptor_t *dir, char *name);


#endif /* CLI_PIPE_H_ */
