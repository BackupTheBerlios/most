/*
 * free.c
 *
 *  Created on: 25.03.2013
 *      Author: maik
 */


#include <ace/stddef.h>
#include <ace/err.h>
#include "free.h"


static long
find_bit (long bits)
{
	long i = 0;
	if (bits != 0){
		for (ACE_u32_t mask = 0x1; mask != 0; mask << 1, ++i)
		{
			if (bits & mask) return i;
		}
	}
	return -1;
}

static int
find_entry (long entry_tab[])
{
	long i, bit;
	for (i = ENTRY_TAB_START; i < ENTRY_TAB_END; ++i){
		bit = find_bit(entry_tab[i]);
		if (bit >= 0 && bit < ENTRY_SIZE){
			return bit + i * ENTRY_SIZE;
		}
	}
	return -1;
}

static void
init_entry_tab(SFS_free_t *free)
{
	long i;
	for (i = ENTRY_TAB_START; i < ENTRY_TAB_END; ++i){
		free->entry_tab[i] = ENTRY_INIT_VALUE;
	}
	for (i = FREE_TAB_START; i < FREE_TAB_END; ++i){
		if (free->free_tab[i] == 0){
			free->entry_tab[i / ENTRY_SIZE] &= ~(1 << (i % ENTRY_SIZE));
		}
	}
}

extern long
SFS_free_get(SFS_free_t *free)
{
	long i, bit;
	i = find_entry(free->entry_tab);
	if (i >= FREE_TAB_START && bit < FREE_TAB_END){
		bit = find_bit(free->free_tab[i]);
		if (bit >= 0 && bit < ENTRY_SIZE){
			free->free_tab[i] &= ~(1 << bit);
			if (free->free_tab[i] == 0){
				free->entry_tab[i / ENTRY_SIZE] &= ~(1 << (i % ENTRY_SIZE));
			}
			return bit + i * ENTRY_SIZE;
		}
	}
	return -1;
}

extern ACE_err_t
SFS_free_put(SFS_free_t *free, long used)
{
	long i = used / ENTRY_SIZE;
	long bit = used % ENTRY_SIZE;
	if (i >= FREE_TAB_START && bit < FREE_TAB_END){
		if (bit >= 0 && bit < ENTRY_SIZE){
			free->free_tab[i] &= ~(1 << bit);
			if (free->free_tab[i] == 0){
				free->entry_tab[i / ENTRY_SIZE] |= (1 << (i % ENTRY_SIZE));
				return ACE_OK;
			}
		}
	}
	return DEF_ERR_RANGE;
}

extern void
SFS_free_format(SFS_free_t *free, MFS_block_t *dev, unsigned long nr)
{
	long i;
	free->dev = dev;
	free->nr = nr;
	for (i = ENTRY_TAB_START; i < ENTRY_TAB_END; ++i){
		free->entry_tab[i] = ENTRY_INIT_VALUE;
	}
	for (i = FREE_TAB_START; i < FREE_TAB_END; ++i){
		free->free_tab[i] = ENTRY_INIT_VALUE;
	}
	SFS_free_sync(free);
}

extern void
SFS_free_mount(SFS_free_t *free, MFS_block_t *dev, unsigned long nr)
{
	ACE_ssize_t len;
	char *data;
	free->dev = dev;
	free->nr = nr;
	len = MFS_get (free->dev, &data, free->nr);
	if (len == SFS_BLOCK_SIZE){
		memcpy(free->free_tab, data, len);
		init_entry_tab(free);
	}
	MFS_confirm(free->dev, free->nr);
}

extern void
SFS_free_sync(SFS_free_t *free)
{
    MFS_put  (free->dev, free->free_tab, SFS_BLOCK_SIZE, free->nr);
}
