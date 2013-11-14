/*
 *  Copyright (C) 2007  Michael Feiner
 *
 */

#ifndef MFS_STREAM_H
#define MFS_STREAM_H

#include <mfs/vfs.h>
#include <mfs/descriptor.h>

/** @addtogroup mfs
 * @{
 */

/** @defgroup stream stream.h
 *
 * A stream performs character io. A file is a stream and also for example e serial device.
 * @{
 */


/** Stream descriptor operations instance. */
extern struct MFS_descriptor_op MFS_stream_descriptor_op;

/** Stream types. */
enum MFS_stream_type
{
    MFS_FILE,                  /**< FILE. */
    MFS_STREAM_IO              /**< IO. */
};


/* Stream structure. */
struct MFS_stream
{
    MFS_descriptor_t desc;
    enum MFS_stream_type type;  /* Stream type. */
    ACE_size_t size_tx;         /* File size, for IO the amount of transmitted characters. */
    ACE_size_t pos_rx;               /* File position, for IO the amount of received characters. */
    struct MFS_stream_op *operations;   /* Stream operations. */
};


/**
 * Read data from stream.
 *
 * @param stream : Stream to read from.
 * @param buf : Buffer into the data will be written.
 * @param len : Size of the buffer or max length of data for reading.
 * @return Length of the read data.
 */
extern ACE_size_t MFS_read (MFS_stream_t * stream, char *buf, ACE_size_t len);

/**
 * Write data to stream.
 *
 * @param stream : Stream type.
 * @param buf : Buffer of data to write.
 * @param len : Length of the data to write.
 * @return Length of written data.
 */
extern ACE_size_t MFS_write (MFS_stream_t * stream, char *buf, ACE_size_t len);

/**
 * Change the file position.
 * This has no effect on character io devices.
 *
 * @param stream : Stream type.
 * @param off : Offset relative to pos.
 * @param pos : SEEK_SET for beginning of the file, SEEK_CUR for current position, SEEK_END for end of file.
 * @return 0 == OK, 0 != ERROR.
 */
extern int MFS_seek (MFS_stream_t * stream, ACE_ssize_t off, ACE_size_t pos);

/**
 * Flush all buffers to disk.
 *
 * @param stream : Stream type.
 */
extern void MFS_flush (MFS_stream_t * stream);


/**
 * Initialize a stream.
 *
 * @param stream : Stream type.
 * @param type : MFS_FILE or MFS_STREAM_IO.
 * @param operations : The stream operations.
 * @param represent : The stream representation.
 */
extern void MFS_stream_init (MFS_stream_t * stream,
                             enum MFS_stream_type type,
                             struct MFS_stream_op *operations);

/**
 * Create an stream.
 * The IO stream has to implement the stream operation interface.
 * @param dir_desc : Directory descriptor in which a new io descriptor is created.
 * @param name : Name for the io.
 * @param desc_op : descriptor operations for the stream.
 * @param stream_op : operations for the stream.
 * @param represent : Stream representation.
 * @param type : io || file.
 * @return Created stream descriptor.
 */
extern MFS_descriptor_t * MFS_stream_create (
                MFS_descriptor_t * dir, char *name, struct MFS_descriptor_op *desc_op,
                struct MFS_stream_op *stream_op, MFS_represent_t * represent, enum MFS_stream_type type);


extern void MFS_stream_info (MFS_stream_t * stream, int number, MFS_info_entry_t *entry);

/** @}
 */

/** @}
 */

#endif
