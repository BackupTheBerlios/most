#ifndef NAP_SYSLOG_H
#define NAP_SYSLOG_H

#include <net/ip_addr.h>

/** @addtogroup nap
 * @{
 */

/** @defgroup syslog syslog.h
 *
 * Syslog client.
 *
 * @{
 */

enum NAP_syslog_level
{
    NAP_SYSLOG_EMERGENCY = 0,        /**< System is unusable */
    NAP_SYSLOG_ERROR = 3,            /**< Error conditions */
    NAP_SYSLOG_WARNING = 4,          /**< Warning conditions */
    NAP_SYSLOG_NOTICE = 5,           /**< Normal but significant conditions */
    NAP_SYSLOG_INFO = 6,             /**< Informational conditions */
    NAP_SYSLOG_DEBUG = 7             /**< debug messages */
};

enum NAP_syslog_facility
{
    NAP_SYSLOG_LOCAL0 = 16,          /**< MOST */
    NAP_SYSLOG_LOCAL1 = 17,          /**< Application 1 */
    NAP_SYSLOG_LOCAL2 = 18,          /**< Application 2 */
    NAP_SYSLOG_LOCAL3 = 19,          /**< Application 3 */
    NAP_SYSLOG_LOCAL4 = 20,          /**< Application 4 */
    NAP_SYSLOG_LOCAL5 = 21,          /**< Application 5 */
    NAP_SYSLOG_LOCAL6 = 22,          /**< Application 6 */
    NAP_SYSLOG_LOCAL7 = 23           /**< Application 7 */
};

/**
 * Open a SYSLOG connection.
 * @param client_addr : Client IP address.
 * @param server_addr : Server IP address.
 * @return -1 if fails, else 0.
 */
int NAP_syslog_open (NET_ip_addr_t * client_addr, NET_ip_addr_t * server_addr);

/**
 * Send logmessage to server.
 * @param level
 * @param facility
 * @param msg
 * @return -1 if fails, else 0.
 */
int NAP_syslog_puts (enum NAP_syslog_level level, enum NAP_syslog_facility facility, char *msg);

/**
 * Send logmessage to server.
 * @param level
 * @param facility
 * @param fmt
 * @return -1 if fails, else 0.
 */
int NAP_syslog_printf (enum NAP_syslog_level level, enum NAP_syslog_facility facility,
                       const char *fmt, ...);

/**
 * Close SYSLOG connection (socket).
 */
void NAP_syslog_close (void);

/** @}
 */

/** @}
 */

#endif
