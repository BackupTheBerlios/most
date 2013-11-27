/*
 * interrupts.c
 *
 *  Created on: 26.11.2013
 *      Author: maik
 */

#include <ace/stddef.h>
#include <uso/cpu.h>
#include <arch/interrupts.h>

static void
eth_dummy_interrupt (DEV_smc91c94_t *smc)
{
    ;
}

static void (*eth_interrupt)(DEV_smc91c94_t *smc);
static DEV_smc91c94_t *emac;

extern ACE_INTERRUPT_ void
ACE_SECTION_ (".unref") MDC_IRQ7_ISR (void)
{
    eth_interrupt(emac);

}

extern void
MDC_interrupts_init(void)
{
    emac = NULL;
    eth_interrupt = eth_dummy_interrupt;
}

extern void
MDC_interrupts_set_eth(void (*f)(DEV_smc91c94_t *), DEV_smc91c94_t *smc)
{
    USO_cpu_status_t ps = USO_disable ();
    emac = smc;
    eth_interrupt = f;
    USO_restore (ps);
}
