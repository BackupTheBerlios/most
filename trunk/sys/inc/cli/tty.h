/*
 *  Copyright (C) 2012  Michael Feiner
 *
 */

#ifndef CLI_TTY_H
#define CLI_TTY_H

#include <uso/semaphore.h>
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

enum CLI_tty_control_key
{
    CLI_TTY_CTRL_IN_MODE = 1,        /**< Set tty receive mode. */
    CLI_TTY_CTRL_OUT_MODE,           /**< Set tty transmit mode. */
    CLI_TTY_CTRL_IN_TRANSL,          /**< Set tty receive translation. */
    CLI_TTY_CTRL_OUT_TRANSL,         /**< Set tty transmit translation. */
    CLI_TTY_CTRL_DEFAULT_TRANSL,     /**< Set tty translation to default. */
    CLI_TTY_CTRL_RX_TIMEOUT          /**< Set serial receive timeout in sec. */
};

enum CLI_tty_in_transl
{
    CLI_TTY_INTRANSL_NONE,
    CLI_TTY_INTRANSL_CR_2_NL,
    CLI_TTY_INTRANSL_REMOVE_CR,
};

enum CLI_tty_out_transl
{
    CLI_TTY_OUTTRANSL_NONE,
    CLI_TTY_OUTTRANSL_NL_2_CR,
    CLI_TTY_OUTTRANSL_ADD_CR
};

/** TTY modes. */
enum CLI_tty_mode
{

    /** No conversion */
    CLI_TTY_MODE_RAW,

    /** Data are processed */
    CLI_TTY_MODE_COOKED,

};

/*
 * TTY Struct
 * 
 * Private.
 */
struct CLI_tty
{
    MFS_descriptor_t *io_stream;
    enum CLI_tty_mode in_mode;
    enum CLI_tty_mode out_mode;
    enum CLI_tty_in_transl in_transl;
    enum CLI_tty_out_transl out_transl;
    enum CLI_tty_in_transl in_transl_default;
    enum CLI_tty_out_transl out_transl_default;
    USO_semaphore_t r_lock;
    USO_semaphore_t w_lock;
    USO_semaphore_t select;
    ACE_bool_t start;
    ACE_bool_t log;
    int t;
};

/**
 * TTY type.
 *
 */
typedef struct CLI_tty CLI_tty_t;

/*------------------------------------------------------------------------*/

/*-------------- Interface -----------------------------------------------*/

/* this function is called from the tty switch
 *
 */
extern void CLI_tty_select(MFS_descriptor_t *stream);


/**
 * Initialize a serial device.
 *
 * @param tty : Pointer to tty
 * @param io_stream :
 * @param tx_mode : newline conversion for transmit.
 * @param rx_mode : newline conversion for receive.
 * @param name : name for tty.
 */

extern void CLI_tty_init (CLI_tty_t * tty,
                          MFS_descriptor_t *io_stream,
                          enum CLI_tty_in_transl in_transl,
                          enum CLI_tty_out_transl out_transl,
                          char *name,
                          ACE_bool_t log);

/*------------------------------------------------------------------------*/


/** @}
 */

/** @}
 */

#endif
