/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

#include <ace/stdio.h>

#include <uso/heap.h>
#include <uso/debug.h>
#include <uso/cpu.h>

#include <mfs/directory.h>
#include <mfs/sysfs.h>

#define USO_HEAP_MIN_BLOCK_SIZE   24    /* 24 + 4 + 4 = 32 (nextf, prevf) */

/*------------- Implementation ------------------------------------------*/

/*
 * The USO_kprintf functions are used. So these functions are not allowed
 * to block. 
 */

/*
 * Next fit algorithm! 
 */

/*
 * The mblock list is sorted by location. 
 */
struct USO_mblock
{
    USO_mblock_t *nextm;
    USO_mblock_t *prevm;
    long tag_size;              /* highest bit is the used tag, so size is signed. */
};

/*
 * The fblock list is not sorted but circular. 
 */
struct USO_fblock
{
    USO_mblock_t head;
    USO_fblock_t *nextf;
    USO_fblock_t *prevf;
};


static void
calc_search_average (USO_heap_t * heap)
{
    unsigned long ac = heap->alloc_count;
    unsigned long sc = heap->search_count;
    if ((ac == 0) || (ac > sc))
    {
        heap->search_count = 0;
        heap->alloc_count = 0;
        return;
    }
    heap->search_average = sc / ac;
}



static void
mblock_insert (USO_mblock_t * prev, USO_mblock_t * block, USO_mblock_t * next, long size)
{
    if (prev != NULL)
    {
        prev->nextm = block;
    }
    if (next != NULL)
    {
        next->prevm = block;
    }
    block->nextm = next;
    block->prevm = prev;
    block->tag_size = size;
}

static void
mblock_remove (USO_mblock_t * block)
{
    if (block->nextm != NULL)
    {
        block->nextm->prevm = block->prevm;
    }
    if (block->prevm != NULL)
    {
        block->prevm->nextm = block->nextm;
    }
}


static void
fblock_insert (USO_heap_t * heap, USO_fblock_t * block)
{
    if (heap->free_blocks == NULL)
    {
        block->nextf = block;
        block->prevf = block;
        heap->free_blocks = block;
    }
    else
    {
        block->nextf = heap->free_blocks;
        block->prevf = heap->free_blocks->prevf;
        heap->free_blocks->prevf->nextf = block;
        heap->free_blocks->prevf = block;
    }
}

static void
fblock_remove (USO_heap_t * heap, USO_fblock_t * block)
{
    if (block->nextf == block)
    {
        heap->free_blocks = NULL;
    }
    else
    {
        block->prevf->nextf = block->nextf;
        block->nextf->prevf = block->prevf;
        heap->free_blocks = block->nextf;
    }
}

#define MINBLOCKSIZE (sizeof(USO_fblock_t) - sizeof(USO_mblock_t) + USO_HEAP_MIN_BLOCK_SIZE)

#define USED_TAG 0x80000000L
#define ROUND 0x4L

#if USO_HEAP_LIST_DEBUG
static void
print_heap_list (USO_heap_t * heap)
{
    USO_mblock_t *block;
    unsigned long mem_blocks = (unsigned long)heap->mem_blocks;
    USO_log_printf (USO_LL_INFO,
                 "\n--------Start---------\n"
                 "Free Blocks: %ld\n"
                 "Alloc: %lu, Search: %lu\n",
                 heap->free_blocks != NULL ?
                 (unsigned long)heap->free_blocks - mem_blocks : -1,
                 heap->alloc_count, heap->search_count);
    for (block = heap->mem_blocks; block != NULL; block = block->nextm)
    {
        long nextm;
        long prevm;
        long size;
        int tag;
        nextm = block->nextm != NULL ? (unsigned long)block->nextm - mem_blocks : -1;
        prevm = block->prevm != NULL ? (unsigned long)block->prevm - mem_blocks : -1;
        size = block->tag_size & ~USED_TAG;
        tag = block->tag_size & USED_TAG ? 1 : 0;
        USO_log_printf (USO_LL_INFO,
                     "nextm: %ld\n"
                     "prevm: %ld\n" "size: %ld\n" "tag: %d\n", nextm, prevm, size, tag);
        if (tag == 0)
        {
            long nextf;
            long prevf;
            nextf = ((USO_fblock_t *) block)->nextf != NULL ?
                (unsigned long)((USO_fblock_t *) block)->nextf - mem_blocks : -1;
            prevf = ((USO_fblock_t *) block)->prevf != NULL ?
                (unsigned long)((USO_fblock_t *) block)->prevf - mem_blocks : -1;
            USO_log_printf (USO_LL_INFO, "\tnextf: %ld\n" "\tprevf: %ld\n", nextf, prevf);
        }
    }
    USO_log_printf (USO_LL_INFO, "--------End-----------\n");
}
#endif

