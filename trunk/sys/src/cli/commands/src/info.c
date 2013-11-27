/*
 * info.c
 *
 *  Created on: 28.10.2012
 *      Author: maik
 */

#include <ace/stdio.h>
#include <cli/command.h>
#include <cli/arg.h>
#include <mfs/descriptor.h>
#include <mfs/directory.h>
#include <mfs/sysfs.h>

static void
print_info (MFS_descriptor_t * desc)
{
    if (desc != NULL)
    {
        MFS_info_entry_t entry;
        ACE_printf("%s {\n", desc->name);
        for (int i = 0; i < MFS_INFO_MAX_ENTRIES; ++i){
            MFS_info_desc (desc, i, &entry);
            if (entry.type != MFS_INFO_NOT_AVAIL){
                switch (entry.type){
                    case MFS_INFO_LONG:
                        ACE_printf("\t%s = %li;\n", entry.name, entry.value.l);
                        break;
                    case MFS_INFO_SIZE:
                        ACE_printf("\t%s = %lu;\n", entry.name, entry.value.z);
                        break;
                    case MFS_INFO_PTR:
                        ACE_printf("\t%s = %p;\n", entry.name, (void*)entry.value.p);
                        break;
                    case MFS_INFO_STRING:
                        ACE_printf("\t%s = %s;\n", entry.name, (char*)entry.value.s);
                        break;
                    default:
                        ACE_printf("\t%s = ?;\n", entry.name);
                        break;
                }
            } else {
                break;
            }
        }
        ACE_puts("}\n");
    }
}



extern ACE_err_t
CLI_cmd_info (CLI_interpreter_t * cli)
{
    ACE_err_t err = ACE_OK;
    MFS_descriptor_t *desc;
    int argc;
    char *argv[CLI_MAX_ARG];

    argc = CLI_arg_parse(cli->p.arg, argv);
    if (argc >= 1)
    {
        desc = MFS_resolve (argv[0]);
        if (desc != NULL)
        {
            print_info (desc);
            MFS_close_desc (desc);
        }
        else
        {
            err = DEF_ERR_NOT_FOUND;
        }
    }
    else
    {
        print_info (USO_thread_work_get(USO_current()));
    }
    return err;
}

