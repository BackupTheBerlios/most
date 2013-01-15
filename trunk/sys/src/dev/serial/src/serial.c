/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

#include <uso/cpu.h>
#include <uso/sleep.h>
#include <ace/stdio.h>
#include <mfs/stream.h>
#include <mfs/directory.h>
#include <mfs/sysfs.h>
#include <dev/serial.h>
#include <dev/err.h>

static void
rx_char (DEV_serial_t * serial, char c)
{
    if (USO_pipe_write_ns (&serial->rx_buf, &c, sizeof (c)) == 0)
    {
        serial->error.rx_buf_overrun++;
    }
    else if (serial->block == TRUE)
    {
        USO_go (&serial->rx_barrier);
    }
}

static void
rx_error (DEV_serial_t * serial, enum DEV_serial_rx_err error)
{
    DEV_serial_error_rx (&serial->error, error);
}

static int
tx_char (DEV_serial_t * serial)
{
    char c;
    if (USO_pipe_read_ns (&serial->tx_buf, &c, sizeof (c)) == 0)
    {
        return ACE_EOF;
    }
    return (unsigned char)c;
}

static void
tx_finished (DEV_serial_t * serial)
{
    serial->tx_busy = FALSE;
}

static const DEV_serial_int_interface_t int_interface = {
    rx_char,
    rx_error,
    tx_char,
    tx_finished
};


static void
tx_start (DEV_serial_t * serial)
{
    if (serial->tx_busy == FALSE)
    {
        serial->tx_busy = TRUE;
        serial->tx_start ();
    }
}


static ACE_err_t
serial_open (MFS_descriptor_t * stream)
{
    DEV_serial_t *serial = (DEV_serial_t *) stream->represent;
    USO_cpu_status_t ps = USO_disable ();

    if (stream->open_cnt == 1){
    	DEV_serial_error_init (&serial->error);
    	USO_pipe_init (&serial->rx_buf, serial->rx_buffer, sizeof (serial->rx_buffer));
    	USO_pipe_init (&serial->tx_buf, serial->tx_buffer, sizeof (serial->tx_buffer));
    	serial->tx_busy = FALSE;
    	USO_mutex_init (&serial->rx_mutex);
    	USO_mutex_init (&serial->tx_mutex);
    	USO_barrier_init (&serial->rx_barrier);
    	serial->open (serial->settings);
    	serial->rx_cancel = FALSE;
    	serial->rx_timeout_sec = 0;
    }
    USO_restore (ps);
    return ACE_OK;
}

static void
serial_close (MFS_descriptor_t * stream)
{
    DEV_serial_t *serial = (DEV_serial_t *)stream->represent;
    USO_cpu_status_t ps = USO_disable ();
    if (stream->open_cnt == 0){
    	serial->close ();
    }
    USO_restore (ps);
}

static void
serial_info (MFS_descriptor_t * stream)
{
    DEV_serial_t *serial = (DEV_serial_t *) stream->represent;
    MFS_stream_print((MFS_stream_t *)stream);
    ACE_printf ("Serial %sblocking IO:\n", serial->block ? "" : "non ");
    DEV_serial_settings_print (serial->settings);
    DEV_serial_error_print (&serial->error);
}

static void
serial_control (MFS_descriptor_t *stream, enum MFS_control_key key, long value)
{
    DEV_serial_t *serial = (DEV_serial_t *) stream->represent;
	switch (key){
	case MFS_CTRL_SER_RX_TIMEOUT:
		serial->rx_timeout_sec = value;
		break;
	default:
		break;
	}
}

static struct MFS_descriptor_op serial_desc_op = {
	.open = serial_open,
    .close = serial_close,
    .info = serial_info,
    .control = serial_control
};

static ACE_size_t
serial_nb_read (MFS_descriptor_t * stream, char *buf, ACE_size_t len)
{
    DEV_serial_t *serial = (DEV_serial_t *) stream->represent;
    ACE_size_t ret;
    USO_cpu_status_t ps = USO_disable ();
    ret = USO_pipe_read_ns (&serial->rx_buf, buf, len);
    ((MFS_stream_t *)stream)->pos_rx += ret;
    USO_restore (ps);
    return ret;
}

