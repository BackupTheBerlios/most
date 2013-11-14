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
    MFS_ctrl_entry_t tty_ctrl;
    tty_ctrl.type = MFS_CTRL_LONG;
    tty_ctrl.value.l = 10;
    MFS_control_desc (USO_current()->in, CLI_TTY_CTRL_RX_TIMEOUT, &tty_ctrl);
    for (c = ACE_getc(); c != ACE_EOF; c = ACE_getc(), cnt++)
    {
        if (arg[1] == 'h')
            ACE_printf("%02X",c);
        else
            ACE_putc(c);
    }
    tty_ctrl.value.l = 0;
    MFS_control_desc (USO_current()->in, CLI_TTY_CTRL_RX_TIMEOUT, &tty_ctrl);
    USO_log_printf(USO_LL_INFO, "%lu bytes received\n", cnt);    /* can not use ACE_printf */
}

extern ACE_err_t
CLI_exe_axfr_r (char *arg)
{
    ACE_err_t err = ACE_OK;
    MFS_ctrl_entry_t tty_ctrl;
    tty_ctrl.type = MFS_CTRL_LONG;
    if (arg != NULL) {
        switch (arg[0]){
            case 'a':
                tty_ctrl.value.l = CLI_TTY_INTRANSL_REMOVE_CR;
                MFS_control_desc (USO_current()->in, CLI_TTY_CTRL_IN_TRANSL, &tty_ctrl);
                axfr_rx(arg);
                MFS_control_desc (USO_current()->in, CLI_TTY_CTRL_DEFAULT_TRANSL, &tty_ctrl);
                break;
            case 'b':
                tty_ctrl.value.l = CLI_TTY_MODE_RAW;
                MFS_control_desc (USO_current()->in, CLI_TTY_CTRL_IN_MODE, &tty_ctrl);
                axfr_rx(arg);
                tty_ctrl.value.l = CLI_TTY_MODE_COOKED;
                MFS_control_desc (USO_current()->in, CLI_TTY_CTRL_IN_MODE, &tty_ctrl);
                break;
            case '?':
                USO_log_printf(USO_LL_INFO, "arg[0] = a ascii | b binary, arg[1]opt = h convert to hex\n"); /* can not use ACE_printf */
                break;
            default:
                err = DEF_ERR_ARG;
                break;
        }
    } else {
        err = DEF_ERR_ARG;
    }
    return err;
}

static void
axfr_tx(char *arg)
{
    unsigned long cnt = 0;
    int c;
    for (c = ACE_getc(); c != ACE_EOF;c = ACE_getc(), cnt++)
    {
        if (arg[1] == 'h')
            ACE_printf("%02X",c);
        else
            ACE_putc(c);
    }
    USO_log_printf(USO_LL_INFO, "%lu bytes transmitted\n", cnt);  /* can not use ACE_printf */
}

extern ACE_err_t
CLI_exe_axfr_s (char *arg)
{
    ACE_err_t err = ACE_OK;
    MFS_ctrl_entry_t tty_ctrl;
    tty_ctrl.type = MFS_CTRL_LONG;
    if (arg != NULL) {
        switch (arg[0]){
            case 'a':
                tty_ctrl.value.l = CLI_TTY_OUTTRANSL_ADD_CR;
                MFS_control_desc (USO_current()->out, CLI_TTY_CTRL_OUT_TRANSL, &tty_ctrl);
                axfr_tx(arg);
                ACE_putc(ACE_CTRL_D);
                MFS_control_desc (USO_current()->out, CLI_TTY_CTRL_DEFAULT_TRANSL, &tty_ctrl);
                break;
            case 'b':
                tty_ctrl.value.l = CLI_TTY_MODE_RAW;
                MFS_control_desc (USO_current()->out, CLI_TTY_CTRL_OUT_MODE, &tty_ctrl);
                axfr_tx(arg);
                tty_ctrl.value.l = CLI_TTY_MODE_COOKED;
                MFS_control_desc (USO_current()->out, CLI_TTY_CTRL_OUT_MODE, &tty_ctrl);
                break;
            case '?':
                USO_log_printf(USO_LL_INFO, "arg[0] = a ascii | b binary, arg[1]opt = h convert to hex\n"); /* can not use ACE_printf */
                break;
            default:
                err = DEF_ERR_ARG;
                break;
        }
    } else {
        err = DEF_ERR_ARG;
    }
    return err;
}