extern void
USO_debug_heap_list (MFS_represent_t * represent)
{
    USO_log_printf (USO_LL_INFO, "\n Debug switch is %s.\n", USO_HEAP_LIST_DEBUG ? "on" : "off");
#if USO_HEAP_LIST_DEBUG
    if (represent != NULL)
        print_heap_list ((USO_heap_t *) represent);
#endif
}


static void
info (MFS_descriptor_t * desc, int number, MFS_info_entry_t *entry)
{
    USO_heap_t *heap = (USO_heap_t *) desc->represent;
    switch (number){
        case 0:
            entry->type = MFS_INFO_SIZE;
            entry->name = "Total";
            entry->value.z = heap->total_mem;
            break;
        case 1:
            entry->type = MFS_INFO_SIZE;
            entry->name = "Free";
            entry->value.z = heap->free_mem;
            break;
        case 2:
            entry->type = MFS_INFO_SIZE;
            entry->name = "AV search";
            entry->value.z = heap->search_average;
            break;
        default:
            entry->type = MFS_INFO_NOT_AVAIL;
            break;
    }
}

static struct MFS_descriptor_op heap_descriptor_op = {
    .open = NULL,
    .close = NULL,
    .info = info,
    .control = NULL,
    .delete = NULL
};

extern void
USO_heap_install (USO_heap_t * heap, char *name)
{
    MFS_descriptor_t *dir = MFS_resolve("/sys/uso/heap");
    MFS_descriptor_create (dir, name, MFS_SYS, &heap_descriptor_op, heap);
    MFS_close_desc(dir);
}

extern ACE_bool_t
USO_heap_init (USO_heap_t * heap, void *start, void *end)
{
    heap->mem_blocks = NULL;
    heap->free_blocks = NULL;
    heap->free_mem = 0;
    heap->total_mem = 0;
    heap->alloc_count = 0;
    heap->search_count = 0;
    heap->search_average = 0;
    long size;
    size = (char *)end - (char *)start;
    size &= ~(ROUND - 1);
    if (size < MINBLOCKSIZE + sizeof (USO_mblock_t))
    {
        DEBUGF (USO_HEAP_DEBUG, ("Heap init failed!\n"));
        return FALSE;
    }
    mblock_insert (NULL, (USO_mblock_t *) start, NULL, (unsigned long)size - sizeof (USO_mblock_t));
    heap->mem_blocks = (USO_mblock_t *) start;
    fblock_insert (heap, (USO_fblock_t *) start);
    heap->total_mem = size;
    heap->free_mem = heap->free_blocks->head.tag_size;
    return TRUE;
}

