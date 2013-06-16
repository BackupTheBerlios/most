#include <ace/stddef.h>
#include <ace/stdio.h>
#include "dev/arch/at91sam7x/uart.h"
#include "dev/arch/at91sam7x/AT91SAM7X256.h"

// *********************************************************************************************************
// Set up USART0 (peripheral ID = 6) 9600 baud, 8 data bits, 1 stop bit, no parity
//
// We will use the onboard baud rate generator to specify 9600 baud
//
// The Olimex SAM7-EX256 board has a 18,432,000 hz crystal oscillator.
//
// MAINCK = 18432000 hz (from Olimex schematic)
// DIV = 14 (set up in lowlevelinit.c)
// MUL = 72 (set up in lowlevelinit.c)
//
// PLLCK = (MAINCK / DIV) * (MUL + 1) = 18432000/14 * (72 + 1)
// PLLCLK = 1316571 * 73 = 96109683 hz
// MCK = PLLCLK / 2 = 96109683 / 2 = 48054841 hz
//
// Baud Rate (asynchronous mode) = MCK / (8(2 - OVER)CD)
//
// MCK = 48054841 hz (set USCLKS = 00 in USART Mode Register US_MR - to select MCK only)
// VER = 0 (bit 19 of the USART Mode Register US_MR)
// CD = divisor (USART Baud Rate Generator Register US_BRGR)
// baudrate = 9600 (desired)
//
//                                 4805484        48054841
// a little algebra: BaudRate = -------------- = -----------
//                               (8(2 - 0)CD)      16(CD)
//
//       48054841     48054841
// CD = ---------- = ---------- = 312.857037
//       9600(16)      153600
//
// CD = 313 (round up)
//
//                                           48054841         48054841
// check the actual baud rate: BaudRate = --------------- = ------------ = 9595.6
//                                         (8 (2 - 0)313        5008
//
//                                desired baudrate          9600
// what's the error: Error = 1 - ------------------ = 1 - --------- = 1 - 1.00045854 = -.0004585
//                                actual baudrate          9595.6                      (not much)
//
// This calculation is taken from : James P Lynch June 22, 2008
// *********************************************************************************************************

static AT91PS_USART const ports[] = {
    AT91C_BASE_US0,
    AT91C_BASE_US1
};

static unsigned int const baudratetable[] = {
    2503,                       // /< 1200 baud
    1251,                       // /< 2400 baud
    626,                        // /< 4800 baud
    313,                        // /< 9600 baud
    156,                        // /< 19200 baud
    78                          // /< 38400 baud
};

static const unsigned int paritytable[] = {
    AT91C_US_PAR_NONE,          // /< No parity
    AT91C_US_PAR_EVEN,          // /< Even parity
    AT91C_US_PAR_ODD            // /< Odd parity
};

static const unsigned int databittable[] = {
    AT91C_US_CHRL_7_BITS,       // /< characterlenght is 7 bits 
    AT91C_US_CHRL_8_BITS        // /< characterlenght is 8 bits 
};

static const unsigned int stopbittable[] = {
    AT91C_US_NBSTOP_1_BIT,      // /< One stopbit added
    AT91C_US_NBSTOP_2_BIT       // /< Two stopbit added
};


extern void
DEV_at91_uart0_init (void)
{
    // enable the usart0 peripheral clock
    volatile AT91PS_PMC pmc = AT91C_BASE_PMC;   // pointer to PMC data structure
    pmc->PMC_PCER = (1 << AT91C_ID_US0);        // enable usart0 peripheral clock

    // set up PIO to enable USART0 peripheral control of pins
    volatile AT91PS_PIO pio = AT91C_BASE_PIOA;
    pio->PIO_PDR = AT91C_PA0_RXD0 | AT91C_PA1_TXD0;     // enable peripheral control of PA0,PA1 (RXD0 and TXD0)
    pio->PIO_ASR = AT91C_PIO_PA0 | AT91C_PIO_PA1;       // assigns the 2 I/O lines to peripheral A function
    pio->PIO_BSR = 0;           // peripheral B function set to "no effect"
}

