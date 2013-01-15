/*
 * ascii_transfer.c
 *
 *  Created on: 28.12.2012
 *      Author: maik
 */

#include <ace/stdio.h>
#include <cli/exec.h>
#include <cli/tty.h>
#include <uso/scheduler.h>
#include <uso/log.h>
#include <mfs/descriptor.h>

static void
axfr_rx(char *arg)
{
	unsigned long cnt = 0;
	int c;
	c = ACE_getc();
	MFS_control_desc (USO_current()->in, MFS_CTRL_SER_RX_TIMEOUT, 10);
	for (;;)
	{
		if (c == ACE_EOF) break;
		cnt++;
		if (arg[1] == 'h')
			ACE_printf("%02X",c);
		else
			ACE_putc(c);
		c = ACE_getc();
	}
	MFS_control_desc (USO_current()->in, MFS_CTRL_SER_RX_TIMEOUT, 0);
	USO_log_printf(USO_LL_INFO, "axfr: %lu b rx\n", cnt);
}

extern void
CLI_exe_axfr_r (char *arg)
{
	if (arg != NULL) {
		switch (arg[0]){
		case 'a':
			MFS_control_desc (USO_current()->in, MFS_CTRL_TTY_IN_TRANSL, CLI_TTY_INTRANSL_REMOVE_CR);
			axfr_rx(arg);
			MFS_control_desc (USO_current()->in, MFS_CTRL_TTY_DEFAULT_TRANSL, 0);
			break;
		case 'b':
			MFS_control_desc (USO_current()->in, MFS_CTRL_TTY_IN_MODE, CLI_TTY_MODE_RAW);
			axfr_rx(arg);
			MFS_control_desc (USO_current()->in, MFS_CTRL_TTY_IN_MODE, CLI_TTY_MODE_COOKED);
			break;
		default:
			USO_log_puts(USO_LL_ERROR, "axfr: a  ascii, b binary, [h] convert to hex\n");
			break;
		}
	} else {
		USO_log_puts(USO_LL_ERROR, "axfr: arg miss\n");
	}
}

static void
axfr_tx(char *arg)
{
	unsigned long cnt = 0;
	int c;
	for (;;)
	{
		c = ACE_getc();
		if (c == ACE_EOF) break;
		cnt++;
		if (arg[1] == 'h')
			ACE_printf("%02X",c);
		else
			ACE_putc(c);
	}
	USO_log_printf(USO_LL_INFO, "axfr: %lu b tx\n", cnt);
}

extern void
CLI_exe_axfr_s (char *arg)
{
	if (arg != NULL) {
		switch (arg[0]){
		case 'a':
			MFS_control_desc (USO_current()->out, MFS_CTRL_TTY_OUT_TRANSL, CLI_TTY_OUTTRANSL_ADD_CR);
			axfr_tx(arg);
			ACE_putc(ACE_CTRL_D);
			MFS_control_desc (USO_current()->out, MFS_CTRL_TTY_DEFAULT_TRANSL, 0);
			break;
		case 'b':
			MFS_control_desc (USO_current()->out, MFS_CTRL_TTY_OUT_MODE, CLI_TTY_MODE_RAW);
			axfr_tx(arg);
			MFS_control_desc (USO_current()->out, MFS_CTRL_TTY_OUT_MODE, CLI_TTY_MODE_COOKED);
			break;
		default:
			USO_log_puts(USO_LL_ERROR, "axfr: a ascii, b binary, [h] convert to hex\n");
			break;
		}
	} else {
		USO_log_puts(USO_LL_ERROR, "axfr: arg miss\n");
	}
}
