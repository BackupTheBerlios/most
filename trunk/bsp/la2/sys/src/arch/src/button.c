/*
 * button.c
 *
 *  Created on: 18.05.2013
 *      Author: maik
 */

#include <uso/thread.h>
#include <uso/semaphore.h>
#include <uso/stack.h>
#include <uso/sleep.h>
#include <uso/log.h>
#include <dev/arch/at91sam7a2/csp.h>


#include <arch/digio.h>
#include <arch/button.h>
#include <arch/pwm.h>

#define BUTTON_STACK_SIZE      (0x400/sizeof(USO_stack_t))

static USO_thread_t button_thread;
static USO_stack_t button_stack[BUTTON_STACK_SIZE];
static USO_semaphore_t sig;

static ACE_err_t
button_run (void *nix)
{
    USO_log_puts (USO_LL_INFO, "Button is running.\n");
    for (;;)
    {
        USO_wait (&sig);
        USO_log_puts (USO_LL_INFO, "Button pressed.\n");
        LA2_beep();
    }
    return DEF_ERR_SYS;
}

static void
button_interrupt(void)
{
    CSP_GIC_SET_EOICR(GIC, 0);
    USO_signal(&sig);
}

extern void
LA2_button_start(void)
{
    USO_semaphore_init (&sig, 0);

    CSP_GIC_SET_ICCR(GIC, EXTIRQ1);
    CSP_GICConfigInterrupt(EXTIRQ1, (NEGATIVE_EDGE_TRIGGERED|PRIOR_5), (U32_T)button_interrupt);

    USO_log_puts (USO_LL_INFO, "Button start.\n");
    USO_thread_init (&button_thread,
                     button_run,
                     button_stack, ACE_ARRAYSIZE (button_stack), USO_INTERRUPT, USO_FIFO, "button");
    USO_start (&button_thread);
}
