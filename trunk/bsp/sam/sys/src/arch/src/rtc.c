/*
 * rtc.c
 *
 *  Created on: 21.12.2010
 *      Author: maik
 */

#include "arch/rtc.h"
#include <dev/arch/at91sam7x/rtt.h>
#include <ace/stddef.h>

static struct DEV_rtt_interface rtt_if;
struct DEV_rtt_interface *rtt = NULL;

static void
reset (void)
{
    DEV_at91_RTT_set_prescaler (AT91C_BASE_RTTC, DEV_AT91_RTT_PRESCALER_1SEC);
}

static unsigned long
get_offset (void)
{
    return DEV_at91_RTT_get_time (AT91C_BASE_RTTC);
}

static void
set_alarm (unsigned long time)
{
    DEV_at91_RTT_set_alarm (AT91C_BASE_RTTC, time);
}

extern void
SAM_rtc_init (void)
{
    rtt_if.reset = reset;
    rtt_if.get_offset = get_offset;
    rtt_if.set_alarm = set_alarm;
    rtt = &rtt_if;
    DEV_rtc_init (rtt);
}

extern void
SAM_rtc_interrupt (void)
{

}
