#include <dev/arch/at91/aic.h>
#include <dev/arch/at91/debug.h>
#include "dev/arch/at91/AT91SAM7X256.h"

#include "arch/exceptions.h"
#include "arch/ticks.h"
#include "arch/uart.h"
#include "arch/eth.h"
#include "arch/digio.h"


extern void
SAM_undefined_instruction(void)
{
    DEV_digout_set (&SAM_red_led);
	DEV_at91_dbgu_print_ascii("-F- Undefined instruction!\n");
    while(1);
}

extern void
SAM_prefetch_abort(void)
{
    DEV_digout_set (&SAM_red_led);
	DEV_at91_dbgu_print_ascii("-F- Prefetch abort at ");
	DEV_at91_dbgu_print_hex8(AT91C_BASE_MC->MC_AASR);
	DEV_at91_dbgu_print_ascii("!\n");
    while(1);
}

extern void
SAM_data_abort(void)
{
    DEV_digout_set (&SAM_red_led);
	DEV_at91_dbgu_print_ascii("-F- Data abort at ");
	DEV_at91_dbgu_print_hex8(AT91C_BASE_MC->MC_AASR);
	DEV_at91_dbgu_print_ascii("\n");
    while(1);
}


extern void
SAM_default_spurious_handler(void)
{
    DEV_digout_set (&SAM_red_led);
    DEV_at91_dbgu_print_ascii("-F- Spurious Interrupt\n");
    while(1);
}

extern void
SAM_default_irq_handler(void)
{
    DEV_digout_set (&SAM_red_led);
    DEV_at91_dbgu_print_ascii("-F- Default IRQ\n");
    while(1);
}

extern void
SAM_default_fiq_handler(void)
{
    DEV_digout_set (&SAM_red_led);
    DEV_at91_dbgu_print_ascii("-F- Default FIQ\n");
    while(1);
}

extern void
SAM_sys_interrupt_init(void)
{
	DEV_at91_aic_configure_IT(AT91C_ID_SYS, (AT91C_AIC_SRCTYPE_INT_HIGH_LEVEL | 0x5 ), SAM_ticks_interrupt);
	DEV_at91_aic_configure_IT(AT91C_ID_US0, (AT91C_AIC_SRCTYPE_INT_HIGH_LEVEL | 0x6 ), SAM_uart_interrupt_0);
	DEV_at91_aic_configure_IT(AT91C_ID_US1, (AT91C_AIC_SRCTYPE_INT_HIGH_LEVEL | 0x6 ), SAM_uart_interrupt_1);
	DEV_at91_aic_configure_IT(AT91C_ID_EMAC, (AT91C_AIC_SRCTYPE_INT_HIGH_LEVEL | 0x3 ), SAM_eth_interrupt);
	DEV_at91_aic_enable_IT(AT91C_ID_SYS);
	DEV_at91_aic_enable_IT(AT91C_ID_US0);
	DEV_at91_aic_enable_IT(AT91C_ID_US1);
	DEV_at91_aic_enable_IT(AT91C_ID_EMAC);
}
