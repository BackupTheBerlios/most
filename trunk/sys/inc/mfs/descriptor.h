/*
 *  Copyright (C) 2007  Michael Feiner
 *
 */

#ifndef MFS_DESCRIPTOR_H
#define MFS_DESCRIPTOR_H

#include <uso/list.h>
#include <mfs/vfs.h>
#include <mfs/err.h>

/** @addtogroup mfs
 * @{
 */

/** @defgroup descriptor descriptor.h
 *
 * Descriptor functions.
 * A descriptor is the common part of each entry in the filesystem.
 * The descriptor operation are open, close and info.
 * Each entry has to provide this operations.
 * For a directory, open means change to that directory and close means go to parent directory.
 * For a stream(file, serial io, ..) open means that the stream will be ready for reading and writing and
 * close means that the stream will be detached.
 * If an entry provides an info operation, the information is printed with the info command.
 * @{
 */

#define MFS_NAME_SIZE 8  /**< File name size including the terminating /0. */


/** Descriptor entry type. */
enum MFS_desc_type
{
    MFS_DIRECTORY,           /**< Directory. */
    MFS_STREAM,              /**< IO stream, files, serial io, ... .*/
    MFS_BLOCK,               /**< Block device */
    MFS_EXEC,                /**< Executable (C function registered as exec). */
    MFS_INFO,                /**< Provide just the descriptor interface, open, close and info. */
    MFS_SYS,                 /**< System descriptor. */
    MFS_USR                  /**< User descriptor. */
};

typedef void MFS_represent_t;

/** File system descriptor structure. */
struct MFS_descriptor
{
    USO_node_t node;                                 /**< List node, to put the descriptor to a list. */
    MFS_represent_t *represent;                       /**< Descriptor representation. */
    struct MFS_descriptor_op *operations;            /**< Descriptor function interface. */
    char name[MFS_NAME_SIZE];                        /**< Name of the descriptor. */
    enum MFS_desc_type type;                        /**< Kind of descriptor(entry). */
    MFS_descriptor_t *parent;                        /**< Parent descriptor. */
    int open_cnt;                                     /**< Open counter. */
};

/**
 * Call the descriptors open operation.
 * @param desc : Descriptor.
 */
extern ACE_err_t MFS_open_desc (MFS_descriptor_t * desc);

/**
 * Lookup for the name in the directory, if a descriptor is found call its open operation.
 * @param dir_desc : Directory.
 * @param name : Name of descriptor for lookup.
 * @return Found and opened descriptor or NULL.
 */
extern MFS_descriptor_t *MFS_open (MFS_descriptor_t * dir_desc, char *name);

extern MFS_descriptor_t *MFS_resolve (MFS_descriptor_t *desc, char *path);

/**
 * Call the descriptors close operation.
 * @param desc : Descriptor.
 * @return Parent descriptor.
 */
extern MFS_descriptor_t *MFS_close_desc (MFS_descriptor_t * desc);

/**
 * Call the descriptors info operation.
 * @param desc : Descriptor.
 */
extern void MFS_info_desc (MFS_descriptor_t * desc);

/**
 * Lookup for the name in the directory, if a descriptor is found call its info operation.
 * @param dir_desc : Directory.
 * @param name : Name of descriptor for lookup.
 */
extern void MFS_info (MFS_descriptor_t * dir_desc, char *name);

extern void MFS_control_desc (MFS_descriptor_t * dir_desc, enum MFS_control_key key, long value);

/**
 * Initialize a descriptor.
 * @param desc : Descriptor.
 * @param represent : Representation of the descriptor.
 * @param operations : The descriptor interface.
 * @param name : Name of the descriptor.
 * @param type : Kind of entry.
 * @param parent : Parent descriptor.
 */
extern void MFS_descriptor_init (MFS_descriptor_t * desc,
							     MFS_represent_t *represent,
                                 struct MFS_descriptor_op *operations,
                                 char *name, enum MFS_desc_type type, MFS_descriptor_t * parent);

/**
 * Initialize a descriptor.
 * @param dir_desc : Directory in which the descriptor will be created.
 * @param name : Name of the descriptor.
 * @param type : Kind of entry.
 * @param parent : Parent descriptor.
 * @param operations : The descriptor interface.
 * @param represent : Representation of the descriptor.
 * @return Created descriptor.
 */
extern MFS_descriptor_t * MFS_descriptor_create (
		MFS_descriptor_t * dir_desc,
		char *name,
		enum MFS_desc_type type,
		struct MFS_descriptor_op *desc_op,
		MFS_represent_t * represent);

/**
 * Print information about the descriptor.
 * @param desc : Descriptor.
 */
extern void MFS_descriptor_print (MFS_descriptor_t * desc);


/** @}
 */

/** @}
 */

#endif
