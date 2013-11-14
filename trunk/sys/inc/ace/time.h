/*
 * time.h
 *
 *  Created on: 23.12.2010
 *      Author: maik
 */

#ifndef ACE_TIME_H_
#define ACE_TIME_H_

#include "ace/err.h"

/** @addtogroup ace
 *
 * @{
 */

/** @defgroup time time.h
 *
 * @{
 */

/** Time type structure.
 *
 */
struct ACE_time
{
    unsigned long time;         /**< in seconds */
    unsigned int year;          /**< 2000 - 2100 */
    unsigned int month;         /**< 1 - 12 */
    unsigned int day;           /**< 1 - 31 */
    unsigned int hour;          /**< 0 - 23 */
    unsigned int min;           /**< 0 - 59 */
    unsigned int sec;           /**< 0 - 59 */
};

/** Time type.
 *
 */
typedef struct ACE_time ACE_time_t;

/**
 * Calculate time in time type from the parameters in time type.
 *
 * @param time : pointer to time type.
 *
 * @return error.
 */
extern ACE_err_t ACE_time_from_param (ACE_time_t * time);

/**
 * Calculate parameters in time type from the time in time type.
 *
 * @param time : pointer to time type.
 *
 * @return error.
 */
extern ACE_err_t ACE_time_to_param (ACE_time_t * time);

/**
 * Print time.
 *
 * @param time : pointer to time type.
 *
 * @return error.
 */
extern ACE_err_t ACE_time_print (ACE_time_t * time, char *buf);

/** @}
 */

/** @}
 */


#endif /* ACE_TIME_H_ */
