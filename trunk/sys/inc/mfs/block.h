/*
 *  Copyright (C) 2007  Michael Feiner
 *
 */

#ifndef MFS_BLOCK_H
#define MFS_BLOCK_H

#include <mfs/vfs.h>
#include <mfs/descriptor.h>

/** @addtogroup mfs
 * @{
 */

/** @defgroup block block.h
 *
 * A block device ? more doc needed.
 * @{
 */

/** Block representation type.
 */
typedef void MFS_block_represent_t;

/** Block descriptor operations instance. */
extern struct MFS_descriptor_op MFS_block_descriptor_op;

/** Block types. */
enum MFS_block_type
{
    MFS_DISK,           /**< DISK. */
    MFS_BLOCK_IO        /**< BLOCK IO. */
};


/* Block structure. */
struct MFS_block
{
	MFS_descriptor_t desc;
    enum MFS_block_type type;  /* Block type. */
    unsigned long start;         /* block number of first block (for DISK). */
    unsigned long end;           /* logical block number after last block (for DISK). */
    ACE_size_t size;          /* size of one block. */
    struct MFS_block_op *operations;   /* Block operations. */
};


/**
 * Get block.
 *
 * @param block : .
 * @param buf : .
 * @param number : .
 * @return block length or error.
 */
extern ACE_ssize_t MFS_get (MFS_block_t *block, char **buf, ACE_size_t number);

/**
 * Put block.
 *
 * @param block :.
 * @param buf : Buffer of data to write.
 * @param len : Length of the data to write.
 * @param number : block number.
 * @return errorcode.
 */
extern ACE_err_t MFS_put (MFS_block_t *block, char *buf, ACE_size_t len, ACE_size_t number);

extern ACE_err_t MFS_confirm (MFS_block_t * block, ACE_size_t number);

/**
 * Initialize a stream.
 *
 * @param block :
 * @param type :
 * @param operations :
 * @param represent :
 */
extern void MFS_block_init (
		MFS_block_t * block,
		enum MFS_block_type type,
		struct MFS_block_op *operations);

extern MFS_descriptor_t *MFS_block_create (MFS_descriptor_t * dir_desc, char *name, struct MFS_descriptor_op *desc_op,
					  struct MFS_block_op *io_op, MFS_represent_t * represent, enum MFS_block_type type);

extern void MFS_block_print (MFS_block_t * block);

/** @}
 */

/** @}
 */

#endif
