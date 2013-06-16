/*
 * free.h
 *
 *  Created on: 08.04.2013
 *      Author: maik
 */

#ifndef SFS_FREE_H_
#define SFS_FREE_H_

#include <sfs.h>

#define ENTRY_SIZE       32
#define FREE_TAB_SIZE    (SFS_BLOCK_SIZE / sizeof(long))
#define ENTRY_TAB_SIZE   (FREE_TAB_SIZE / ENTRY_SIZE)
#define ENTRY_INIT_VALUE 0xFFFFFFFF

#define ENTRY_TAB_START  0
#define ENTRY_TAB_END    (ENTRY_TAB_START + ENTRY_TAB_SIZE)
#define FREE_TAB_START   0
#define FREE_TAB_END     (FREE_TAB_START + FREE_TAB_SIZE)

struct SFS_free
{
	MFS_block_t *dev;               /* block dev */
	unsigned long nr;                 /* block number containing the free blocks bitfield */
	long entry_tab[ENTRY_TAB_SIZE];   /* each bit marks one entry(32 Blocks) in the free tab : 1 ... one of the 32 entries blocks is free*/
	long free_tab[FREE_TAB_SIZE];     /* each bit marks one block : 1 ... block is free, 0 ... block is used */
};

typedef struct SFS_free SFS_free_t;


/* get a free block */
extern long SFS_free_get(SFS_free_t *free);

/* put a used block to free blocks */
extern ACE_err_t SFS_free_put(SFS_free_t *free, long used);

/* format free blocks bitfield with all blocks free */
extern void SFS_free_format(SFS_free_t *free, MFS_block_t *dev, unsigned long nr);

/* initialize free blocks bitfield with stored information on disk */
extern void SFS_free_mount(SFS_free_t *free, MFS_block_t *dev, unsigned long nr);

/* synchronize free blocks bitfield with disk */
extern void SFS_free_sync(SFS_free_t *free);



#endif /* SFS_FREE_H_ */
