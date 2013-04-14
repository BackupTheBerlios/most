/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

#include <ace/stdlib.h>
#include <uso/cpu.h>
#include <uso/sleep.h>
#include <ace/stdio.h>
#include <mfs/stream.h>
#include <mfs/directory.h>
#include <mfs/sysfs.h>
#include <cli/pipe.h>
#include <cli/err.h>


static ACE_err_t
pipe_open (MFS_descriptor_t *desc)
{
    CLI_pipe_t *pipe = (CLI_pipe_t *) desc->represent;
    USO_lock (&pipe->sync);
    if (pipe->state == CLI_PIPE_INIT)
    	pipe->state = CLI_PIPE_OPEN;
    USO_unlock (&pipe->sync);
    return ACE_OK;
}

static void
pipe_close (MFS_descriptor_t *desc)
{
	CLI_pipe_t *pipe = (CLI_pipe_t *) desc->represent;
    USO_lock (&pipe->sync);
    if (pipe->state == CLI_PIPE_OPEN)
    	pipe->state = CLI_PIPE_CLOSE;
    if (desc->open_cnt == 0)
    	pipe->state = CLI_PIPE_INIT;
    USO_go_all(&pipe->cond_full);
    USO_go_all(&pipe->cond_empty);
    USO_unlock (&pipe->sync);
    return;
}

static void
pipe_info (MFS_descriptor_t *desc)
{
	CLI_pipe_t *pipe = (CLI_pipe_t *) desc->represent;
    USO_lock (&pipe->sync);
    MFS_stream_print((MFS_stream_t *)desc);
	ACE_puts("Pipe IO\n");
    USO_unlock (&pipe->sync);
}

static struct MFS_descriptor_op pipe_desc_op = {
	.open = pipe_open,
    .close = pipe_close,
    .info = pipe_info,
    .control = NULL,
};

static ACE_size_t
pipe_read (MFS_stream_t *stream, char *buf, ACE_size_t len)
{
	CLI_pipe_t *pipe = (CLI_pipe_t *) ((MFS_descriptor_t *)stream)->represent;
    ACE_size_t ret = len;
    ACE_size_t readed;
    USO_lock (&pipe->r_lock);
    USO_lock (&pipe->sync);
    if (pipe->state != CLI_PIPE_OPEN && pipe->pipe->state == USO_PIPE_EMPTY) return ACE_EOF;
    while (len)
    {
    	while (pipe->pipe->state == USO_PIPE_EMPTY && pipe->state == CLI_PIPE_OPEN){
            USO_monitor (&pipe->sync, &pipe->cond_empty);
    	}
        readed = USO_pipe_read_ns (pipe->pipe, buf, len);
        len -= readed;
        buf += readed;
        USO_go_all(&pipe->cond_full);
        if (pipe->pipe->state == USO_PIPE_EMPTY && pipe->state != CLI_PIPE_OPEN)
        	break;
    }
    readed = ret - len;
    stream->pos_rx += readed;
    USO_unlock (&pipe->sync);
    USO_unlock (&pipe->r_lock);
    return readed;
}

static ACE_size_t
pipe_write (MFS_stream_t * stream, const char *buf, ACE_size_t len)
{
	CLI_pipe_t *pipe = (CLI_pipe_t *) ((MFS_descriptor_t *)stream)->represent;
    ACE_size_t ret = len;
    ACE_size_t written;
    USO_lock (&pipe->w_lock);
    USO_lock (&pipe->sync);
    if (pipe->state != CLI_PIPE_OPEN) return ACE_EOF;
    while (len)
    {
    	while (pipe->pipe->state == USO_PIPE_FULL && pipe->state == CLI_PIPE_OPEN){
            USO_monitor (&pipe->sync, &pipe->cond_full);
    	}
        written = USO_pipe_write_ns (pipe->pipe, buf, len);
        len -= written;
        buf += written;
        USO_go_all(&pipe->cond_empty);
        if (pipe->state != CLI_PIPE_OPEN)
        	break;
    }
    written = ret - len;
    stream->size_tx += written;
    USO_unlock (&pipe->sync);
    USO_unlock (&pipe->w_lock);
    return written;
}


static struct MFS_stream_op pipe_stream_op = {
    .read = pipe_read,
    .write = pipe_write,
    .seek = NULL,
    .flush = NULL
};


extern void
CLI_pipe_init (CLI_pipe_t * pipe, MFS_descriptor_t *dir, char *name, USO_pipe_t * pipe_ns)
{
	pipe->pipe = pipe_ns;
    USO_mutex_init (&pipe->sync);
    USO_mutex_init (&pipe->r_lock);
    USO_mutex_init (&pipe->w_lock);
    USO_barrier_init (&pipe->cond_full);
    USO_barrier_init (&pipe->cond_empty);
	pipe->desc = MFS_stream_create (dir, name, &pipe_desc_op, &pipe_stream_op, (MFS_represent_t *) pipe, MFS_STREAM_IO);
	pipe->state = CLI_PIPE_INIT;
}

extern CLI_pipe_t *
CLI_pipe_new (ACE_size_t size, MFS_descriptor_t *dir, char *name)
{
    CLI_pipe_t *pipe = ACE_malloc (sizeof (CLI_pipe_t));
    if (pipe != NULL)
    {
        pipe->pipe = USO_pipe_new (size);
        if (pipe->pipe != NULL){
        	CLI_pipe_init (pipe, dir, name, pipe->pipe);
        } else {
        	ACE_free (pipe);
        	pipe = NULL;
        }
    }
    return pipe;
}

extern void
CLI_pipe_del (MFS_descriptor_t *dir, char *name)
{
    MFS_descriptor_t *desc = MFS_lookup (dir, name);
    if (desc != NULL && desc->type == MFS_STREAM) /** todo we don't know if this is a pipe struct !!! */
    {
    	CLI_pipe_t *pipe = ((CLI_pipe_t *) desc->represent);
    	if (pipe != NULL && pipe->state == CLI_PIPE_INIT){
    		MFS_remove_desc (dir, pipe->desc);
    		USO_pipe_del (pipe->pipe);
    		ACE_free (pipe);
    	}
    }
}