extern void
DEV_at91_uart1_init (void)
{
    // enable the usart0 peripheral clock
    volatile AT91PS_PMC pmc = AT91C_BASE_PMC;   // pointer to PMC data structure
    pmc->PMC_PCER = (1 << AT91C_ID_US1);        // enable usart1 peripheral clock

    // set up PIO to enable USART0 peripheral control of pins
    volatile AT91PS_PIO pio = AT91C_BASE_PIOA;
    pio->PIO_PDR = AT91C_PA5_RXD1 | AT91C_PA6_TXD1;     // enable peripheral control of PA5,PA6 (RXD1 and TXD1)
    pio->PIO_ASR = AT91C_PIO_PA5 | AT91C_PIO_PA6;       // assigns the 2 I/O lines to peripheral A function
    pio->PIO_BSR = 0;           // peripheral B function set to "no effect"
}

extern void
DEV_at91_uart_open (int port, const struct DEV_serial_settings *settings)
{
    // set up the USART0 registers
    volatile AT91PS_USART uart = ports[port];
    uart->US_MR = paritytable[settings->parity] |
        databittable[settings->data] | stopbittable[settings->stop];

    uart->US_IER = 0x00;        // no usart0 interrupts enabled (no effect)
    uart->US_IDR = 0xFFFF;      // disable all USART0 interrupts

    uart->US_BRGR = baudratetable[settings->baud];

    uart->US_RTOR = 0;          // receiver time-out (disabled)
    uart->US_TTGR = 0;          // transmitter timeguard (disabled)

    // enable the USART0 receiver and transmitter
    uart->US_CR = AT91C_US_RXEN;

    // enable the USART0 receive interrupt
    uart->US_IDR = ~0;          // disable all interrupts
    uart->US_IER = AT91C_US_RXRDY | AT91C_US_OVRE | AT91C_US_FRAME | AT91C_US_PARE;
}

extern void
DEV_at91_uart_close (int port)
{
    volatile AT91PS_USART uart = ports[port];
    uart->US_IDR = ~0;          // disable all interrupts
}


static void
tx_write (int port, char c)
{
    volatile AT91PS_USART uart = ports[port];
    uart->US_THR = c;
}

extern void
DEV_at91_uart_start (int port, DEV_serial_t * serial)
{
    int c;
    volatile AT91PS_USART uart = ports[port];

    if ((c = serial->int_interface->tx_char (serial)) != ACE_EOF)
    {
        uart->US_IER = AT91C_US_TXRDY | AT91C_US_TXEMPTY;
        uart->US_CR = AT91C_US_TXEN;
        tx_write (port, (char)c);
    }
    else
    {
        serial->int_interface->tx_finished (serial);
    }
}

extern void
DEV_at91_uart_interrupt (int port, DEV_serial_t * serial)
{
    volatile AT91PS_USART uart = ports[port];

    if ((uart->US_CSR & AT91C_US_RXRDY) == AT91C_US_RXRDY)
    {
        char c;

        c = uart->US_RHR;
        serial->int_interface->rx_char (serial, c);
    }
    else
    {
        if ((uart->US_CSR & AT91C_US_OVRE) == AT91C_US_OVRE)
        {
            serial->int_interface->rx_error (serial, DEV_SER_RX_OVERRUN);
            uart->US_CR = AT91C_US_RSTSTA;
        }
        else if ((uart->US_CSR & AT91C_US_FRAME) == AT91C_US_FRAME)
        {
            serial->int_interface->rx_error (serial, DEV_SER_RX_FRAMING);
            uart->US_CR = AT91C_US_RSTSTA;
        }
        else if ((uart->US_CSR & AT91C_US_PARE) == AT91C_US_PARE)
        {
            serial->int_interface->rx_error (serial, DEV_SER_RX_PARITY);
            uart->US_CR = AT91C_US_RSTSTA;
        }
    }

    if ((uart->US_CSR & AT91C_US_TXEMPTY) == AT91C_US_TXEMPTY)
    {
        int c;

        if ((c = serial->int_interface->tx_char (serial)) == ACE_EOF)
        {
            uart->US_CR = AT91C_US_TXDIS;
            uart->US_IDR = AT91C_US_TXEMPTY;
            serial->int_interface->tx_finished (serial);
        }
        else
        {
            uart->US_IER = AT91C_US_TXRDY;
            tx_write (port, (char)c);
        }
    }
    else if ((uart->US_CSR & AT91C_US_TXRDY) == AT91C_US_TXRDY)
    {
        int c;

        if ((c = serial->int_interface->tx_char (serial)) == ACE_EOF)
        {
            uart->US_IDR = AT91C_US_TXRDY;
        }
        else
        {
            tx_write (port, (char)c);
        }
    }
}
