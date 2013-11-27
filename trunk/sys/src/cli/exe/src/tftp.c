/*
 * tftp.c
 *
 *  Created on: 20.11.2013
 *      Author: maik
 */

#include <cli/exec.h>
#include <cli/arg.h>
#include <uso/scheduler.h>
#include <nap/tftp.h>

static CLI_exec_t tftp_get;

static ACE_bool_t
tftp_write_rx_data (char *data, ACE_size_t len, MFS_descriptor_t *out)
{
    ACE_size_t written = ACE_fwrite (out, data, len);
    if (written != len) return FALSE;
    return TRUE;
}

extern ACE_err_t
CLI_tftp_get (char *arg)
{
    ACE_err_t err = ACE_OK;;
    int argc;
    char *argv[CLI_MAX_ARG];

    argc = CLI_arg_parse(arg, argv);
    if (argc >= 1)
    {
        if (NAP_tftp_open () >= 0)
        {
            MFS_descriptor_t *out = USO_current()->out;
            int err = 0;
            if (out->type == MFS_STREAM) {
                err = NAP_tftp_get (argv[0], tftp_write_rx_data, out);
            }

            if (err >= 0)
            {
                USO_log_puts (USO_LL_INFO, "Tftp get done\n");
            }
            else
            {
                USO_log_puts (USO_LL_ERROR, "Tftp get failed\n");
            }
            NAP_tftp_close ();
        }
        else
        {
            USO_log_puts (USO_LL_ERROR, "Tftp open failed\n");
        }
    }
    else
    {
        err = DEF_ERR_ARG;
    }
    return err;
}


extern void
CLI_tftp_install (NET_ip_addr_t * client_address, NET_ip_addr_t * server_address)
{
    NAP_tftp_init(client_address, server_address);
    MFS_descriptor_t *dir = MFS_resolve("/sys/cli/exe");
    CLI_exec_init (dir, &tftp_get, "tftp_g", "TFTP get", CLI_tftp_get);
    MFS_close_desc(dir);
}

