/*
 *  Copyright (C) 2007  Michael Feiner
 *
 */

#ifndef MFS_VFS_H
#define MFS_VFS_H

#include "ace/stddef.h"

/** @defgroup vfs vfs.h
 *
 * Virtual file system.
 * @{
 */

/** An entry in a descriptor like directories, streams, ... . */
typedef void MFS_entry_t;

/** Descriptor type. */
typedef struct MFS_descriptor MFS_descriptor_t;	

/** Descriptor interface. */
struct MFS_descriptor_op
{
	void (*open)(MFS_entry_t *entry);      /**< Open. */
	void (*close)(MFS_entry_t *entry);     /**< Close. */
	void (*info)(MFS_entry_t *entry);      /**< Info. */
};

/** Super type. */
typedef struct MFS_super MFS_super_t;

/** Super interface. */
struct MFS_super_op
{
	void (*mount)(MFS_super_t *super);     /**< Mount. */
	void (*umount)(MFS_super_t *super);    /**< Unmount. */
};

/** Directory type. */
typedef struct MFS_directory MFS_directory_t;

/** Directory interface. */
struct MFS_directory_op
{
	void (*open)(MFS_directory_t *dir);          /**< Open. */
	void (*close)(MFS_directory_t *dir);         /**< Close. */
	void (*create)(MFS_descriptor_t *desc);      /**< Create. */
	void (*remove)(MFS_descriptor_t *desc);      /**< Remove. */
	void (*rename)(MFS_descriptor_t *desc);      /**< Rename. */
};

/** Stream type. */
typedef struct MFS_stream MFS_stream_t;

/** Stream interface. */
struct MFS_stream_op
{
	void (*open)(MFS_stream_t *stream);             /**< Open. */
	void (*close)(MFS_stream_t *stream);            /**< Close. */
	void (*info)(MFS_stream_t *stream);             /**< Info. */
	ACE_size_t (*read)(MFS_stream_t *stream, char *buf, ACE_size_t len);            /**< Read. */
	ACE_size_t (*write)(MFS_stream_t *stream, const char *buf, ACE_size_t len);     /**< Write. */
	int (*seek)(MFS_stream_t *stream, ACE_ssize_t off, ACE_size_t pos);             /**< Seek. */
	void (*flush)(MFS_stream_t *stream);                                            /**< Flush. */
};

/** Virtual file system interface. */
struct MFS_vfs_op
{
	struct MFS_super_op *super_op;       /**< Super interface. */
	struct MFS_directory_op *dir_op;     /**< Directory interface. */
	struct MFS_stream_op *stream_op;     /**< Stream interface. */
};

/** @}
 */

#endif
