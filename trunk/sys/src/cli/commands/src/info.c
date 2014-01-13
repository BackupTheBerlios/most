/*
 * info.c
 *
 *  Created on: 28.10.2012
 *      Author: maik
 */

#include <ace/stdio.h>
#include <ace/stdlib.h>
#include <cli/command.h>
#include <cli/arg.h>
#include <mfs/descriptor.h>
#include <mfs/directory.h>
#include <mfs/sysfs.h>

static void
print_info_value (MFS_descriptor_t * desc, int i)
{
    if (desc != NULL)
    {
        MFS_info_entry_t entry;
        MFS_info_desc (desc, i, &entry);
        if (entry.type != MFS_INFO_NOT_AVAIL){
            switch (entry.type){
                case MFS_INFO_LONG:
                    ACE_printf("%li", entry.value.l);
                    break;
                case MFS_INFO_SIZE:
                    ACE_printf("%lu", entry.value.z);
                    break;
                case MFS_INFO_PTR:
                    ACE_printf("%p", entry.value.p);
                    break;
                case MFS_INFO_STRING:
                    ACE_printf("%s", entry.value.s);
                    break;
                default:
                    ACE_putc('?');
                    break;
            }
        }
    }
}

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
                        ACE_printf("\t %i %s = %li;\n", i, entry.name, entry.value.l);
                        break;
                    case MFS_INFO_SIZE:
                        ACE_printf("\t %i %s = %lu;\n", i, entry.name, entry.value.z);
                        break;
                    case MFS_INFO_PTR:
                        ACE_printf("\t %i %s = %p;\n", i, entry.name, entry.value.p);
                        break;
                    case MFS_INFO_STRING:
                        ACE_printf("\t %i %s = %s;\n", i, entry.name, entry.value.s);
                        break;
                    default:
                        ACE_printf("\t %i %s = ?;\n", i, entry.name);
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
    desc = USO_thread_work_get(USO_current());
    if (argc >= 1)
    {
        print_info_value(desc, ACE_atoi(argv[0]));
    }
    else
    {
        print_info (desc);
    }
    return err;
}

