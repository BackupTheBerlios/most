/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

#include <ace/stdio.h>
#include <uso/heap.h>

#include "dev/serial_error.h"

extern void
DEV_serial_error_init (struct DEV_serial_error *error)
{
    error->rx_overrun = 0;
    error->rx_framing = 0;
    error->rx_parity = 0;
    error->rx_buf_overrun = 0;
    error->tx_buf_overrun = 0;
}

extern void
DEV_serial_error_print (struct DEV_serial_error *error)
{
    printf ("\tError:\n"
                "\t\tOverrun\t%lu\n"
                "\t\tFraming\t%lu\n"
                "\t\tParity\t%lu\n"
                "\t\tRX_Buf_Overrun\t%lu\n"
                "\t\tTX_Buf_Overrun\t%lu\n",
                error->rx_overrun,
                error->rx_framing,
                error->rx_parity, error->rx_buf_overrun, error->tx_buf_overrun);
}

extern void
DEV_serial_error_rx (struct DEV_serial_error *error,
                     enum DEV_serial_rx_err err_code)
{
    switch (err_code)
    {
    case DEV_SER_RX_OVERRUN:
        error->rx_overrun++;
        break;
    case DEV_SER_RX_FRAMING:
        error->rx_framing++;
        break;
    case DEV_SER_RX_PARITY:
        error->rx_parity++;
        break;
    default:
        break;
    }
}
