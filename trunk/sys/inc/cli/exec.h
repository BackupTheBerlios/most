/*
 * exec.h
 *
 *  Created on: 28.10.2012
 *      Author: maik
 */

#ifndef EXEC_H_
#define EXEC_H_

#include <cli/err.h>
#include <mfs/vfs.h>
#include <net/ip_addr.h>

/** @addtogroup cli
 * @{
 */

/** @defgroup exec exec.h
 *
 * Register executables.
 * Executeables are registered  C function which you can
 * execute with the commands exec and run.
 * @{
 */



struct CLI_exec
{
    char *description;
    ACE_err_t (*f) (char *);
};

/** Executeable type. */
typedef struct CLI_exec CLI_exec_t;

/**
 *  Initialize and install an C function as executeable.
 * 	This function will be executed with the commands exec or run.
 *  An argument may be passed to the function.
 *
 *  @param dir : Directory descriptor to which the executeable will be installed.
 *  @param exec : Pointer to a CLI_exec_t.
 *  @param name : Pointer to name of executeable.
 *  @param description :  Pointer to a short description of the executeable.
 *  @param f : Function which will be executed with the commands exec or run.
 */
extern void CLI_exec_init (MFS_descriptor_t * dir, CLI_exec_t * exec,
                           char *name, char *description, ACE_err_t (*f) (char *));

extern void CLI_executes_init (void);
extern void CLI_executes_install (void);

extern MFS_descriptor_t *CLI_find_exe (char* name);

extern ACE_err_t CLI_exe_copy (char *arg);
extern ACE_err_t CLI_exe_axfr_r (char *arg);
extern ACE_err_t CLI_exe_axfr_s (char *arg);
extern ACE_err_t CLI_exe_heap_debug (char *arg);

extern ACE_err_t CLI_bootp (void);
extern void CLI_bootp_install (void);

extern ACE_err_t CLI_tftp_get (char *arg);
extern void CLI_tftp_install (NET_ip_addr_t * client_address, NET_ip_addr_t * server_address);

extern ACE_err_t CLI_ymodem_r (char *arg);
extern void CLI_ymodem_install (void);

/** @}
 */

/** @}
 */


#endif /* EXEC_H_ */
