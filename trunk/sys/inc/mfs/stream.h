/*
 *  Copyright (C) 2007  Michael Feiner
 *
 */

#ifndef MFS_STREAM_H
#define MFS_STREAM_H

#include "uso/mutex.h"
#include "mfs/vfs.h"

typedef void MFS_stream_represent_t;

extern struct MFS_descriptor_op MFS_stream_descriptor_op;

enum MFS_stream_type
{
	MFS_FILE,
	MFS_IO
};

struct MFS_stream
{
	enum MFS_stream_type type;
	ACE_size_t size_tx;
	ACE_size_t pos_rx;
	USO_mutex_t lock;
	struct MFS_stream_op *operations;
	MFS_stream_represent_t *represent;
};

extern ACE_size_t MFS_read(MFS_stream_t *stream, char *buf, ACE_size_t len);
extern ACE_size_t MFS_write(MFS_stream_t *stream, char *buf, ACE_size_t len);
extern int MFS_seek(MFS_stream_t *stream, ACE_ssize_t off, ACE_size_t pos);
extern void MFS_flush(MFS_stream_t *stream);

extern MFS_stream_t *MFS_get_stream (MFS_descriptor_t *stream_desc);

extern void MFS_stream_init(MFS_stream_t *stream,
								enum MFS_stream_type type,
								struct MFS_stream_op *operations,
								MFS_stream_represent_t *represent);

extern ACE_size_t MFS_empty_read(MFS_stream_t *stream, char *buf, ACE_size_t len);
extern ACE_size_t MFS_empty_write(MFS_stream_t *stream, const char *buf, ACE_size_t len);

#endif
