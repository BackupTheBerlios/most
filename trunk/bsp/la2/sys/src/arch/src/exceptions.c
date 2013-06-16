#include <ace/stddef.h>
#include <dev/cpu.h>
#include <dev/arch/at91sam7a2/csp.h>

#include <arch/digio.h>
#include <arch/exceptions.h>


#define BLINK_PAUSE 2000000           /* 2 s */
#define BLINK_PULSE 500               /* 2  p/s */

static void blink (int c) ACE_SECTION_ (".unref");

static void
blink (int c)
{
    while (1)
    {
        DEV_blink_nb (&LA2_red_led, c, BLINK_PULSE);
        DEV_cpudelay (DEV_USEC_2_LOOPS(BLINK_PAUSE));
    }
}

extern void
LA2_default_reset_handler (void)
{
    blink (1);
}

extern void
LA2_undefined_instruction (void)
{
    blink (2);
}

extern void
LA2_software_interrupt (void)
{
    blink (3);
}

extern void
LA2_prefetch_abort (void)
{
    blink (4);
}

extern void
LA2_data_abort (void)
{
    blink (5);
}

extern void
LA2_default_irq_handler (void)
{
    blink (7);
}

extern void
LA2_default_fiq_handler (void)
{
    blink (8);
}

extern void
LA2_default_spurious_handler (void)
{
    blink (10);
}
