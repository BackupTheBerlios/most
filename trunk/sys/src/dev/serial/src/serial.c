/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

#include <uso/arch/cpu.h>
#include <uso/sleep.h>
#include <ace/stdio.h>
#include <mfs/stream.h>
#include <mfs/directory.h>
#include <mfs/sysfs.h>

#include "dev/serial.h"

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


static void
serial_open (ACE_FILE * stream)
{
    DEV_serial_t *serial = (DEV_serial_t *) stream->represent;
    USO_cpu_status_t ps = USO_disable ();

    DEV_serial_error_init (&serial->error);
    USO_pipe_init (&serial->rx_buf, serial->rx_buffer, sizeof (serial->rx_buffer));
    USO_pipe_init (&serial->tx_buf, serial->tx_buffer, sizeof (serial->tx_buffer));
    serial->tx_busy = FALSE;
    USO_mutex_init (&serial->rx_mutex);
    USO_mutex_init (&serial->tx_mutex);
    USO_barrier_init (&serial->rx_barrier);
    serial->open (serial->settings);
    USO_restore (ps);
}

static void
serial_close (ACE_FILE * stream)
{
    DEV_serial_t *serial = (DEV_serial_t *) stream->represent;
    USO_cpu_status_t ps = USO_disable ();
    serial->close ();
    USO_restore (ps);
}

static void
serial_info (ACE_FILE * stream)
{
    DEV_serial_t *serial = (DEV_serial_t *) stream->represent;
    ACE_printf ("Serial %sblocking IO:\n", serial->block ? "" : "non ");
    DEV_serial_settings_print (serial->settings);
    DEV_serial_error_print (&serial->error);
    ACE_printf ("\n");
}


static ACE_size_t
serial_nb_read (ACE_FILE * stream, char *buf, ACE_size_t len)
{
    DEV_serial_t *serial = (DEV_serial_t *) stream->represent;
    ACE_size_t ret;
    USO_cpu_status_t ps = USO_disable ();
    ret = USO_pipe_read_ns (&serial->rx_buf, buf, len);
    stream->pos_rx += ret;
    USO_restore (ps);
    return ret;
}

static ACE_size_t
serial_nb_write (ACE_FILE * stream, const char *buf, ACE_size_t len)
{
    DEV_serial_t *serial = (DEV_serial_t *) stream->represent;
    ACE_size_t ret;
    USO_cpu_status_t ps = USO_disable ();
    ret = USO_pipe_write_ns (&serial->tx_buf, buf, len);
    if (ret > 0)
    {
        tx_start (serial);
        stream->size_tx += ret;
    }
    serial->error.tx_buf_overrun += (len - ret);
    USO_restore (ps);
    return ret;
}


static ACE_size_t
serial_b_read (ACE_FILE * stream, char *buf, ACE_size_t len)
{
    DEV_serial_t *serial = (DEV_serial_t *) stream->represent;
    ACE_size_t ret = len;
    ACE_size_t readed;
    USO_lock (&serial->rx_mutex);
    while (len)
    {
        USO_cpu_status_t ps = USO_disable ();
        readed = USO_pipe_read_ns (&serial->rx_buf, buf, len);
        len -= readed;
        buf += readed;
        if (len > 0)
        {
            USO_block (&serial->rx_barrier);
            USO_sleep (serial->settings->timeout);
        }
        USO_restore (ps);
    }
    stream->pos_rx += ret;
    USO_unlock (&serial->rx_mutex);
    return ret;
}

static ACE_size_t
serial_b_write (ACE_FILE * stream, const char *buf, ACE_size_t len)
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
            USO_sleep (serial->settings->timeout);
        }
        USO_restore (ps);
    }
    stream->size_tx += ret;
    USO_unlock (&serial->tx_mutex);
    return ret;
}

static struct MFS_stream_op serial_b_stream_op = {.open = serial_open,
    .close = serial_close,
    .info = serial_info,
    .read = serial_b_read,
    .write = serial_b_write,
    .seek = NULL,
    .flush = NULL
};

static struct MFS_stream_op serial_nb_stream_op = {.open = serial_open,
    .close = serial_close,
    .info = serial_info,
    .read = serial_nb_read,
    .write = serial_nb_write,
    .seek = NULL,
    .flush = NULL
};

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
    MFS_create_io (MFS_sysfs_get_dir (MFS_SYSFS_DIR_SERIAL), name,
                   (serial->block == TRUE) ? &serial_b_stream_op : &serial_nb_stream_op,
                   (MFS_stream_represent_t *) serial);
}
