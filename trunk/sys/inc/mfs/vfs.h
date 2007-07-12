/*
 *  Copyright (C) 2007  Michael Feiner
 *
 */

#ifndef MFS_VFS_H
#define MFS_VFS_H

#include "ace/stddef.h"

typedef void MFS_entry_t;


typedef struct MFS_descriptor MFS_descriptor_t;	

struct MFS_descriptor_op
{
	void (*open)(MFS_entry_t *entry);
	void (*close)(MFS_entry_t *entry);
	void (*info)(MFS_entry_t *entry);
};

typedef struct MFS_super MFS_super_t;

struct MFS_super_op
{
	void (*mount)(MFS_super_t *super);
	void (*umount)(MFS_super_t *super);
};


typedef struct MFS_directory MFS_directory_t;

struct MFS_directory_op
{
	void (*open)(MFS_directory_t *dir);
	void (*close)(MFS_directory_t *dir);
	void (*create)(MFS_descriptor_t *desc);
	void (*remove)(MFS_descriptor_t *desc);
	void (*rename)(MFS_descriptor_t *desc);
};


typedef struct MFS_stream MFS_stream_t;

struct MFS_stream_op
{
	void (*open)(MFS_stream_t *stream);
	void (*close)(MFS_stream_t *stream);
	void (*info)(MFS_stream_t *stream);
	size_t (*read)(MFS_stream_t *stream, char *buf, size_t len);
	size_t (*write)(MFS_stream_t *stream, const char *buf, size_t len);
	int (*seek)(MFS_stream_t *stream, ssize_t off, size_t pos);
	void (*flush)(MFS_stream_t *stream);
};


struct MFS_vfs_op
{
	struct MFS_super_op *super_op;
	struct MFS_directory_op *dir_op;
	struct MFS_stream_op *stream_op;
};

#endif
