/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

#include <ace/stdio.h>
#include <ace/stdlib.h>
#include <uso/arch/cpu.h>
#include <mfs/stream.h>
#include <mfs/directory.h>
#include <mfs/sysfs.h>

#include "cli/tty.h"

static void
tty_print (CLI_tty_t * tty)
{
    char *tx_mode;
    char *rx_mode;
    switch (tty->tx_mode)
    {
    case CLI_TTY_MODE_NL2NL:
        tx_mode = "NL2NL";
        break;
    case CLI_TTY_MODE_NL2CR:
        tx_mode = "NL2CR";
        break;
    case CLI_TTY_MODE_NL2CRNL:
        tx_mode = "NL2CRNL";
        break;
    default:
        tx_mode = "error";
        break;
    }
    switch (tty->rx_mode)
    {
    case CLI_TTY_MODE_NL2NL:
        rx_mode = "NL2NL";
        break;
    case CLI_TTY_MODE_NL2CR:
        rx_mode = "NL2CR";
        break;
    case CLI_TTY_MODE_NL2CRNL:
        rx_mode = "NL2CRNL";
        break;
    default:
        rx_mode = "error";
        break;
    }
    ACE_printf ("TTY tx mode: %s rx mode: %s\n", tx_mode, rx_mode);
}

static void
tty_open (MFS_stream_t * tty_stream)
{
    CLI_tty_t *tty = (CLI_tty_t *) tty_stream->represent;
    USO_cpu_status_t ps = USO_disable ();
    tty->io_stream_desc = MFS_open (MFS_sysfs_get_dir (tty->io_stream_dir), tty->io_stream_name);
    if (tty->io_stream_desc == NULL)
    {
        ACE_PANIC ("TTY:open stream failed");
    }
    tty->io_stream = MFS_get_stream (tty->io_stream_desc);
    USO_restore (ps);
}

static void
tty_close (MFS_stream_t * tty_stream)
{
    CLI_tty_t *tty = (CLI_tty_t *) tty_stream->represent;
    USO_cpu_status_t ps = USO_disable ();
    if (tty->io_stream_desc != NULL)
    {
        MFS_close_desc (tty->io_stream_desc);
    }
    USO_restore (ps);
}

static void
tty_info (MFS_stream_t * tty_stream)
{
    CLI_tty_t *tty = (CLI_tty_t *) tty_stream->represent;
    tty_print (tty);
    if (tty->io_stream_desc != NULL)
    {
        MFS_info_desc (tty->io_stream_desc);
    }
}

static int t = '\0';

static ACE_size_t
tty_read (MFS_stream_t * tty_stream, char *buf, ACE_size_t len)
{
    CLI_tty_t *tty = (CLI_tty_t *) tty_stream->represent;
    ACE_size_t ret = 0;
    int c = ACE_EOF;
    if (tty->io_stream != NULL)
    {
        if (tty->rx_mode == CLI_TTY_MODE_NL2NL)
        {
            return ACE_fread (tty->io_stream, buf, len);
        }
        else
        {
            while (len > 0)
            {
                if (tty->rx_mode == CLI_TTY_MODE_NL2CR)
                {
                    c = ACE_fgetc (tty->io_stream);
                    if (c == ACE_EOF)
                    {
                        return c;
                    }
                    *buf++ = (c != '\r') ? c : '\n';
                }
                else if (tty->rx_mode == CLI_TTY_MODE_NL2CRNL)
                {
                    if (t != '\0')
                    {
                        if (t != '\r')
                        {
                            *buf++ = c;
                            t = '\0';
                        }
                        else
                        {
                            c = ACE_fgetc (tty->io_stream);
                            if (c == ACE_EOF)
                            {
                                return c;
                            }
                            if (c != '\n')
                            {
                                *buf++ = t;
                                t = c;
                            }
                            else
                            {
                                *buf++ = c;
                                t = '\0';
                            }
                        }
                    }
                    else
                    {
                        c = ACE_fgetc (tty->io_stream);
                        if (c == ACE_EOF)
                        {
                            return c;
                        }
                        if (c != '\r')
                        {
                            *buf++ = c;
                        }
                        else
                        {
                            t = c;
                            continue;
                        }
                    }
                }
                len--;
                ret++;
            }
        }
    }
    return ret;
}

static ACE_size_t
tty_write (MFS_stream_t * tty_stream, const char *buf, ACE_size_t len)
{
    CLI_tty_t *tty = (CLI_tty_t *) tty_stream->represent;
    ACE_size_t ret = 0;
    if (tty->io_stream != NULL)
    {
        if (tty->tx_mode == CLI_TTY_MODE_NL2NL)
        {
            return ACE_fwrite (tty->io_stream, (char *)buf, len);
        }
        else
        {
            while (len > 0)
            {
                char c = *buf++;
                if (c == '\n')
                {
                    if (tty->tx_mode == CLI_TTY_MODE_NL2CR)
                    {
                        c = '\r';
                    }
                    else if (tty->tx_mode == CLI_TTY_MODE_NL2CRNL)
                    {
                        c = '\r';
                        ACE_fputc (tty->io_stream, c);
                        c = '\n';
                    }
                }
                ACE_fputc (tty->io_stream, c);
                len--;
                ret++;
            }
        }
    }
    return ret;
}

static struct MFS_stream_op tty_stream_op = {
    .open = tty_open,
    .close = tty_close,
    .info = tty_info,
    .read = tty_read,
    .write = tty_write,
    .seek = NULL,
    .flush = NULL
};

extern void
CLI_tty_init (CLI_tty_t * tty,
              enum MFS_sysfs_dir io_stream_dir,
              char *io_stream_name,
              enum CLI_tty_mode tx_mode, enum CLI_tty_mode rx_mode, char *name)
{
    tty->io_stream = NULL;
    tty->io_stream_dir = io_stream_dir;
    tty->io_stream_name = io_stream_name;
    tty->tx_mode = tx_mode;
    tty->rx_mode = rx_mode;
    MFS_create_io (MFS_sysfs_get_dir (MFS_SYSFS_DIR_CLI), name,
                   &tty_stream_op, (MFS_stream_represent_t *) tty);
}
