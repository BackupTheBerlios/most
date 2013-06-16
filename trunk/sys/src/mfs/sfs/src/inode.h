/*
 * inode.h
 *
 *  Created on: 08.04.2013
 *      Author: maik
 */

#ifndef SFS_INODE_H_
#define SFS_INODE_H_

#include <sfs.h>

#define INODE_HEADER_SIZE   32       /** todo use real header size */
#define DATA_TAB_SIZE       ( (SFS_BLOCK_SIZE - INODE_HEADER_SIZE) / sizeof(long) )

#define DATA_TAB_START   0
#define DATA_TAB_END     (DATA_TAB_START + DATA_TAB_SIZE)

struct SFS_inode_block
{
	ACE_u32_t size;                        /* size of data in bytes */
	ACE_u32_t data_tab[DATA_TAB_SIZE];     /* table containing the block numbers for data blocks */
};

struct SFS_inode
{
	MFS_block_t *dev;                 /* block dev */
	unsigned long nr;                 /* block number containing the inode */
	long current;                     /* current data block */
	struct SFS_inode_block block;     /* inode block on disc */
};

typedef struct SFS_inode SFS_inode_t;


/* get a data block */
extern long SFS_inode_get(SFS_inode_t *inode);

/* put a data block */
extern ACE_err_t SFS_inode_put(SFS_inode_t *inode, long used);

/* format inode to empty file pointing to one data block*/
extern void SFS_inode_format(SFS_inode_t *inode, MFS_block_t *dev, unsigned long nr);

/* initialize inode with stored information on disk */
extern void SFS_inode_mount(SFS_inode_t *inode, MFS_block_t *dev, unsigned long nr);

/* synchronize inode with disk */
extern void SFS_inode_sync(SFS_inode_t *inode);


#endif /* SFS_INODE_H_ */
