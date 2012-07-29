/*
 * time.c
 *
 *  Created on: 23.12.2010
 *      Author: maik
 */

#include "ace/time.h"
#include "ace/stdio.h"

#define BASE_YEAR   2000
#define EPOCH_YEARS 100

#define SEC_IN_MIN        60
#define MINUTE_IN_HOUR    60
#define HOUR_IN_DAY       24
#define MONTH_IN_YEAR     12

#define SEC_IN_HOUR       ((unsigned long)(MINUTE_IN_HOUR * SEC_IN_MIN))
#define SEC_IN_DAY        ((unsigned long)(HOUR_IN_DAY * SEC_IN_HOUR))

static const int days_in_month[2][MONTH_IN_YEAR] = {
    {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
    {31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}
};

static const int days_before_month[2][MONTH_IN_YEAR] = {
    {0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334},
    {0, 31, 60, 91, 121, 152, 182, 213, 244, 274, 305, 335}
};

static const int days_in_year[2] = { 365, 366 };

/* Ein Jahr hat 365,25 Tage, daher ist jedes 4 Jahr ein Schaltjahr,
 * ausser es ist ein Jahrhundert Jahr, da is auch nur jedes 4 Jahundert Jahr ein
 * Schaltjahr.
 */
static int
is_leap (unsigned int year)
{
    return year % 4 == 0 && (year % 100 != 0 || year % 400 == 0);
}

static ACE_err_t
time_valid_param (ACE_time_t * time)
{
    if (time->year < BASE_YEAR || time->year > BASE_YEAR + EPOCH_YEARS)
        return ACE_ERR_YEAR;
    if (time->month < 1 || time->month > 12)
        return ACE_ERR_MONTH;
    if (time->day < 1 || time->day > days_in_month[is_leap (time->year)][time->month - 1])
        return ACE_ERR_DAY;
    if (time->hour > 23)
        return ACE_ERR_HOUR;
    if (time->min > 59)
        return ACE_ERR_MIN;
    if (time->sec > 59)
        return ACE_ERR_SEC;
    return ACE_ERR_OK;
}

extern ACE_err_t
ACE_time_from_param (ACE_time_t * time)
{
    ACE_err_t err = time_valid_param (time);
    if (err != ACE_ERR_OK)
        return err;

    unsigned long t;

    /* compute hour, min, sec */
    t = time->sec + (time->min * SEC_IN_MIN) + (time->hour * SEC_IN_HOUR);

    /* compute days in year */
    unsigned long days;
    days = time->day - 1;
    days += days_before_month[is_leap (time->year)][time->month - 1];

    /* compute days in other years */
    if (time->year > BASE_YEAR)
    {
        unsigned int year;
        for (year = BASE_YEAR; year < time->year; year++)
            days += days_in_year[is_leap (year)];
    }

    /* compute total seconds */
    t += (days * SEC_IN_DAY);
    time->time = t;
    return ACE_ERR_OK;
}

extern ACE_err_t
ACE_time_to_param (ACE_time_t * time)
{
    unsigned long days;
    unsigned long rem_sec;
    days = time->time / SEC_IN_DAY;
    rem_sec = time->time % SEC_IN_DAY;

    /* compute hour, min, and sec */
    time->hour = (unsigned int)(rem_sec / SEC_IN_HOUR);
    rem_sec %= SEC_IN_HOUR;
    time->min = (unsigned int)(rem_sec / SEC_IN_MIN);
    time->sec = (unsigned int)(rem_sec % SEC_IN_MIN);

    /* compute year */
    unsigned int year = BASE_YEAR;
    int leap;
    for (;;)
    {
        leap = is_leap (year);
        if (days < days_in_year[leap])
            break;
        year++;
        days -= days_in_year[leap];
    }
    time->year = year;

    /* compute month and day */
    const int *days_per_month;
    days_per_month = days_in_month[leap];
    unsigned int month;
    for (month = 0; days >= days_per_month[month]; ++month)
    {
        days -= days_per_month[month];
    }
    time->month = month + 1;
    time->day = days + 1;

    return time_valid_param (time);
}

extern ACE_err_t
ACE_time_print (ACE_time_t * time)
{
    ACE_err_t err = time_valid_param (time);
    if (err != ACE_ERR_OK)
        return err;

    ACE_printf ("%04d.%02d.%02d - %02d:%02d:%02d",
                time->year, time->month, time->day, time->hour, time->min, time->sec);

    return ACE_ERR_OK;
}
