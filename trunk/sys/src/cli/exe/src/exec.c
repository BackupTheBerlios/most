/*
 * exec.c
 *
 *  Created on: 20.11.2013
 *      Author: maik
 */

#include <cli/exec.h>
#include <mfs/vfs.h>
#include <mfs/directory.h>
#include <mfs/sysfs.h>

static CLI_exec_t copy;
static CLI_exec_t axfr_r;
static CLI_exec_t axfr_s;
static CLI_exec_t heap_debug;

extern void
CLI_executes_install (void)
{
    MFS_descriptor_t *exe_dir = MFS_resolve ("/sys/cli/exe");
    CLI_exec_init(exe_dir, &copy, "cp", "copy stdin 2 stdout", CLI_exe_copy);
    CLI_exec_init(exe_dir, &axfr_r, "axfr_r", "ascii transfer receive", CLI_exe_axfr_r);
    CLI_exec_init(exe_dir, &axfr_s, "axfr_s", "ascii transfer send", CLI_exe_axfr_s);
    CLI_exec_init(exe_dir, &heap_debug, "heap_D", "Debug heap list", CLI_exe_heap_debug);
}


