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
tty_open (MFS_descriptor_t * tty_stream)
{
    CLI_tty_t *tty = (CLI_tty_t *) tty_stream->represent;
    USO_cpu_status_t ps = USO_disable ();
    if (tty_stream->open_cnt == 1){
    	tty->io_stream = MFS_open (tty->io_stream_dir, tty->io_stream_name);
    	if (tty->io_stream == NULL)
    	{
    		ACE_PANIC ("TTY:open stream failed");
    	}
    }
    USO_restore (ps);
    return ACE_OK;
}

static void
tty_close (MFS_descriptor_t * tty_stream)
{
    CLI_tty_t *tty = (CLI_tty_t *) tty_stream->represent;
    USO_cpu_status_t ps = USO_disable ();
    if (tty_stream->open_cnt == 0){
    	if (tty->io_stream != NULL)
    	{
    		MFS_close_desc (tty->io_stream);
    	}
    }
    USO_restore (ps);
}

static void
tty_info (MFS_descriptor_t * tty_stream)
{
    CLI_tty_t *tty = (CLI_tty_t *) tty_stream->represent;
    MFS_stream_print((MFS_stream_t *)tty_stream);
    tty_print (tty);
    if (tty->io_stream != NULL)
    {
        MFS_info_desc (tty->io_stream);
    }
}

static void
tty_control (MFS_descriptor_t * tty_stream, enum MFS_control_key key, long value)
{
    CLI_tty_t *tty = (CLI_tty_t *) tty_stream->represent;
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
		MFS_control_desc(tty->io_stream, key, value);
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
tty_read (MFS_descriptor_t * tty_stream, char *buf, ACE_size_t len)
{
    CLI_tty_t *tty = (CLI_tty_t *) tty_stream->represent;
    ACE_size_t ret = 0;
    int c = ACE_EOF;
    if (tty->io_stream != NULL)
    {
        if ( tty->in_mode == CLI_TTY_MODE_RAW )
        {
            return ACE_fread (tty->io_stream, buf, len);
        }
        else
        {
            while (len > 0)
            {
                c = ACE_fgetc (tty->io_stream);
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
    return ret;
}

static ACE_size_t
tty_write (MFS_descriptor_t * tty_stream, const char *buf, ACE_size_t len)
{
    CLI_tty_t *tty = (CLI_tty_t *) tty_stream->represent;
    ACE_size_t ret = 0;
    if (tty->io_stream != NULL)
    {
        if ( tty->out_mode == CLI_TTY_MODE_RAW )
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
                    switch (tty->out_transl) {
                    case CLI_TTY_OUTTRANSL_NONE:
                    	break;
                    case CLI_TTY_OUTTRANSL_NL_2_CR:
                        c = '\r';
                    	break;
                    case CLI_TTY_OUTTRANSL_ADD_CR:
                        c = '\r';
                        ACE_fputc (tty->io_stream, c);
                        c = '\n';
                    	break;
                    default:
                    	break;
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
    .read = tty_read,
    .write = tty_write,
    .seek = NULL,
    .flush = NULL
};


extern void
CLI_tty_init (CLI_tty_t * tty,
              MFS_descriptor_t *io_stream_dir,
              char *io_stream_name,
              enum CLI_tty_in_transl in_transl,
              enum CLI_tty_out_transl out_transl, char *name)
{
    tty->io_stream = NULL;
    tty->io_stream_dir = io_stream_dir;
    tty->io_stream_name = io_stream_name;
    tty->in_transl = in_transl;
    tty->out_transl = out_transl;
    tty->in_transl_default = in_transl;
    tty->out_transl_default = out_transl;
    tty->in_mode = CLI_TTY_MODE_COOKED;
    tty->out_mode = CLI_TTY_MODE_COOKED;
    MFS_stream_create_io (MFS_resolve(MFS_get_root(), "sys/cli"), name, &tty_desc_op,
                   &tty_stream_op, (MFS_represent_t *) tty);
}
