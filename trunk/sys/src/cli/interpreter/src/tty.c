/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

#include <ace/stdio.h>
#include <ace/stddef.h>
#include <ace/stdlib.h>
#include <uso/cpu.h>
#include <dev/serial.h>
#include <mfs/stream.h>
#include <mfs/directory.h>
#include <mfs/sysfs.h>
#include <cli/tty.h>
#include <cli/err.h>

static ACE_err_t
tty_open (MFS_descriptor_t * desc)
{
    CLI_tty_t *tty = (CLI_tty_t *) desc->represent;
    USO_lock(&tty->lock);
    if (desc->open_cnt == 0){
    }
    USO_unlock(&tty->lock);
    return ACE_OK;
}

static ACE_err_t
tty_close (MFS_descriptor_t * desc)
{
    CLI_tty_t *tty = (CLI_tty_t *) desc->represent;
    USO_lock(&tty->lock);
    if (desc->open_cnt == 1){
    }
    USO_unlock(&tty->lock);
    return ACE_OK;
}

static void
tty_info (MFS_descriptor_t * desc, int number, MFS_info_entry_t *entry)
{
    CLI_tty_t *tty = (CLI_tty_t *) desc->represent;
    switch (number){
        case 0:
        case 1:
        case 2:
            MFS_stream_info((MFS_stream_t *)desc, number, entry);
            break;
        case 3:
            entry->type = MFS_INFO_STRING;
            entry->name = "in_mode";
            entry->value.s = (tty->in_mode == CLI_TTY_MODE_RAW) ? "RAW" : "COOKED";
            break;
        case 4:
            entry->type = MFS_INFO_LONG;
            entry->name = "in_transl";
            entry->value.l = tty->in_transl;
            break;
        case 5:
            entry->type = MFS_INFO_STRING;
            entry->name = "in_stream";
            entry->value.s = tty->in_stream->name;
            break;
        case 6:
            entry->type = MFS_INFO_STRING;
            entry->name = "out_mode";
            entry->value.s = (tty->out_mode == CLI_TTY_MODE_RAW) ? "RAW" : "COOKED";
            break;
        case 7:
            entry->type = MFS_INFO_LONG;
            entry->name = "out_transl";
            entry->value.l = tty->out_transl;
            break;
        case 8:
            entry->type = MFS_INFO_STRING;
            entry->name = "out_stream";
            entry->value.s = tty->out_stream->name;
            break;
        default:
            entry->type = MFS_INFO_NOT_AVAIL;
            break;
   }
}

static void
tty_control (MFS_descriptor_t * desc, int number, MFS_ctrl_entry_t *entry)
{
    CLI_tty_t *tty = (CLI_tty_t *) desc->represent;
    switch (number){
        case 0:
            if (entry->type == MFS_CTRL_INFO){
                ACE_sprintf(entry->value.s, "1 l in_mode\n"
                                            "2 l out_mode\n"
                                            "3 l in_transl\n"
                                            "4 l out_transl\n"
                                            "5   def_transl\n"
                                            "6 l rx_timeout");
            }
            break;
        case CLI_TTY_CTRL_IN_MODE:
            if (entry->type == MFS_CTRL_LONG){
                tty->in_mode = entry->value.l;
            }
            break;
        case CLI_TTY_CTRL_OUT_MODE:
            if (entry->type == MFS_CTRL_LONG){
                tty->out_mode = entry->value.l;
            }
            break;
        case CLI_TTY_CTRL_IN_TRANSL:
            if (entry->type == MFS_CTRL_LONG){
                tty->in_transl = entry->value.l;
            }
            break;
        case CLI_TTY_CTRL_OUT_TRANSL:
            if (entry->type == MFS_CTRL_LONG){
                tty->out_transl = entry->value.l;
            }
            break;
        case CLI_TTY_CTRL_DEFAULT_TRANSL:
            tty->in_transl = tty->in_transl_default;
            tty->out_transl = tty->out_transl_default;
            break;
        case CLI_TTY_CTRL_RX_TIMEOUT:
            if (entry->type == MFS_CTRL_LONG){
                MFS_control_desc(tty->in_stream, DEV_SER_CTRL_RX_TIMEOUT, entry);
            }
            break;
        default:
            break;
    }
}

