#include <ace/stdio.h>
#include <ace/stdlib.h>
#include <ace/assert.h>
#include <uso/heap.h>
#include <mfs/stream.h>
#include <mfs/directory.h>
#include <mfs/err.h>



extern ACE_size_t
MFS_read (MFS_stream_t * stream, char *buf, ACE_size_t len)
{
    ACE_SANITY_CHECK(stream);
    return (stream->operations->read != NULL) ? stream->operations->read (stream, buf, len) : ACE_EOF;
}

extern ACE_size_t
MFS_write (MFS_stream_t * stream, char *buf, ACE_size_t len)
{
    ACE_SANITY_CHECK(stream);
    return (stream->operations->write != NULL) ? stream->operations->write (stream, buf, len) : ACE_EOF;
}

extern int
MFS_seek (MFS_stream_t * stream, ACE_ssize_t off, ACE_size_t pos)
{
    ACE_SANITY_CHECK(stream);
    return (stream->operations->seek != NULL) ? stream->operations->seek (stream, off, pos) : ACE_EOF;
}

extern void
MFS_flush (MFS_stream_t * stream)
{
    ACE_SANITY_CHECK(stream);
    if (stream->operations->flush != NULL) stream->operations->flush (stream);
}



extern void
MFS_stream_init (MFS_stream_t * stream,
                 enum MFS_stream_type type,
                 struct MFS_stream_op *operations)
{
    ACE_SANITY_CHECK(stream);
    stream->type = type;
    stream->size_tx = 0;
    stream->pos_rx = 0;
    stream->operations = operations;
}

extern MFS_descriptor_t *
MFS_stream_create (MFS_descriptor_t * dir, char *name, struct MFS_descriptor_op *desc_op,
                struct MFS_stream_op *stream_op, MFS_represent_t * represent, enum MFS_stream_type type)
{
    MFS_stream_t *stream = NULL;
    if (dir && (dir->type == MFS_DIRECTORY))
    {
        stream = ACE_malloc (sizeof (MFS_stream_t));
        if (stream != NULL)
        {
            MFS_descriptor_init((MFS_descriptor_t *)stream, represent, desc_op, name, MFS_STREAM, dir);
            MFS_stream_init (stream, type, stream_op);
            MFS_create_desc (dir, (MFS_descriptor_t *)stream);
        }
    }
    return (MFS_descriptor_t *)stream;
}

extern void
MFS_stream_info (MFS_stream_t * stream, int number, MFS_info_entry_t *entry)
{
    ACE_SANITY_CHECK(stream);
    switch (number){
        case 0:
            entry->type = MFS_INFO_STRING;
            entry->name = "type";
            if (stream->type == MFS_STREAM_IO) {
                entry->value.s = "io";
            } else {
                entry->value.s = "file";
            }
            break;
        case 1:
            entry->type = MFS_INFO_SIZE;
            if (stream->type == MFS_STREAM_IO) {
                entry->name = "tx";
            } else {
                entry->name = "size";
            }
            entry->value.z = stream->size_tx;
            break;
        case 2:
            entry->type = MFS_INFO_SIZE;
            if (stream->type == MFS_STREAM_IO) {
                entry->name = "rx";
            } else {
                entry->name = "pos";
            }
            entry->value.z = stream->pos_rx;
            break;
        default:
            entry->type = MFS_INFO_NOT_AVAIL;
            break;
    }
}
