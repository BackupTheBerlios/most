/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

#include <uso/cpu.h>
#include <uso/sleep.h>
#include <uso/pipe.h>
#include <ace/stdio.h>
#include <mfs/stream.h>
#include <mfs/directory.h>
#include <mfs/sysfs.h>
#include "file.h"
#include "sfs.h"

struct SFS_file
{
    USO_pipe_t buf;
    USO_mutex_t lock;
    char buffer[SFS_BLOCK_SIZE];
};

typedef struct SFS_file SFS_file_t;


static ACE_err_t
file_open (MFS_descriptor_t * desc)
{
    SFS_file_t *file = (SFS_file_t *) desc->represent;

    USO_pipe_init (&file->buf, file->buffer, sizeof (file->buffer));
    USO_mutex_init (&file->lock);

    return ACE_OK;
}


static void
file_close (MFS_descriptor_t * desc)
{
    //MFS_sfs_file_t *file = (MFS_sfs_file_t *) desc->represent;
}

static struct MFS_descriptor_op file_desc_op = {
	.open = file_open,
    .close = file_close,
    .info = NULL,
    .control = NULL
};


static ACE_size_t
file_read (MFS_stream_t * stream, char *buf, ACE_size_t len)
{
    SFS_file_t *file = (SFS_file_t *) ((MFS_descriptor_t *)stream)->represent;
    ACE_size_t ret;
    ret = USO_pipe_read_ns (&file->buf, buf, len);
    stream->pos_rx += ret;
    return ret;
}

static ACE_size_t
file_write (MFS_stream_t * stream, const char *buf, ACE_size_t len)
{
    SFS_file_t *file = (SFS_file_t *) ((MFS_descriptor_t *)stream)->represent;
    ACE_size_t ret;
    ret = USO_pipe_write_ns (&file->buf, buf, len);
    if (ret > 0)
    {
        stream->size_tx += ret;
    }
    // serial->error.tx_buf_overrun += (len - ret);
    return ret;
}

static struct MFS_stream_op file_op = {
	.read = file_read,
	.write = file_write,
    .seek = NULL,
    .flush = NULL
};




extern void
SFS_file_init (void)
{
}
