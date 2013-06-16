/*
 * inode.c
 *
 *  Created on: 08.04.2013
 *      Author: maik
 */


#include <ace/stddef.h>
#include <ace/err.h>
#include "inode.h"
#include "free.h"



static void
init_inode(SFS_inode_t *inode)
{
}

extern long
SFS_inode_get(SFS_inode_t *inode)
{
	return -1;
}

extern ACE_err_t
SFS_inode_put(SFS_inode_t *inode, long data)
{
	return ACE_OK;
}

extern void
SFS_inode_format(SFS_inode_t *inode, MFS_block_t *dev, unsigned long nr)
{
	long i;
	inode->dev = dev;
	inode->nr = nr;
	for (i = DATA_TAB_START; i < DATA_TAB_END; ++i){
		inode->block.data_tab[i] = 0;
	}
	inode->block.size = 0;
	init_inode(inode);
	SFS_inode_sync(inode);
}

extern void
SFS_inode_mount(SFS_inode_t *inode, MFS_block_t *dev, unsigned long nr)
{
	ACE_ssize_t len;
	char *data;
	inode->dev = dev;
	inode->nr = nr;
	len = MFS_get (inode->dev, &data, inode->nr);
	if (len == SFS_BLOCK_SIZE){
		memcpy(inode->block, data, len);
		init_inode(inode);
	}
	MFS_confirm(inode->dev, inode->nr);
}

extern void
SFS_inode_sync(SFS_inode_t *inode)
{
    MFS_put  (inode->dev, inode->block, SFS_BLOCK_SIZE, inode->nr);
}
