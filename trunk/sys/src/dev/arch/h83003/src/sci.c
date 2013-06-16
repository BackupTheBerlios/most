/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

#include <ace/stddef.h>
#include <ace/stdio.h>
#include "dev/cpu.h"
#include "dev/arch/h83003/sci.h"
#include "dev/arch/h83003/H83003_REG.h"
#include "dev/arch/h83003/H83003.h"

/**
 * \brief Data structure to access the registers of SCI
 * 
 * Registers for serial communication in order of their H8 register addresses.
 */
struct sci_reg
{
    volatile unsigned char SMR; // /< serial mode register
    volatile unsigned char BRR; // /< baud rate register
    volatile unsigned char SCR; // /< serial control register
    volatile unsigned char TDR; // /< transmit data register
    volatile unsigned char SSR; // /< serial status register
    volatile unsigned char RDR; // /< receive data register
} ACE_PACKED_;

typedef struct sci_reg sci_reg_t;

/**
 * \brief Address table of the ports register.
 */
static sci_reg_t *const ports[] = {
    (sci_reg_t *) & H8_SCI_SMR0,
    (sci_reg_t *) & H8_SCI_SMR1
};


/**
 * \name Tables with values for the port settings
 * \brief The tables contain the information to translate the parameter of the 
 *        \ref COM_PROPERTIES to register settings.
 * 
 * \attention Changing the tables content must be in order to their enumerations
 *            because they are used as index for the tables.
 * @{
 */




/**
 * \brief Holds information for baudrate setting.
 * 
 * The baudrate is set via the smr clock source setting and the value of brr.
 */
struct baud_rate_table
{
    unsigned char smr;          // /< Serial mode register clock source
    // for a certaion baud rate.
    unsigned char brr;          // /< Baud rate register value decides
    // baudrate of port. See ref\ _BRR
    unsigned short delay;       // /< Milliseconds to wait if port is new
    // initialized
};
typedef struct baud_rate_table baud_rate_table_t;

/**
 * \brief The settings for the supplied baudrates
 */
static const baud_rate_table_t baudratetable[] = {
    {H8_SMR_CLK4, 103, 13},     // /< 1200 baud
    {H8_SMR_CLK1, 207, 6},      // /< 2400 baud
    {H8_SMR_CLK1, 103, 3},      // /< 4800 baud
    {H8_SMR_CLK1, 51, 2},       // /< 9600 baud
    {H8_SMR_CLK1, 25, 1},       // /< 19200 baud
    {H8_SMR_CLK1, 12, 1},       // /< 38400 baud
};

/**
 * \brief The settings for the supplied character lenghts
 */
static const unsigned char databittable[] = {
    H8_SMR_CLEN7,               // /< characterlenght is 7 bits 
    H8_SMR_CLEN8                // /< characterlenght is 8 bits 
};

/**
 * \brief The settings for the supplied parity modes
 */
static const unsigned char paritytable[] = {
    H8_SMR_NOPAR,               // /< No parity
    H8_SMR_EVENPAR,             // /< Even parity
    H8_SMR_ODDPAR               // /< Odd parity
};

/**
 * \brief The settings for the supplied stopbits
 */
static const unsigned char stopbittable[] = {
    H8_SMR_1STOP,               // /< One stopbit added
    H8_SMR_2STOP                // /< Two stopbit added
};

/**
 * \brief The settings for the mode control
 */
static const unsigned char modetable[2] = {
    H8_SMR_ASYNC,               // /< Asynchronous mode
    H8_SMR_SYNC                 // /< Synchronous mode
};

/**
 * \brief The settings for wakeup mode
 */
static const unsigned char wakeuptable[2] = {
    H8_SMR_NO_MP,               // /< No wakeup mode
    H8_SMR_MPMODE               // /< Use wakeup mode
};

// @}

extern void
DEV_h8_sci_open (int port, const struct DEV_serial_settings *settings)
{
    sci_reg_t *const p = ports[port];

    p->SCR = 0x00;
    p->SMR =
        modetable[0] |
        baudratetable[settings->baud].smr |
        databittable[settings->data] |
        paritytable[settings->parity] | stopbittable[settings->stop] | wakeuptable[0];
    p->BRR = baudratetable[settings->baud].brr;

    // Wait for the baud rate generator to stabilize (the time to send one 
    // bit).
    DEV_cpudelay (baudratetable[settings->baud].delay * DEV_USEC_2_LOOPS (1000));

    p->SSR &= ~(H8_SSR_ORER | H8_SSR_FER | H8_SSR_PER | H8_SSR_RDRF);
    p->SCR |= H8_SCR_RE | H8_SCR_RIE | H8_SCR_TE;
}

extern void
DEV_h8_sci_close (int port)
{
    sci_reg_t *const p = ports[port];

    p->SCR = 0;
}

static void
tx_write (int port, char c)
{
    sci_reg_t *const p = ports[port];

    if (p->SSR & H8_SSR_TDRE)
    {
        p->TDR = c;
        p->SSR &= ~H8_SSR_TDRE;
    }
}

extern void
DEV_h8_sci_start (int port, DEV_serial_t * serial)
{
    int c;
    sci_reg_t *const p = ports[port];

    if ((c = serial->int_interface->tx_char (serial)) != ACE_EOF)
    {
        tx_write (port, (char)c);
        p->SCR |= H8_SCR_TIE | H8_SCR_TEIE;
    }
    else
    {
        serial->int_interface->tx_finished (serial);
    }
}

extern void
DEV_h8_sci_txe (int port, DEV_serial_t * serial)
{
    int c;
    sci_reg_t *const p = ports[port];

    if ((c = serial->int_interface->tx_char (serial)) == ACE_EOF)
    {
        p->SCR &= ~H8_SCR_TEIE;
        serial->int_interface->tx_finished (serial);
    }
    else
    {
        tx_write (port, (char)c);
        p->SCR |= H8_SCR_TIE;
    }
}

extern void
DEV_h8_sci_txi (int port, DEV_serial_t * serial)
{
    int c;
    sci_reg_t *const p = ports[port];

    if ((c = serial->int_interface->tx_char (serial)) == ACE_EOF)
    {
        p->SCR &= ~H8_SCR_TIE;
    }
    else
    {
        tx_write (port, (char)c);
    }
}

extern void
DEV_h8_sci_rxi (int port, DEV_serial_t * serial)
{
    char c;
    sci_reg_t *const p = ports[port];

    c = p->RDR;
    p->SSR &= ~H8_SSR_RDRF;
    serial->int_interface->rx_char (serial, c);
}

extern void
DEV_h8_sci_eri (int port, DEV_serial_t * serial)
{
    sci_reg_t *const p = ports[port];
    unsigned char const ssr = p->SSR;

    if (ssr & H8_SSR_ORER)
        serial->int_interface->rx_error (serial, DEV_SER_RX_OVERRUN);
    if (ssr & H8_SSR_FER)
        serial->int_interface->rx_error (serial, DEV_SER_RX_FRAMING);
    if (ssr & H8_SSR_PER)
        serial->int_interface->rx_error (serial, DEV_SER_RX_PARITY);
    p->SSR &= ~(H8_SSR_ORER | H8_SSR_FER | H8_SSR_PER);
}
