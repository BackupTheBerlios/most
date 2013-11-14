#include <ace/stdio.h>
#include <ace/stdlib.h>
#include <ace/assert.h>
#include <uso/heap.h>
#include <mfs/block.h>
#include <mfs/descriptor.h>
#include <mfs/err.h>



extern ACE_ssize_t
MFS_get (MFS_block_t *block, char **buf, ACE_size_t number)
{
    ACE_SANITY_CHECK(block);
    if (block->type == MFS_DISK){
        if ( (number < block->start) || (number >= block->end) ) return DEF_ERR_RANGE;
    }
    if (block->operations->get != NULL){
        return block->operations->get(block, buf, number);
    } else {
        *buf = NULL;
        return 0;
    }
}

extern ACE_err_t
MFS_put (MFS_block_t *block, char *buf, ACE_size_t len, ACE_size_t number)
{
    ACE_SANITY_CHECK(block);
    if (block->type == MFS_DISK){
        if ( (number < block->start) || (number >= block->end) ) return DEF_ERR_RANGE;
    }
    if (block->size > 0){
        if (len != block->size) return DEF_ERR_SIZE;
    }
    return (block->operations->put != NULL) ? block->operations->put (block, buf, len, number) : ACE_OK;
}

extern ACE_err_t
MFS_confirm (MFS_block_t * block, ACE_size_t number)
{
    ACE_SANITY_CHECK(block);
    if (block->type == MFS_DISK){
        if ( (number < block->start) || (number >= block->end) ) return DEF_ERR_RANGE;
    }
    return (block->operations->confirm != NULL) ? block->operations->confirm (block, number) : ACE_OK;
}

extern void
MFS_block_init (MFS_block_t * block,
                 enum MFS_block_type type,
                 struct MFS_block_op *operations)
{
    ACE_SANITY_CHECK(block);
    block->type = type;
    block->start = 0;
    block->end = 0;
    block->size = 0;
    block->operations = operations;
}

extern MFS_descriptor_t *
MFS_block_create (MFS_descriptor_t * dir, char *name, struct MFS_descriptor_op *desc_op,
                struct MFS_block_op *block_op, MFS_represent_t * represent, enum MFS_block_type type)
{
    MFS_block_t *block = NULL;
    if (dir && (dir->type == MFS_DIRECTORY))
    {
        block = ACE_malloc (sizeof (MFS_block_t));
        if (block != NULL)
        {
            MFS_descriptor_init((MFS_descriptor_t *)block, represent, desc_op, name, MFS_BLOCK, dir);
            MFS_block_init (block, type, block_op);
            MFS_create_desc (dir, (MFS_descriptor_t *)block);
        }
    }
    return (MFS_descriptor_t *)block;
}

extern void
MFS_block_info (MFS_block_t * block, int number, MFS_info_entry_t *entry)
{
    ACE_SANITY_CHECK(block);
    switch (number){
       case 0:
           entry->type = MFS_INFO_STRING;
           entry->name = "type";
           if (block->type == MFS_BLOCK_IO){
               entry->value.s = "io";
           } else {
               entry->value.s = "disc";
           }
           break;
       case 1:
           entry->type = MFS_INFO_SIZE;
           entry->name = "start";
           entry->value.z = block->start;
           break;
       case 2:
           entry->type = MFS_INFO_SIZE;
           entry->name = "end";
           entry->value.z = block->end;
           break;
       case 3:
           entry->type = MFS_INFO_SIZE;
           entry->name = "size";
           entry->value.z = block->size;
           break;
       case 4:
           entry->type = MFS_INFO_SIZE;
           entry->name = "total";
           entry->value.z = (block->end - block->start) * block->size;
           break;
       default:
           entry->type = MFS_INFO_NOT_AVAIL;
           break;
   }
}
