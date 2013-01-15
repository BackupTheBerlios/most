/*
 * exec.c
 *
 *  Created on: 28.10.2012
 *      Author: maik
 */

#include <ace/stdio.h>
#include <cli/exec.h>
#include <mfs/vfs.h>
#include <mfs/directory.h>
#include <mfs/sysfs.h>

static MFS_descriptor_t * exe_dir;

static void
info_head (MFS_descriptor_t * desc)
{
    ACE_printf ("Description\n");
}

static struct MFS_descriptor_op exec_head_descriptor_op = {
	.open = NULL,
    .close = NULL,
    .info = info_head,
	.control = NULL,
};

static void
exec_info_head (void)
{
    MFS_descriptor_create (exe_dir, "exec", MFS_INFO, &exec_head_descriptor_op, NULL);
}

static void
exec_info (MFS_descriptor_t * desc)
{
    CLI_exec_t *exec = (CLI_exec_t *) desc->represent;
    ACE_printf ("[ %s ].\n", exec->description);
}

static struct MFS_descriptor_op exec_descriptor_op = {.open = NULL,
    .close = NULL,
    .info = exec_info
};


extern void
CLI_exec_init (MFS_descriptor_t * dir, CLI_exec_t * exec,
               char *name, char *description, void (*f) (char *))
{
    exec->description = description;
    exec->f = f;
    MFS_descriptor_create (dir, name, MFS_EXEC, &exec_descriptor_op, (MFS_represent_t *) exec);
}

static CLI_exec_t copy;
static CLI_exec_t axfr_r;
static CLI_exec_t axfr_s;

extern void
CLI_executes_install (void)
{
	exe_dir = MFS_resolve (MFS_get_root(), "sys/cli/exe");
	exec_info_head ();
    CLI_exec_init(exe_dir, &copy, "cp", "copy stdin 2 stdout", CLI_exe_copy);
    CLI_exec_init(exe_dir, &axfr_r, "axfr_r", "ascii transfer receive", CLI_exe_axfr_r);
    CLI_exec_init(exe_dir, &axfr_s, "axfr_s", "ascii transfer send", CLI_exe_axfr_s);
}

extern MFS_descriptor_t *
CLI_find_exe (char* name)
{
    MFS_descriptor_t *desc;
    desc = MFS_open (exe_dir, name);
    if (NULL != desc && desc->type == MFS_EXEC)
        return desc;
    return NULL;
}
