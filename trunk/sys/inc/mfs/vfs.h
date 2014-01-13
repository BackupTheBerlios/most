/*
 *  Copyright (C) 2007  Michael Feiner
 *
 */

#ifndef MFS_VFS_H
#define MFS_VFS_H

#include <ace/stddef.h>
#include <ace/err.h>

/** @addtogroup mfs
 * @{
 */

/** @defgroup vfs vfs.h
 *
 * Virtual file system.
 * @{
 */

#define MFS_PATH_SIZE 64
#define MFS_INFO_MAX_ENTRIES 32

enum MFS_info_type
{
    MFS_INFO_NOT_AVAIL,           /**< entry is not available, name and value are not valid */
    MFS_INFO_LONG,                /**< signed 32 bit integer value */
    MFS_INFO_SIZE,                /**< ACE_size_t (unsigned long) */
    MFS_INFO_PTR,                 /**< pointer */
    MFS_INFO_STRING               /**< char *,-> c string (terminated with 0) */
};

union MFS_info_value
{
    long l;
    ACE_size_t z;
    void *p;
    char *s;
};

struct MFS_info_entry
{
    enum MFS_info_type type;
    char *name;
    union MFS_info_value value;
};

typedef struct MFS_info_entry MFS_info_entry_t;

enum MFS_ctrl_type
{
    MFS_CTRL_HELP,            /**< get ctrl entries info in value.s */
    MFS_CTRL_LONG,            /**< signed 32 bit integer value */
    MFS_CTRL_SIZE,            /**< ACE_size_t (unsigned long) */
    MFS_CTRL_HEX,             /**< unsigned long in hex */
    MFS_CTRL_STRING           /**< char *,-> c string (terminated with 0) */
};

union MFS_ctrl_value
{
    long l;
    ACE_size_t z;
    unsigned long u;
    char *s;
};

struct MFS_ctrl_entry
{
    enum MFS_ctrl_type type;
    union MFS_ctrl_value value;
};

typedef struct MFS_ctrl_entry MFS_ctrl_entry_t;


/** Descriptor type. */
typedef struct MFS_descriptor MFS_descriptor_t;

/** Descriptor interface. */
struct MFS_descriptor_op
{
    ACE_err_t (*open) (MFS_descriptor_t * desc);                                       /**< Open. */
    ACE_err_t (*close) (MFS_descriptor_t * desc);                                      /**< Close. */
    void (*info) (MFS_descriptor_t * desc, int number, MFS_info_entry_t *entry);       /**< Info. */
    void (*control) (MFS_descriptor_t * desc, int number, MFS_ctrl_entry_t *entry);    /**< Control */
    ACE_err_t (*delete) (MFS_descriptor_t *desc);                                      /**< Must implement delete if descripter shall be removeable */
};

/** Super type. */
typedef struct MFS_super MFS_super_t;

/** Super interface. */
struct MFS_super_op
{
    void (*mount) (MFS_super_t * super);       /**< Mount. */
    void (*umount) (MFS_super_t * super);      /**< Unmount. */
};

/** Directory type. */
typedef struct MFS_directory MFS_directory_t;

/** Directory interface. */
struct MFS_directory_op
{
    void (*create) (MFS_descriptor_t * desc);        /**< Create. */
    void (*remove) (MFS_descriptor_t * desc);        /**< Remove. */
    void (*rename) (MFS_descriptor_t * desc);        /**< Rename. */
};

/** Stream type. */
typedef struct MFS_stream MFS_stream_t;

/** Stream interface. */
struct MFS_stream_op
{
    ACE_size_t (*read) (MFS_stream_t * stream, char *buf, ACE_size_t len);            /**< Read. */
    ACE_size_t (*write) (MFS_stream_t * stream, const char *buf, ACE_size_t len);     /**< Write. */
    int (*seek) (MFS_stream_t * stream, ACE_ssize_t off, ACE_size_t pos);               /**< Seek. */
    void (*flush) (MFS_stream_t * stream);                                              /**< Flush. */
};

/** Block type. */
typedef struct MFS_block MFS_block_t;

/** Block interface. */
struct MFS_block_op
{
    ACE_ssize_t (*get) (MFS_block_t *block, char **buf, ACE_size_t number);
    ACE_err_t (*put) (MFS_block_t *block, char *buf, ACE_size_t len, ACE_size_t number);
    ACE_err_t (*confirm) (MFS_block_t * block, ACE_size_t number);
};

/** Virtual file system interface. */
struct MFS_vfs_op
{
    struct MFS_descriptor_op *super_desc_op;     /**< Super descriptor interface. */
    struct MFS_super_op *super_op;               /**< Super interface. */
    struct MFS_descriptor_op *dir_desc_op;       /**< Directory descriptor interface. */
    struct MFS_directory_op *dir_op;             /**< Directory interface. */
    struct MFS_descriptor_op *stream_desc_op;    /**< Stream descriptor interface. */
    struct MFS_stream_op *stream_op;             /**< Stream interface. */
};

/** @}
 */

/** @}
 */

#endif
