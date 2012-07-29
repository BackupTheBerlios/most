/*
 *  Copyright (C) 2007  Michael Feiner
 *
 */

#ifndef MFS_DIRECTORY_H
#define MFS_DIRECTORY_H

#include "uso/list.h"
#include "mfs/vfs.h"
#include "mfs/descriptor.h"
#include "mfs/stream.h"

/** @addtogroup mfs
 * @{
 */

/** @defgroup directory directory.h
 *
 * A directory is a special descriptor which contains a list of descriptors.
 * Descriptors can be created(added) , removed or renamed. It also provides the facility
 * to iterate over the directory list and to search for descriptors in the list.
 * @{
 */

/** Directory operation s. */
extern struct MFS_descriptor_op MFS_dir_descriptor_op;

/* Private: Directory structure. */
struct MFS_directory
{
    struct MFS_vfs_op *vfs_op;  /* Virtual file system operations. */
    USO_list_t descriptors;     /* List of descriptors. */
    struct MFS_directory_op *operations;        /* Directory operation interface. */
    void (*info) (void);        /* Info function. */
};

/**
 * Get next entry from directory list.
 * @param dir_desc : Descriptor where the entry is a directory.
 * @param iterator : Current descriptor or NULL to initialize iterator to list head.
 * @return Next descriptor or NULL if there is no descriptor.
 */
extern MFS_descriptor_t *MFS_next_entry (MFS_descriptor_t * dir_desc, MFS_descriptor_t * iterator);

/**
 * Find a descriptor for a given name in the directory.
 * @param dir_desc : Descriptor where the entry is a directory.
 * @param name : Search for a descriptor of the given name.
 * @return Found descriptor or NULL if no descriptor is found.
 */
extern MFS_descriptor_t *MFS_lookup (MFS_descriptor_t * dir_desc, char *name);


/**
 * Create a directory.
 *
 * @param dir_desc : Directory descriptor in which a new directory descriptor is created.
 * @param name : Name of the new directory.
 * @return Created directory descriptor.
 */
extern MFS_descriptor_t *MFS_create_dir (MFS_descriptor_t * dir_desc, char *name);

/**
 * Create a file.
 * A file is a special kind of stream io.
 * The stream operations for a file are used.
 * @param dir_desc : Directory descriptor in which a new file descriptor is created.
 * @param name : Name of the new file.
 * @return Created file descriptor.
 */
extern MFS_descriptor_t *MFS_create_file (MFS_descriptor_t * dir_desc, char *name);

/**
 * Create an IO stream.
 * The IO stream has to implement the stream operation interface.
 * @param dir_desc : Directory descriptor in which a new io descriptor is created.
 * @param name : Name for the io.
 * @param io_op : IO operations for the stream.
 * @param represent : Stream representation.
 * @return Created io descriptor.
 */
extern MFS_descriptor_t *MFS_create_io (MFS_descriptor_t * dir_desc, char *name,
                                        struct MFS_stream_op *io_op,
                                        MFS_stream_represent_t * represent);

/**
 * Create an descriptor.
 * @param dir_desc : Directory descriptor in which a new io descriptor is created.
 * @param name : Name for the descriptor.
 * @param entry : Implementation of the descriptor.
 * @param type : Kind of the implementation.
 * @param desc_op : Descriptor operations implementation.
 * @return Created descriptor.
 */
extern MFS_descriptor_t *MFS_create_desc (MFS_descriptor_t * dir_desc, char *name,
                                          MFS_entry_t * entry, enum MFS_entry_type type,
                                          struct MFS_descriptor_op *desc_op);


/**
 * Remove descriptor from directory list.
 * @param dir_desc : Directory from which the descriptor is removed.
 * @param desc : Descriptor to be removed.
 */
extern void MFS_remove_desc (MFS_descriptor_t * dir_desc, MFS_descriptor_t * desc);

/**
 * Search for name and if descriptor is found remove it from directory list.
 * @param dir_desc : Directory from which the descriptor is removed.
 * @param name : Name of the descriptor which should be removed.
 */
extern void MFS_remove (MFS_descriptor_t * dir_desc, char *name);


/**
 * Rename descriptor.
 * @param dir_desc : Directory descriptor in which the descriptor to be renamed is placed.
 * @param desc : Descriptor which should be renamed.
 * @param name : New name for the descriptor.
 */
extern void MFS_rename_desc (MFS_descriptor_t * dir_desc, MFS_descriptor_t * desc, char *name);

/**
 * Search for name and rename descriptor if found.
 * @param dir_desc : Directory descriptor in which the descriptor to be renamed is placed.
 * @param old_name : Descriptor name for which is searched for.
 * @param new_name : New name for the descriptor.
 */
extern MFS_descriptor_t *MFS_rename (MFS_descriptor_t * dir_desc, char *old_name, char *new_name);


/**
 * Initialize directory.
 * @param dir : Directory.
 * @param vfs_op : Pointer to virtual file system operations.
 */
extern void MFS_directory_init (MFS_directory_t * dir, struct MFS_vfs_op *vfs_op);

/** @}
 */

/** @}
 */

#endif
