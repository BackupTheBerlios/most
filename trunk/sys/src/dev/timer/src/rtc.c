/*
 * rtc.c
 *
 *  Created on: 17.12.2010
 *      Author: maik
 */

#include <ace/stdio.h>
#include <ace/time.h>
#include <ace/err.h>
#include <mfs/sysfs.h>
#include <mfs/vfs.h>
#include <mfs/descriptor.h>
#include <mfs/directory.h>

#include "dev/rtc.h"

static struct DEV_rtc
{
    unsigned long base_time;    /* in seconds since BASE_YEAR */
    struct DEV_rtt_interface *rtt;
} rtc;


static void
info (MFS_descriptor_t * desc)
{
    static ACE_time_t time;
    time.time = DEV_time_get ();
    ACE_err_t err = ACE_time_to_param (&time);
    if (err != ACE_OK)
    {
        ACE_printf ("Inval time: %d\n", err);
        return;
    }
    ACE_puts ("Date: ");
    ACE_time_print (&time);
    ACE_puts ("\n");
}

static struct MFS_descriptor_op rtc_descriptor_op = {
	.open = NULL,
    .close = NULL,
    .info = info,
    .control = NULL
};

extern void
DEV_rtc_init (struct DEV_rtt_interface *rtt)
{
    rtc.base_time = 0;
    rtc.rtt = rtt;
    MFS_descriptor_create (MFS_resolve(MFS_get_root(), "sys/dev/clock"), "rtc",
                     MFS_SYS, &rtc_descriptor_op, (MFS_represent_t *) &rtc);
    rtc.rtt->reset ();
}

extern unsigned long
DEV_time_get (void)
{
    return rtc.base_time + rtc.rtt->get_offset ();
}


extern void
DEV_time_set (unsigned long time)
{
    rtc.base_time = time;
    rtc.rtt->reset ();
}

extern void
DEV_alarm_set (unsigned long time)
{
    if (time > (rtc.base_time + rtc.rtt->get_offset ()))
    {
        rtc.rtt->set_alarm (rtc.base_time - time);
    }
    else
    {
        /* error alarm is in the past */
    }
}

