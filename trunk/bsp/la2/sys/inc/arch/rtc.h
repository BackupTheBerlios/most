/*
 * rtc.h
 *
 *  Created on: 21.12.2010
 *      Author: maik
 */

#ifndef LA2_RTC_H_
#define LA2_RTC_H_

#include <ace/stddef.h>
#include <dev/rtc.h>

/** @addtogroup arch
 *
 * @{
 */

/** @defgroup rtc rtc.h
 *
 * Real time clock.
 *
 * @{
 */

/**
 *
 */
extern struct DEV_rtt_interface *rtt;

/**
 *
 */
extern void LA2_rtc_init (void);

/**
 *
 */
extern void LA2_rtc_interrupt (void) ACE_SECTION_ (".unref");

/** @}
 */

/** @}
 */

#endif /* LA2_RTC_H_ */
