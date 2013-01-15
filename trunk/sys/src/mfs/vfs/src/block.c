#include <ace/stdio.h>
#include <ace/stdlib.h>
#include <uso/heap.h>
#include <mfs/block.h>
#include <mfs/descriptor.h>
#include <mfs/err.h>



extern ACE_ssize_t
MFS_get (MFS_block_t *block, char **buf, ACE_size_t number)
{
    if (block->operations->get != NULL){
    	return block->operations->get((MFS_descriptor_t *)block, buf, number);
    } else {
    	*buf = NULL;
    	return 0;
    }
}

extern ACE_err_t
MFS_put (MFS_block_t *block, char *buf, ACE_size_t len, ACE_size_t number)
{
    return (block->operations->put != NULL) ? block->operations->put ((MFS_descriptor_t *)block, buf, len, number) : ACE_OK;
}

extern ACE_err_t
MFS_confirm (MFS_block_t * block, ACE_size_t number)
{
    return (block->operations->confirm != NULL) ? block->operations->confirm ((MFS_descriptor_t *)block, number) : ACE_OK;
}

extern void
MFS_block_init (MFS_block_t * block,
                 enum MFS_block_type type,
                 struct MFS_block_op *operations)
{
    block->type = type;
    block->start = 0;
    block->end = 0;
    block->size = 0;
    block->operations = operations;
}

extern MFS_descriptor_t *
MFS_block_create (MFS_descriptor_t * dir_desc, char *name, struct MFS_descriptor_op *desc_op,
					  struct MFS_block_op *block_op, MFS_represent_t * represent, enum MFS_block_type type)
{
	MFS_block_t *block = NULL;
    if (dir_desc->type == MFS_DIRECTORY)
    {
        block = ACE_malloc (sizeof (MFS_block_t));
        if (block != NULL)
        {
        	MFS_descriptor_init((MFS_descriptor_t *)block, represent, desc_op, name, MFS_BLOCK, dir_desc);
            MFS_block_init (block, type, block_op);
            MFS_create_desc (dir_desc, (MFS_descriptor_t *)block);
        }
    }
    return (MFS_descriptor_t *)block;
}

extern void
MFS_block_print (MFS_block_t * block)
{
    char *type;
    switch (block->type)
    {
    case MFS_BLOCK_IO:
        type = "io";
        break;
    case MFS_DISK:
    default:
        type = "disc";
        break;
    }
    ACE_size_t ts = (block->end - block->start) * block->size;
    ACE_printf ("%s: start: %lu, end: %lu, block_size: %lu, total_size: %lu\n",
    		type, block->start, block->end, block->size, ts);
}
