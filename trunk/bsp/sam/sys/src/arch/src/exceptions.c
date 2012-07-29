#include <dev/arch/at91/aic.h>
#include <dev/arch/at91/debug.h>
#include <dev/arch/at91/pit.h>
#include <dev/arch/at91/rtt.h>
#include <dev/arch/at91/AT91SAM7X256.h>
#include <dev/cpu.h>

#include "arch/exceptions.h"
#include "arch/ticks.h"
#include "arch/uart.h"
#include "arch/pwm.h"
#include "arch/eth.h"
#include "arch/digio.h"
#include "arch/rtc.h"


#define PAUSE 1000000           /* 1 s = 1000 0000 us */
#define PULSE 400               /* 400 ms = 2.5  p/s */

static void
blink (int c)
{
    while (1)
    {
        DEV_blink_nb (&SAM_red_led, c, PULSE);
        DEV_cpudelay (PAUSE);
    }
}

extern void
SAM_undefined_instruction (void)
{
    DEV_at91_DBGU_print_ascii ("Undefined instruction!\n");
    blink (2);
}

extern void
SAM_prefetch_abort (void)
{
    DEV_at91_DBGU_print_ascii ("Prefetch abort at ");
    DEV_at91_DBGU_print_hex8 (AT91C_BASE_MC->MC_AASR);
    DEV_at91_DBGU_print_ascii ("!\n");
    blink (3);
}

extern void
SAM_data_abort (void)
{
    DEV_at91_DBGU_print_ascii ("Data abort at ");
    DEV_at91_DBGU_print_hex8 (AT91C_BASE_MC->MC_AASR);
    DEV_at91_DBGU_print_ascii ("\n");
    blink (4);
}


extern void
SAM_default_spurious_handler (void)
{
    DEV_at91_DBGU_print_ascii ("Spurious Interrupt\n");
    blink (5);
}

extern void
SAM_default_irq_handler (void)
{
    DEV_at91_DBGU_print_ascii ("Default IRQ\n");
    blink (6);
}

extern void
SAM_default_fiq_handler (void)
{
    DEV_at91_DBGU_print_ascii ("Default FIQ\n");
    blink (1);
}


static void
sys_interrupt (void)
{
    AT91C_BASE_AIC->AIC_ICCR = (1 << AT91C_ID_SYS);
    if (DEV_at91_PIT_get_status () == 1)
    {
        SAM_ticks_interrupt ();
    }
    unsigned int v = DEV_at91_RTT_get_status (AT91C_BASE_RTTC); /*reset alarm, increment */
    if (v == 1 || v == 2)
    {
        SAM_rtc_interrupt ();
    }
}

extern void
SAM_sys_interrupt_init (void)
{
    DEV_at91_AIC_configure_IT (AT91C_ID_SYS, (AT91C_AIC_SRCTYPE_INT_HIGH_LEVEL | 0x3),
                               sys_interrupt);
    DEV_at91_AIC_configure_IT (AT91C_ID_US0, (AT91C_AIC_SRCTYPE_INT_HIGH_LEVEL | 0x4),
                               SAM_uart_interrupt_0);
    DEV_at91_AIC_configure_IT (AT91C_ID_US1, (AT91C_AIC_SRCTYPE_INT_HIGH_LEVEL | 0x4),
                               SAM_uart_interrupt_1);
    DEV_at91_AIC_configure_IT (AT91C_ID_PWMC, (AT91C_AIC_SRCTYPE_INT_HIGH_LEVEL | 0x6),
                               SAM_pwm_interrupt);
    DEV_at91_AIC_enable_IT (AT91C_ID_SYS);
    DEV_at91_AIC_enable_IT (AT91C_ID_US0);
    DEV_at91_AIC_enable_IT (AT91C_ID_US1);
    DEV_at91_AIC_enable_IT (AT91C_ID_PWMC);
}


extern void
SAM_emac_interrupt_init (void)
{
    DEV_at91_AIC_configure_IT (AT91C_ID_EMAC, (AT91C_AIC_SRCTYPE_INT_HIGH_LEVEL | 0x1),
                               SAM_eth_interrupt);
    DEV_at91_AIC_enable_IT (AT91C_ID_EMAC);
}
