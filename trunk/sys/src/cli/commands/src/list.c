/*
 * list.c
 *
 *  Created on: 28.10.2012
 *      Author: maik
 */

#include <ace/stdio.h>
#include <ace/esc.h>
#include <cli/command.h>
#include <cli/arg.h>
#include <mfs/descriptor.h>
#include <mfs/directory.h>
#include <mfs/sysfs.h>

static void
print_info_name(MFS_descriptor_t * desc, int y, int x)
{
    if (desc != NULL)
    {
        MFS_info_entry_t entry;
        for (int i = 0; i < MFS_INFO_MAX_ENTRIES; ++i){
            MFS_info_desc (desc, i, &entry);
            if (entry.type != MFS_INFO_NOT_AVAIL){
                ACE_printf("%s", entry.name);
                x += 10;
                ACE_esc_set_cursor(y, x);
            } else {
                break;
            }
        }
    }
}

static void
print_info_value(MFS_descriptor_t * desc, int y, int x)
{
    if (desc != NULL)
    {
        MFS_info_entry_t entry;
        for (int i = 0; i < MFS_INFO_MAX_ENTRIES; ++i){
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
                        ACE_printf("%p", (void*)entry.value.p);
                        break;
                    case MFS_INFO_STRING:
                        ACE_printf("%s", (char*)entry.value.s);
                        break;
                    default:
                        ACE_puts("t?");
                        break;
                }
                x += 10;
                ACE_esc_set_cursor(y, x);
            } else {
                break;
            }
        }
    }
}


static void
descriptor_print (MFS_descriptor_t * desc, ACE_bool_t info, ACE_bool_t head)
{
    char *type;
    int c;
    int y, x;
    ACE_bool_t b;
    c = ACE_esc_request_cursor(&y, &x, &b);
    if (c != ACE_EOF || b != TRUE){
        ACE_printf ("VT100 response err: %i\n", c);
        return;
    }
    switch (desc->type)
    {
    case MFS_DIRECTORY:
        type = "dir";
        break;
    case MFS_STREAM:
        type = "stream";
        break;
    case MFS_BLOCK:
        type = "block";
        break;
    case MFS_EXEC:
        type = "exec";
        break;
    case MFS_SYS:
        type = "sys";
        break;
    case MFS_USR:
        type = "usr";
        break;
    default:
        type = "?";
        break;
    }
    if (head){
        x = 0; // we always start at 0;
        ACE_puts (ACE_ESC_BRIGHT"Type");
        x += 8;
        ACE_esc_set_cursor(y, x);
        ACE_puts ("Name");
        x += 8;
        ACE_esc_set_cursor(y, x);
        ACE_puts ("Open");
        x += 5;
        ACE_esc_set_cursor(y, x);
        if (info){
            print_info_name(desc, y, x);
        }
        ACE_puts(ACE_ESC_NORMAL"\n");
        c = ACE_esc_request_cursor(&y, &x, &b);
        if (c != ACE_EOF || b != TRUE){
            ACE_printf ("VT100 response err: %i\n", c);
            return;
        }
    }
    x = 0;
    ACE_printf ("%s", type);
    x += 8;
    ACE_esc_set_cursor(y, x);
    ACE_printf ("%s", desc->name);
    x += 8;
    ACE_esc_set_cursor(y, x);
    ACE_printf ("%i", desc->open_cnt);
    x += 5;
    ACE_esc_set_cursor(y, x);
    if (info){
        print_info_value(desc, y, x);
    }
    ACE_puts("\n");
}

extern ACE_err_t
CLI_cmd_list (CLI_interpreter_t * cli)
{
    ACE_err_t err = ACE_OK;
    ACE_bool_t info = FALSE;
    ACE_bool_t close = FALSE;
    MFS_descriptor_t *desc = NULL;
    int argc;
    char *argv[CLI_MAX_ARG];

    argc = CLI_arg_parse(cli->p.arg, argv);
    if (argc >= 1)
    {
        switch (argv[0][0])
        {
        case 'i':
            info = TRUE;
            desc = USO_thread_work_get(USO_current());
            break;
        case 't':
            info = TRUE;
            close = TRUE;
            desc = MFS_resolve("/sys/uso/thread");
            break;
        case 'm':
            info = TRUE;
            close = TRUE;
            desc = MFS_resolve("/sys/uso/heap");
            break;
        case 'c':
            info = TRUE;
            close = TRUE;
            desc = MFS_resolve("/sys/cli/cmd");
            break;
        case 'e':
            info = TRUE;
            close = TRUE;
            desc = MFS_resolve("/sys/cli/exe");
            break;
        case 'a':
            info = TRUE;
            close = TRUE;
            desc = MFS_resolve("/sys/dev/timer/timer");
            break;
        default:
            break;
        }
    } else {
        desc = USO_thread_work_get(USO_current());
    }
    MFS_descriptor_t *iterator = NULL;
    if (desc){
        if ((iterator = MFS_next_entry (desc, iterator)) != NULL) {
            descriptor_print (iterator, info, TRUE);
            while ((iterator = MFS_next_entry (desc, iterator)) != NULL)
            {
                descriptor_print (iterator, info, FALSE);
            }
        }
        if (close) MFS_close_desc(desc);
    } else {
        err = DEF_ERR_ARG;
    }
    return err;
}

