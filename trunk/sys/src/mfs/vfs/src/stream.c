#include <ace/stdio.h>
#include <ace/stdlib.h>
#include <uso/heap.h>
#include <mfs/stream.h>
#include <mfs/directory.h>
#include <mfs/err.h>



extern ACE_size_t
MFS_read (MFS_stream_t * stream, char *buf, ACE_size_t len)
{
    return (stream->operations->read != NULL) ? stream->operations->read ((MFS_descriptor_t *)stream, buf, len) : ACE_EOF;
}

extern ACE_size_t
MFS_write (MFS_stream_t * stream, char *buf, ACE_size_t len)
{
    return (stream->operations->write != NULL) ? stream->operations->write ((MFS_descriptor_t *)stream, buf, len) : ACE_EOF;
}

extern int
MFS_seek (MFS_stream_t * stream, ACE_ssize_t off, ACE_size_t pos)
{
    return (stream->operations->seek != NULL) ? stream->operations->seek ((MFS_descriptor_t *)stream, off, pos) : ACE_EOF;
}

extern void
MFS_flush (MFS_stream_t * stream)
{
    if (stream->operations->flush != NULL) stream->operations->flush ((MFS_descriptor_t *)stream);
}



extern void
MFS_stream_init (MFS_stream_t * stream,
                 enum MFS_stream_type type,
                 struct MFS_stream_op *operations)
{
    stream->type = type;
    stream->size_tx = 0;
    stream->pos_rx = 0;
    stream->operations = operations;
}

extern MFS_descriptor_t *
MFS_stream_create_file (MFS_descriptor_t * dir_desc, char *name)
{
    MFS_stream_t *file = NULL;
    if (dir_desc->type == MFS_DIRECTORY)
    {
        file = ACE_malloc (sizeof (MFS_stream_t));
        if (file != NULL)
        {
            MFS_directory_t *dir = (MFS_directory_t *) dir_desc;
        	MFS_descriptor_init((MFS_descriptor_t *)file, NULL, dir->vfs_op->stream_desc_op, name, MFS_STREAM, dir_desc);
            MFS_stream_init (file, MFS_FILE, dir->vfs_op->stream_op);
            MFS_create_desc (dir_desc, (MFS_descriptor_t *)file);
        }
    }
    return (MFS_descriptor_t *)file;
}

extern MFS_descriptor_t *
MFS_stream_create_io (MFS_descriptor_t * dir_desc, char *name, struct MFS_descriptor_op *desc_op,
					  struct MFS_stream_op *io_op, MFS_represent_t * represent)
{
	MFS_stream_t *io = NULL;
    if (dir_desc->type == MFS_DIRECTORY)
    {
        io = ACE_malloc (sizeof (MFS_stream_t));
        if (io != NULL)
        {
        	MFS_descriptor_init((MFS_descriptor_t *)io, represent, desc_op, name, MFS_STREAM, dir_desc);
            MFS_stream_init (io, MFS_STREAM_IO, io_op);
            MFS_create_desc (dir_desc, (MFS_descriptor_t *)io);
        }
    }
    return (MFS_descriptor_t *)io;
}

extern void
MFS_stream_print (MFS_stream_t * stream)
{
    char *type;
    char *size_tx;
    char *pos_rx;
    switch (stream->type)
    {
    case MFS_STREAM_IO:
        type = "io";
        size_tx = "tx";
        pos_rx = "rx";
        break;
    case MFS_FILE:
    default:
        type = "file";
        size_tx = "size";
        pos_rx = "pos";
        break;
    }
    ACE_printf ("%s %s: %lu %s: %lu open: %i\n", type, size_tx, stream->size_tx, pos_rx, stream->pos_rx);
}
