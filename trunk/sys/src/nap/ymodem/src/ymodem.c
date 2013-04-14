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

static void
ymr_exec (char *arg)
{
	MFS_descriptor_t *desc = MFS_open(USO_thread_dir_get(USO_current()), arg);
	if (desc == NULL) {
		ACE_printf("%s not found.\n", arg);
		return;
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
}

extern void
NAP_ymodem_install (void)
{
    CLI_exec_init (MFS_resolve(MFS_get_root(), "sys/cli/exe"), &ymr, "ym_r", "Ymodem receive", ymr_exec);
}
