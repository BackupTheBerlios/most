/*
 * rtc.h
 *
 *  Created on: 17.12.2010
 *      Author: maik
 */

#ifndef DEV_RTC_H_
#define DEV_RTC_H_

/** @addtogroup dev
 * @{
 */

/** @defgroup rtc rtc.h
 *
 * @{
 */

/**
 *
 */
struct DEV_rtt_interface
{
    void (*reset) (void);
    unsigned long (*get_offset) (void);
    void (*set_alarm) (unsigned long);
};

/**
 *
 */
extern void DEV_rtc_init (struct DEV_rtt_interface *rtt);

/**
 *
 */
extern unsigned long DEV_time_get (void);

/**
 *
 */
extern void DEV_time_set (unsigned long time);

/**
 *
 */
extern void DEV_alarm_set (unsigned long time);


/** @}
 */

/** @}
 */

#endif /* DEV_RTC_H_ */
