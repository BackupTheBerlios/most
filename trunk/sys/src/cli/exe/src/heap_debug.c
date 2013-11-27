/*
 * heap_debug.c
 *
 *  Created on: 19.11.2013
 *      Author: maik
 */

#include <ace/stdio.h>
#include <cli/exec.h>
#include <cli/arg.h>
#include <uso/heap.h>
#include <uso/log.h>
#include <mfs/descriptor.h>

extern ACE_err_t
CLI_exe_heap_debug (char *arg)
{
    ACE_err_t err = ACE_OK;
    int argc;
    char *argv[CLI_MAX_ARG];

    argc = CLI_arg_parse(arg, argv);
    if (argc >= 1){
        MFS_descriptor_t *desc = MFS_walk_in (MFS_resolve("/sys/uso/heap"), argv[0], TRUE);
        if (desc != NULL) {
            USO_debug_heap_list (desc->represent);
        } else {
            err = DEF_ERR_NOT_FOUND;
        }
        MFS_close_desc(desc);
    } else {
        err = DEF_ERR_ARG;
    }
    return err;
}


