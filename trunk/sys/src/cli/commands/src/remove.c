/*
 * goto.c
 *
 *  Created on: 31.10.2012
 *      Author: maik
 */

#include <ace/stdio.h>
#include <cli/command.h>
#include <mfs/descriptor.h>
#include <mfs/directory.h>
#include <mfs/sysfs.h>

extern ACE_bool_t
CLI_cmd_remove (CLI_interpreter_t * cli)
{
    MFS_descriptor_t *desc = USO_thread_dir_get(USO_current());
    MFS_remove (desc, cli->argv [0]);
    return TRUE;
}


