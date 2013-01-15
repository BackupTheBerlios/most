/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

#include <ace/stdio.h>
#include <uso/heap.h>

#include "dev/serial_settings.h"

extern void
DEV_serial_settings_init (struct DEV_serial_settings *settings)
{
    settings->baud = DEV_SER_BAUD_9600;
    settings->data = DEV_SER_DATA_8;
    settings->stop = DEV_SER_STOP_1;
    settings->parity = DEV_SER_PARITY_NONE;
    settings->buffer_fill_time = DEV_SER_BUFFER_FILL_TIME_MSEC;
    settings->buffer_empty_time = DEV_SER_BUFFER_EMPTY_TIME_MSEC;
}

extern void
DEV_serial_settings_print (const struct DEV_serial_settings *settings)
{
    char *baud, *data, *stop, *parity, *error;
    error = "error";
    switch (settings->baud)
    {
    case DEV_SER_BAUD_1200:
        baud = "1200";
        break;
    case DEV_SER_BAUD_2400:
        baud = "2400";
        break;
    case DEV_SER_BAUD_4800:
        baud = "4800";
        break;
    case DEV_SER_BAUD_9600:
        baud = "9600";
        break;
    case DEV_SER_BAUD_19200:
        baud = "19200";
        break;
    case DEV_SER_BAUD_38400:
        baud = "38400";
        break;
    default:
        baud = error;
        break;
    }
    switch (settings->data)
    {
    case DEV_SER_DATA_7:
        data = "7";
        break;
    case DEV_SER_DATA_8:
        data = "8";
        break;
    default:
        data = error;
        break;
    }
    switch (settings->stop)
    {
    case DEV_SER_STOP_1:
        stop = "1";
        break;
    case DEV_SER_STOP_2:
        stop = "2";
        break;
    default:
        stop = error;
        break;
    }
    switch (settings->parity)
    {
    case DEV_SER_PARITY_NONE:
        parity = "none";
        break;
    case DEV_SER_PARITY_EVEN:
        parity = "even";
        break;
    case DEV_SER_PARITY_ODD:
        parity = "odd";
        break;
    default:
        parity = error;
        break;
    }
    ACE_printf ("\tSettings:\n"
                "\t\tBaud\t%s\n"
                "\t\tData\t%s\n"
                "\t\tStop\t%s\n"
                "\t\tParity\t%s\n"
                "\t\tBuffer fill time\t%d ms\n"
    			"\t\tBuffer empty time\t%d ms\n", baud, data, stop, parity, settings->buffer_fill_time, settings->buffer_empty_time);
}
