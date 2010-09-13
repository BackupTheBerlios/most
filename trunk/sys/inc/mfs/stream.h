/*
 *  Copyright (C) 2007  Michael Feiner
 *
 */

#ifndef MFS_STREAM_H
#define MFS_STREAM_H

#include "uso/mutex.h"
#include "mfs/vfs.h"

/** @defgroup stream stream.h
 *
 * A stream performs character io. A file is a stream and also for example e serial device.
 * @{
 */

/** Stream representation type.
 */
typedef void MFS_stream_represent_t;

/** Stream descriptor operations instance. */
extern struct MFS_descriptor_op MFS_stream_descriptor_op;

/** Stream types. */
enum MFS_stream_type
{
	MFS_FILE,       /**< FILE. */
	MFS_IO          /**< IO. */
};


/* Stream structure. */
struct MFS_stream
{
	enum MFS_stream_type type;         /* Stream type. */
	ACE_size_t size_tx;                /* File size, for IO the amount of transmitted characters. */
	ACE_size_t pos_rx;                 /* File position, for IO the amount of received characters. */
	USO_mutex_t lock;                    /* Synchronization lock. */
	struct MFS_stream_op *operations;    /* Stream operations. */
	MFS_stream_represent_t *represent;   /* Realized stream. */
};


/**
 * Read data from stream.
 *
 * @param stream : Stream to read from.
 * @param buf : Buffer into the data will be written.
 * @param len : Size of the buffer or max length of data for reading.
 * @return Length of the read data.
 */
extern ACE_size_t MFS_read(MFS_stream_t *stream, char *buf, ACE_size_t len);

/**
 * Write data to stream.
 *
 * @param stream : Stream type.
 * @param buf : Buffer of data to write.
 * @param len : Length of the data to write.
 * @return Length of written data.
 */
extern ACE_size_t MFS_write(MFS_stream_t *stream, char *buf, ACE_size_t len);

/**
 * Change the file position.
 * This has no effect on character io devices.
 *
 * @param stream : Stream type.
 * @param off : Offset relative to pos.
 * @param pos : SEEK_SET for beginning of the file, SEEK_CUR for current position, SEEK_END for end of file.
 * @return 0 == OK, 0 != ERROR.
 */
extern   int MFS_seek(MFS_stream_t *stream, ACE_ssize_t off, ACE_size_t pos);

/**
 * Flush all buffers to disk.
 *
 * @param stream : Stream type.
 */
extern void MFS_flush(MFS_stream_t *stream);

/**
 * Get stream from stream descriptor.
 *
 * @param stream : Stream type.
 * @return .
 */
extern MFS_stream_t *MFS_get_stream (MFS_descriptor_t *stream_desc);

/**
 * Initialize a stream.
 *
 * @param stream : Stream type.
 * @param type : MFS_FILE or MFS_IO.
 * @param operations : The stream operations.
 * @param represent : The stream representation.
 */
extern void MFS_stream_init(MFS_stream_t *stream,
								enum MFS_stream_type type,
								struct MFS_stream_op *operations,
								MFS_stream_represent_t *represent);

/**
 * Dummy read function.
 * @return Always EOF.
 */
extern ACE_size_t MFS_empty_read(MFS_stream_t *stream, char *buf, ACE_size_t len);

/**
 * Dummy write function.
 *
 * @return Always EOF (does write return EOF?).
 */
extern ACE_size_t MFS_empty_write(MFS_stream_t *stream, const char *buf, ACE_size_t len);

/** @}
 */

#endif
