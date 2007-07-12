#include "ace/stdio.h"
#include "uso/heap.h"
#include "mfs/stream.h"
#include "mfs/descriptor.h"

static void stream_open(MFS_entry_t *entry)
{
	MFS_stream_t * stream = (MFS_stream_t *)entry;
	if (stream->operations->open != NULL) {stream->operations->open(stream);}
}

static void stream_close(MFS_entry_t *entry)
{
	MFS_stream_t * stream = (MFS_stream_t *)entry;
	if (stream->operations->close != NULL) {stream->operations->close(stream);}
}

static void stream_info(MFS_entry_t *entry)
{
	MFS_stream_t * stream = (MFS_stream_t *)entry;
	char *type;
	char *size_tx;
	char *pos_rx;
	switch (stream->type){
		case MFS_IO:
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
	printf("%s %s: %lu %s: %lu\n", type, size_tx, stream->size_tx, pos_rx, stream->pos_rx);
	if (stream->operations->info != NULL) {stream->operations->info(stream);}
}


struct MFS_descriptor_op MFS_stream_descriptor_op = {.open = stream_open,
											 .close = stream_close,
										     .info = stream_info};




extern size_t
MFS_read(MFS_stream_t *stream, char *buf, size_t len)
{
	return stream->operations->read(stream, buf, len);
}

extern size_t
MFS_write(MFS_stream_t *stream, char *buf, size_t len)
{
	return stream->operations->write(stream, buf, len);
}

extern int
MFS_seek(MFS_stream_t *stream, ssize_t off, size_t pos)
{
	return (stream->operations->seek != NULL) ? stream->operations->seek(stream, off, pos) : 0;
}

extern void
MFS_flush(MFS_stream_t *stream)
{
	if (stream->operations->flush != NULL) {stream->operations->flush(stream);}
}

extern MFS_stream_t *
MFS_get_stream (MFS_descriptor_t *stream_desc)
{
	MFS_stream_t *stream = NULL;
	if (stream_desc->type == MFS_STREAM){
		stream = (MFS_stream_t *)stream_desc->entry;
	}
	return stream;
}


extern void
MFS_stream_init(MFS_stream_t *stream,
								enum MFS_stream_type type,
								struct MFS_stream_op *operations,
								MFS_stream_represent_t *represent)
{
	stream->type = type;
	stream->size_tx = 0;
	stream->pos_rx = 0;
	stream->operations = operations;
	USO_mutex_init(&stream->lock);
	stream->represent = represent;
}

extern size_t
MFS_empty_read(MFS_stream_t *stream, char *buf, size_t len)
{
	return EOF;
}

extern size_t
MFS_empty_write(MFS_stream_t *stream, const char *buf, size_t len)
{
	return EOF;
}