extern void *
USO_mem_alloc (USO_heap_t * heap, ACE_size_t size)
{
    USO_fblock_t *free;
    USO_mblock_t *used;
    long leftover;
    if (size < MINBLOCKSIZE)
        size = MINBLOCKSIZE;
    if (size & (ROUND - 1))
    {
        size += ROUND;
        size &= ~(ROUND - 1);
    }
    USO_cpu_status_t ps = USO_disable ();
    if (heap->free_blocks == NULL)
    {
        DEBUGF (USO_HEAP_DEBUG, ("Mem alloc: Out of free memmory!\n"));
        USO_restore (ps);
        return NULL;
    }
    free = heap->free_blocks;
    ++heap->alloc_count;
    do
    {
        ++heap->search_count;
        if (free->head.tag_size >= size)
        {
            if ((leftover = free->head.tag_size - (long)size) >=
                (MINBLOCKSIZE + sizeof (USO_mblock_t)))
            {
                free->head.tag_size = leftover - sizeof (USO_mblock_t);
                used = (USO_mblock_t *) ((char *)free + leftover);
                mblock_insert ((USO_mblock_t *) free, used, ((USO_mblock_t *) free)->nextm, size);
                heap->free_blocks = free->nextf;
                heap->free_mem -= sizeof (USO_mblock_t);
            }
            else
            {
                fblock_remove (heap, free);
                used = (USO_mblock_t *) free;
            }
            heap->free_mem -= used->tag_size;
            calc_search_average (heap);
            used->tag_size |= USED_TAG;
            USO_restore (ps);
            return ++used;
        }
        free = free->nextf;
    }
    while (free != heap->free_blocks);
    calc_search_average (heap);
    DEBUGF (USO_HEAP_DEBUG, ("Mem alloc: Block to large(free:%ld/block:%ld)!\n",
                             heap->free_mem, (long)size));
    USO_restore (ps);
    return NULL;
}

static ACE_bool_t
prev_block_free (USO_mblock_t * block)
{
    return ((block->prevm != NULL) && (!(block->prevm->tag_size & USED_TAG))) ? TRUE : FALSE;
}

static ACE_bool_t
next_block_free (USO_mblock_t * block)
{
    return ((block->nextm != NULL) && (!(block->nextm->tag_size & USED_TAG))) ? TRUE : FALSE;
}

extern void
USO_mem_free (USO_heap_t * heap, void *block)
{
    USO_mblock_t *used;
    USO_mblock_t *ntemp;
    USO_mblock_t *ptemp;
    USO_mblock_t *next;
    USO_mblock_t *prev;
    long size;

    used = (USO_mblock_t *) block;      // block points to allocated space
    --used;                     // now used points to the mblock
    used->tag_size &= ~USED_TAG;

    USO_cpu_status_t ps = USO_disable ();
    heap->free_mem += used->tag_size;

    if ((prev_block_free (used) == TRUE) && (next_block_free (used) == TRUE))
    {
        ptemp = used->prevm;
        ntemp = used->nextm;
        fblock_remove (heap, (USO_fblock_t *) ntemp);
        size = ptemp->tag_size + used->tag_size + ntemp->tag_size + 2 * sizeof (USO_mblock_t);
        next = ntemp->nextm;
        prev = ptemp->prevm;
        mblock_remove (ntemp);
        mblock_remove (ptemp);
        mblock_remove (used);
        mblock_insert (prev, ptemp, next, size);
        heap->free_mem += (2 * sizeof (USO_mblock_t));
    }
    else if (prev_block_free (used) == TRUE)
    {
        ptemp = used->prevm;
        size = ptemp->tag_size + used->tag_size + sizeof (USO_mblock_t);
        next = used->nextm;
        prev = ptemp->prevm;
        mblock_remove (ptemp);
        mblock_remove (used);
        mblock_insert (prev, ptemp, next, size);
        heap->free_mem += sizeof (USO_mblock_t);
    }
    else if (next_block_free (used) == TRUE)
    {
        ntemp = used->nextm;
        fblock_remove (heap, (USO_fblock_t *) ntemp);
        fblock_insert (heap, (USO_fblock_t *) used);
        size = used->tag_size + ntemp->tag_size + sizeof (USO_mblock_t);
        next = ntemp->nextm;
        prev = used->prevm;
        mblock_remove (ntemp);
        mblock_remove (used);
        mblock_insert (prev, used, next, size);
        heap->free_mem += sizeof (USO_mblock_t);
    }
    else
    {
        fblock_insert (heap, (USO_fblock_t *) used);
    }

    USO_restore (ps);
}

/*------------------------------------------------------------------------*/
