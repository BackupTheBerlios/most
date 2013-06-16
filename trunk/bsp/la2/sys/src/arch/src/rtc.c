/*
 * rtc.c
 *
 *  Created on: 21.12.2010
 *      Author: maik
 */

#include <ace/stddef.h>
#include <dev/arch/at91sam7a2/csp.h>

#include <arch/cpu.h>
#include <arch/rtc.h>

static struct DEV_rtt_interface rtt_if;
struct DEV_rtt_interface *rtt = NULL;

static void
reset (void)
{
	CSP_WTSetTime(WT, 0);
}

static unsigned long
get_offset (void)
{
    return CSP_WTGetTime(WT) / LA2_EXT_CLOCK;
}

static void
set_alarm (unsigned long time)
{
	CSP_WTSetAlarm(WT, time * LA2_EXT_CLOCK);
}

extern void
LA2_rtc_init (void)
{
	CSP_WTInit(WT, SECRST_A8B);
	rtt_if.reset = reset;
    rtt_if.get_offset = get_offset;
    rtt_if.set_alarm = set_alarm;
    rtt = &rtt_if;
    DEV_rtc_init (rtt);
    CSP_WTEnable(WT, SECSEN);
}

extern void
LA2_rtc_interrupt (void)
{

}
