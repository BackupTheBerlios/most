/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

#include <ace/stdio.h>
#include <ace/stddef.h>
#include <ace/stdlib.h>
#include <uso/cpu.h>
#include <mfs/stream.h>
#include <mfs/directory.h>
#include <mfs/sysfs.h>
#include <cli/tty.h>
#include <cli/err.h>

static void
tty_print (CLI_tty_t * tty)
{
	char *in_mode;
	char *out_mode;
    switch (tty->in_mode)
    {
    case CLI_TTY_MODE_RAW:
        in_mode = "RAW";
        break;
    case CLI_TTY_MODE_COOKED:
        in_mode = "COOKED";
        break;
    default:
        in_mode = "?";
        break;
    }
    switch (tty->out_mode)
    {
    case CLI_TTY_MODE_RAW:
        out_mode = "RAW";
        break;
    case CLI_TTY_MODE_COOKED:
        out_mode = "COOKED";
        break;
    default:
        out_mode = "?";
        break;
    }
    ACE_printf ("TTY: in_mode: %s, out_mode %s, in_transl: %i, out_transl: %i\n", in_mode, out_mode, tty->in_transl, tty->out_transl);
}

static ACE_err_t
tty_open (MFS_descriptor_t * desc)
{
    CLI_tty_t *tty = (CLI_tty_t *) desc->represent;
    USO_lock(&tty->lock);
    if (desc->open_cnt == 1){
    }
    USO_unlock(&tty->lock);
    return ACE_OK;
}

static void
tty_close (MFS_descriptor_t * desc)
{
    CLI_tty_t *tty = (CLI_tty_t *) desc->represent;
    USO_lock(&tty->lock);
    if (desc->open_cnt == 0){
    }
    USO_unlock(&tty->lock);
}

static void
tty_info (MFS_descriptor_t * desc)
{
    CLI_tty_t *tty = (CLI_tty_t *) desc->represent;
    MFS_stream_print((MFS_stream_t *)desc);
    tty_print (tty);
    if (tty->in_stream != NULL)
    {
        MFS_info_desc (tty->in_stream);
    }
    if (tty->out_stream != NULL)
    {
        MFS_info_desc (tty->out_stream);
    }
}

static void
tty_control (MFS_descriptor_t * desc, enum MFS_control_key key, long value)
{
    CLI_tty_t *tty = (CLI_tty_t *) desc->represent;
	switch (key){
	case MFS_CTRL_TTY_IN_MODE:
		tty->in_mode = value;
		break;
	case MFS_CTRL_TTY_OUT_MODE:
		tty->out_mode = value;
		break;
	case MFS_CTRL_TTY_IN_TRANSL:
		tty->in_transl = value;
		break;
	case MFS_CTRL_TTY_OUT_TRANSL:
		tty->out_transl = value;
		break;
	case MFS_CTRL_TTY_DEFAULT_TRANSL:
		tty->in_transl = tty->in_transl_default;
		tty->out_transl = tty->out_transl_default;
		break;
	default:
		MFS_control_desc(tty->in_stream, key, value);
		MFS_control_desc(tty->out_stream, key, value);
		break;
	}
}

static struct MFS_descriptor_op tty_desc_op = {
    .open = tty_open,
    .close = tty_close,
    .info = tty_info,
    .control = tty_control
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
    MFS_stream_create (MFS_resolve(MFS_get_root(), "sys/cli"), name, &tty_desc_op,
                   &tty_stream_op, (MFS_represent_t *) tty, MFS_STREAM_IO);
}
