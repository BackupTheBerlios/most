/*
 * super.h
 *
 *  Created on: 26.04.2013
 *      Author: maik
 */

#ifndef SFS_SUPER_H_
#define SFS_SUPER_H_

struct SFS_super_block
{
	ACE_u32_t size;                        /* size of partition */
	ACE_u32_t used;                        /* used data size */
	long super_nr;                /* super block number */
	long free_nr;                 /* free  block number */
};

struct SFS_super
{
	MFS_block_t *dev;                 /* block dev */
	struct SFS_super_block block;     /* super block on disc */
};

typedef struct SFS_super SFS_super_t;



/* format super block*/
extern void SFS_super_format(SFS_super_t *super, MFS_block_t *dev);

/* initialize super with stored information on disk */
extern void SFS_super_mount(SFS_super_t *super, MFS_block_t *dev);

/* synchronize super with disk */
extern void SFS_super_sync(SFS_super_t *super);


#endif /* SFS_SUPER_H_ */