static struct MFS_descriptor_op tty_desc_op = {
    .open = tty_open,
    .close = tty_close,
    .info = tty_info,
    .control = tty_control,
    .delete = NULL
};

static ACE_size_t
tty_read (MFS_stream_t *stream, char *buf, ACE_size_t len)
{
    CLI_tty_t *tty = (CLI_tty_t *) ((MFS_descriptor_t *)stream)->represent;
    ACE_size_t ret = 0;
    int c = ACE_EOF;
    if (tty->in_stream != NULL)
    {
        if ( tty->in_mode == CLI_TTY_MODE_RAW )
        {
            return ACE_fread (tty->in_stream, buf, len);
        }
        else
        {
            while (len > 0)
            {
                c = ACE_fgetc (tty->in_stream);
                if ( (c == ACE_EOF) || (c == ACE_CTRL_D) )
                {
                    break;
                }
                switch (tty->in_transl) {
                    case CLI_TTY_INTRANSL_NONE:
                        *buf++ = c;
                        break;
                    case CLI_TTY_INTRANSL_CR_2_NL:
                        *buf++ = (c != '\r') ? c : '\n';
                        break;
                    case CLI_TTY_INTRANSL_REMOVE_CR:
                        if (c != '\r') *buf++ = c;
                        else {
                            len++;
                            ret--;
                        }
                        break;
                    default:
                        *buf++ = c;
                        break;
                }
                len--;
                ret++;
            }
        }
    }
    stream->pos_rx += ret;

    return ret;
}

static ACE_size_t
tty_write (MFS_stream_t *stream, const char *buf, ACE_size_t len)
{
    CLI_tty_t *tty = (CLI_tty_t *) ((MFS_descriptor_t *)stream)->represent;
    ACE_size_t ret = 0;
    if (tty->out_stream != NULL)
    {
        if ( tty->out_mode == CLI_TTY_MODE_RAW )
        {
            return ACE_fwrite (tty->out_stream, (char *)buf, len);
        }
        else
        {
            while (len > 0)
            {
                char c = *buf++;
                if (c == '\n')
                {
                    switch (tty->out_transl) {
                    case CLI_TTY_OUTTRANSL_NONE:
                        break;
                    case CLI_TTY_OUTTRANSL_NL_2_CR:
                        c = '\r';
                        break;
                    case CLI_TTY_OUTTRANSL_ADD_CR:
                        c = '\r';
                        ACE_fputc (tty->out_stream, c);
                        c = '\n';
                        break;
                    default:
                        break;
                    }
                }
                ACE_fputc (tty->out_stream, c);
                len--;
                ret++;
            }
        }
    }
    stream->size_tx += ret;

    return ret;
}


static struct MFS_stream_op tty_stream_op = {
    .read = tty_read,
    .write = tty_write,
    .seek = NULL,
    .flush = NULL
};


extern void
CLI_tty_init (CLI_tty_t * tty,
              MFS_descriptor_t *in_stream,
              MFS_descriptor_t *out_stream,
              enum CLI_tty_in_transl in_transl,
              enum CLI_tty_out_transl out_transl, char *name)
{
    tty->in_stream = in_stream;
    tty->out_stream = out_stream;
    tty->in_transl = in_transl;
    tty->out_transl = out_transl;
    tty->in_transl_default = in_transl;
    tty->out_transl_default = out_transl;
    tty->in_mode = CLI_TTY_MODE_COOKED;
    tty->out_mode = CLI_TTY_MODE_COOKED;
    USO_mutex_init(&tty->lock);
    MFS_descriptor_t *dir = MFS_resolve("/sys/cli");
    MFS_stream_create (dir, name, &tty_desc_op,
                   &tty_stream_op, (MFS_represent_t *) tty, MFS_STREAM_IO);
    MFS_close_desc(dir);
}
