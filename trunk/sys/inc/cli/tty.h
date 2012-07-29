/*
 *  Copyright (C) 2012  Michael Feiner
 *
 */

#ifndef CLI_TTY_H
#define CLI_TTY_H

#include <ace/stddef.h>
#include <mfs/vfs.h>
#include <mfs/stream.h>

/** @addtogroup cli
 * @{
 */

/** @defgroup tty tty.h
 *
 * Teletype adapter.
 * @{
 */

/*------------- Representation ------------------------------------------*/

/** TTY modes. */
enum CLI_tty_mode
{

    /** No newline conversion */
    CLI_TTY_MODE_NL2NL,

    /** Newline 2 Carriage return */
    CLI_TTY_MODE_NL2CR,

    /** Newline 2 Carriage return & Newline*/
    CLI_TTY_MODE_NL2CRNL,
};

/*
 * TTY Struct
 * 
 * Private.
 */
struct CLI_tty
{
    MFS_descriptor_t *io_stream_desc;
    MFS_stream_t *io_stream;
    enum MFS_sysfs_dir io_stream_dir;
    char *io_stream_name;
    enum CLI_tty_mode tx_mode;
    enum CLI_tty_mode rx_mode;
};

/**
 * TTY type.
 *
 */
typedef struct CLI_tty CLI_tty_t;

/*------------------------------------------------------------------------*/

/*-------------- Interface -----------------------------------------------*/

/**
 * Initialize a serial device.
 *
 * @param tty : Pointer to tty
 * @param stream_dir : Directory serching for stream.
 * @param stream_name : Pointer to name of the stream.
 * @param tx_mode : newline conversion for transmit.
 * @param rx_mode : newline conversion for receive.
 * @param name : name for tty.
 */

extern void CLI_tty_init (CLI_tty_t * tty,
                          enum MFS_sysfs_dir stream_dir,
                          char *stream_name,
                          enum CLI_tty_mode tx_mode, enum CLI_tty_mode rx_mode, char *name);


/*------------------------------------------------------------------------*/


/** @}
 */

/** @}
 */

#endif
