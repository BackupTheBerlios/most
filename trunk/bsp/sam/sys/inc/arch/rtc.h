/*
 * rtc.h
 *
 *  Created on: 21.12.2010
 *      Author: maik
 */

#ifndef SAM_RTC_H_
#define SAM_RTC_H_

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
extern void SAM_rtc_init (void);

/**
 *
 */
extern void SAM_rtc_interrupt (void);

/** @}
 */

/** @}
 */

#endif /* SAM_RTC_H_ */