static ACE_size_t
serial_nb_write (MFS_descriptor_t * stream, const char *buf, ACE_size_t len)
{
    DEV_serial_t *serial = (DEV_serial_t *) stream->represent;
    ACE_size_t ret;
    USO_cpu_status_t ps = USO_disable ();
    ret = USO_pipe_write_ns (&serial->tx_buf, buf, len);
    if (ret > 0)
    {
        tx_start (serial);
        ((MFS_stream_t *)stream)->size_tx += ret;
    }
    serial->error.tx_buf_overrun += (len - ret);
    USO_restore (ps);
    return ret;
}


static ACE_size_t
serial_b_read (MFS_descriptor_t * stream, char *buf, ACE_size_t len)
{
    DEV_serial_t *serial = (DEV_serial_t *) stream->represent;
    ACE_size_t ret = len;
    ACE_size_t readed;
    USO_lock (&serial->rx_mutex);
    if (serial->rx_timeout_sec > 0){
    	DEV_timer_start (&serial->rx_timer, USO_SEC_2_TICKS(serial->rx_timeout_sec));
    }
    while (len && (serial->rx_cancel == FALSE) )
    {
        USO_cpu_status_t ps = USO_disable ();
        readed = USO_pipe_read_ns (&serial->rx_buf, buf, len);
        len -= readed;
        buf += readed;
        if ( (len > 0) && (serial->rx_cancel == FALSE) )
        {
            USO_block (&serial->rx_barrier);
            if (serial->rx_cancel == FALSE)
            	USO_sleep (USO_MSEC_2_TICKS(serial->settings->buffer_fill_time));
        }
        USO_restore (ps);
    }
    ((MFS_stream_t *)stream)->pos_rx += ret - len;
    serial->rx_cancel = FALSE;
    DEV_timer_stop (&serial->rx_timer);
    USO_unlock (&serial->rx_mutex);
    return ret - len;
}

static ACE_size_t
serial_b_write (MFS_descriptor_t * stream, const char *buf, ACE_size_t len)
{
    DEV_serial_t *serial = (DEV_serial_t *) stream->represent;
    ACE_size_t ret = len;
    ACE_size_t written;
    USO_lock (&serial->tx_mutex);
    while (len)
    {
        USO_cpu_status_t ps = USO_disable ();
        written = USO_pipe_write_ns (&serial->tx_buf, buf, len);
        if (written > 0)
        {
            len -= written;
            buf += written;
            tx_start (serial);
        }
        if (len > 0)
        {
            USO_sleep (USO_MSEC_2_TICKS(serial->settings->buffer_empty_time));
        }
        USO_restore (ps);
    }
    ((MFS_stream_t *)stream)->size_tx += ret;
    USO_unlock (&serial->tx_mutex);
    return ret;
}

static struct MFS_stream_op serial_b_stream_op = {
    .read = serial_b_read,
    .write = serial_b_write,
    .seek = NULL,
    .flush = NULL
};

static struct MFS_stream_op serial_nb_stream_op = {
	.read = serial_nb_read,
	.write = serial_nb_write,
    .seek = NULL,
    .flush = NULL
};

static void rx_timeout(void * serial)
{
	DEV_serial_rx_cancel((DEV_serial_t *)serial);
}

extern void
DEV_serial_init (DEV_serial_t * serial,
                 const struct DEV_serial_settings *settings,
                 void (*open) (const struct DEV_serial_settings *),
                 void (*close) (void), void (*tx_start) (void), ACE_bool_t block, char *name)
{
    serial->settings = settings;
    serial->open = open;
    serial->close = close;
    serial->tx_start = tx_start;
    serial->int_interface = &int_interface;
    serial->block = block;
    DEV_timer_init(&serial->rx_timer, rx_timeout, serial, DEV_TIMER_INT);
    DEV_timer_install (&serial->rx_timer, "srx_to");
    MFS_stream_create_io (MFS_resolve(MFS_get_root(), "sys/dev/serial"), name, &serial_desc_op,
                   (serial->block == TRUE) ? &serial_b_stream_op : &serial_nb_stream_op,
                   (MFS_represent_t *) serial);
}

extern void
DEV_serial_rx_cancel(DEV_serial_t *serial)
{
	serial->rx_cancel = TRUE;
    USO_go (&serial->rx_barrier);
}
