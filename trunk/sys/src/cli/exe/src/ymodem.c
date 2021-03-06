/*
 * ymodem.c
 *
 *  Created on: 24.12.2012
 *      Author: maik
 */

#include <ace/stdio.h>
#include <ace/stdlib.h>
#include <uso/log.h>
#include <uso/scheduler.h>
#include <uso/thread.h>
#include <nap/ymodem.h>
#include <dev/serial.h>
#include <cli/tty.h>
#include <cli/exec.h>
#include <cli/arg.h>
#include <mfs/stream.h>
#include <mfs/directory.h>
#include <mfs/sysfs.h>
#include <mfs/block.h>

static CLI_exec_t ymr;

static ACE_size_t
write_packet(char *data, int length, MFS_descriptor_t *desc)
{
    return ACE_fwrite (desc, data, length);
}

extern ACE_err_t
CLI_ymodem_r (char *arg)
{
    ACE_err_t err = ACE_OK;;
    int argc;
    char *argv[CLI_MAX_ARG];

    argc = CLI_arg_parse(arg, argv);
    if (argc < 1){
        return DEF_ERR_ARG;
    }
    
    MFS_descriptor_t *desc = MFS_open(USO_thread_work_get(USO_current()), argv[0]);
    if (desc == NULL) {
        return DEF_ERR_NOT_FOUND;
    }

    ACE_size_t size = 0;
    char *name = ACE_malloc(NAP_YMODEM_FILE_NAME_LENGTH);
    if (name != NULL){
        name[0] = '\0';
    }

    if (desc->type == MFS_STREAM) {
        size = NAP_ymodem_receive(name, write_packet, desc);
    }

    ACE_printf("%s %lu bytes rx.\n", name ? name : "no_mem", size);

    if (name != NULL){
        ACE_free(name);
    }

    MFS_close_desc(desc);
    return err;
}

extern void
CLI_ymodem_install (void)
{
    MFS_descriptor_t *dir = MFS_resolve("/sys/cli/exe");
    CLI_exec_init (dir, &ymr, "ym_r", "Ymodem receive", CLI_ymodem_r);
    MFS_close_desc(dir);
}
