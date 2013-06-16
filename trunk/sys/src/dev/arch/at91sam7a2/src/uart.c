#include <ace/stddef.h>
#include <ace/stdio.h>
#include <dev/arch/at91sam7a2/usart.h>
#include <dev/arch/at91sam7a2/uart.h>
#include <dev/arch/at91sam7a2/csp.h>

static CSP_USART_T *const ports[] = {
		((CSP_USART_T*) USART0_BASE_ADDRESS),
		((CSP_USART_T*) USART1_BASE_ADDRESS)
};

/* LA2_SYSTEM_CLOCK / (16 * baudrate) */
static unsigned int const baudratetable[] = {
    1563,                       // /< 1200 baud
    781,                        // /< 2400 baud
    391,                        // /< 4800 baud
    195,                        // /< 9600 baud
    98,                         // /< 19200 baud
    49                          // /< 38400 baud
};

static const unsigned int paritytable[] = {
	PAR_NO,          // /< No parity
	PAR_EVEN,          // /< Even parity
	PAR_ODD            // /< Odd parity
};

static const unsigned int databittable[] = {
		CHRL_7,       // /< characterlenght is 7 bits
		CHRL_8        // /< characterlenght is 8 bits
};

static const unsigned int stopbittable[] = {
		NBSTOP_1,      // /< One stopbit added
		NBSTOP_2       // /< Two stopbit added
};

extern void
AT91SAM7A2_uart_init (int port)
{
	CSP_USART_T *const uart = ports[port];

    /* Enable USART Clock */
	CSP_USART_SET_ECR(uart, (USART | PIO));

	/* USART Software Reset */
	CSP_USART_SET_CR(uart, (SWRST | RSTRX | RSTTX | RSTSTA));

	/* Disable USART PIO */
	CSP_USART_SET_PDR(uart, (RXD | TXD | SCK));
}

extern void
AT91SAM7A2_uart_open (int port, const struct DEV_serial_settings *settings)
{

	CSP_USART_T *const uart = ports[port];

	/* Set Mode */
	CSP_USART_SET_MR(uart, USCLKS_MCKI | ASYNC |
		paritytable[settings->parity] |
		databittable[settings->data] |
		stopbittable[settings->stop] );

	/* Set Baudrate */
	CSP_USART_SET_BRGR(uart, baudratetable[settings->baud]);

	/* Set Receiver Time Out */
	CSP_USART_SET_RTOR(uart, TO_DISABLE);

	/* Set Transmitter Timer Guard */
	CSP_USART_SET_TTGR(uart, TG_DISABLE);

	/* USART Rx and/or Tx Enable */
	CSP_USART_SET_CR(uart, RXEN | TXEN);

   /* Disable all interrupt */
   CSP_USART_SET_IDR(uart, 0xFFFFFFFF);

   /* Interrupt Enable */
   CSP_USART_SET_IER(uart, RXRDY | USOVRE | FRAME | PARE );

}

extern void
AT91SAM7A2_uart_close (int port)
{
	CSP_USART_T *const uart = ports[port];

	/* USART Rx and/or Tx Disable */
	CSP_USART_SET_CR(uart, RXDIS | TXDIS);

	/* Disable all interrupt */
	CSP_USART_SET_IDR(uart, 0xFFFFFFFF);

}


static void
tx_write (int port, char c)
{
	CSP_USART_T *const uart = ports[port];

	/* THR : USART Transmit Holding Register */
	CSP_USART_SET_THR(uart, c);
}

extern void
AT91SAM7A2_uart_start (int port, DEV_serial_t * serial)
{
    int c;
	CSP_USART_T *const uart = ports[port];

    if ((c = serial->int_interface->tx_char (serial)) != ACE_EOF)
    {
    	/* Interrupt Enable */
    	CSP_USART_SET_IER(uart, TXRDY | TXEMPTY);

    	/* USART Tx Enable */
    	CSP_USART_SET_CR(uart, TXEN);

        tx_write (port, (char)c);
    }
    else
    {
        serial->int_interface->tx_finished (serial);
    }
}

extern void
AT91SAM7A2_uart_interrupt (int port, DEV_serial_t * serial)
{
	CSP_USART_T *const uart = ports[port];

    if ((CSP_USART_GET_SR(uart) & RXRDY) == RXRDY)
    {
        char c;

        c = CSP_USART_GET_RHR(uart);
        serial->int_interface->rx_char (serial, c);
    }
    else
    {
        if ((CSP_USART_GET_SR(uart) & USOVRE) == USOVRE)
        {
            serial->int_interface->rx_error (serial, DEV_SER_RX_OVERRUN);
        	CSP_USART_SET_CR(uart, RSTSTA);
        }
        else if ((CSP_USART_GET_SR(uart) & FRAME) == FRAME)
        {
            serial->int_interface->rx_error (serial, DEV_SER_RX_FRAMING);
        	CSP_USART_SET_CR(uart, RSTSTA);
        }
        else if ((CSP_USART_GET_SR(uart) & PARE) == PARE)
        {
            serial->int_interface->rx_error (serial, DEV_SER_RX_PARITY);
        	CSP_USART_SET_CR(uart, RSTSTA);
        }
    }

    if ((CSP_USART_GET_SR(uart) & TXEMPTY) == TXEMPTY)
    {
        int c;

        if ((c = serial->int_interface->tx_char (serial)) == ACE_EOF)
        {
        	CSP_USART_SET_CR(uart, TXDIS);
        	CSP_USART_SET_IDR(uart, TXEMPTY);
            serial->int_interface->tx_finished (serial);
        }
        else
        {
        	CSP_USART_SET_IER(uart, TXRDY);
            tx_write (port, (char)c);
        }
    }
    else if ((CSP_USART_GET_SR(uart) & TXRDY) == TXRDY)
    {
        int c;

        if ((c = serial->int_interface->tx_char (serial)) == ACE_EOF)
        {
        	CSP_USART_SET_IDR(uart, TXRDY);
        }
        else
        {
            tx_write (port, (char)c);
        }
    }
}
