#include <ace/stdio.h>
#include <ace/stddef.h>
#include <ace/stdlib.h>
#include <ace/string.h>
#include <uso/mutex.h>
#include <net/udp_sock.h>

#include "nap/syslog.h"
#include "nap/debug.h"

#define NAP_SYSLOG_SERVER_PORT 514        /**< SYSLOG server port. */
#define NAP_SYSLOG_MSG_SIZE 0x90         /**< SYSLOG max log message size. */
#define NAP_SYSLOG_HEAD_SIZE 0x10            /**< SYSLOG header <facility+level>. */

static char msg_buf[NAP_SYSLOG_MSG_SIZE];
static USO_mutex_t buf_sync;

static USO_buf_pool_t pool;
static NET_netbuf_t *send_packet = NULL;

struct syslog_packet
{
    unsigned char msg[NAP_SYSLOG_HEAD_SIZE + NAP_SYSLOG_MSG_SIZE];
} ACE_PACKED_;

static struct syslog_packet packets[1];

static NET_udp_socket_t sock;

extern int
NAP_syslog_open (NET_ip_addr_t * client_addr, NET_ip_addr_t * server_addr)
{
    USO_mutex_init (&buf_sync);
    USO_buf_pool_init (&pool, packets, 1, sizeof (struct syslog_packet));
    NET_udp_socket_init (&sock);
    NET_udp_bind (&sock, client_addr, 49152);
    NET_udp_connect (&sock, server_addr, NAP_SYSLOG_SERVER_PORT);
    NET_udp_socket_open (&sock);
    DEBUGF (NAP_SYSLOG_DEBUG, ("SYSLOG sock open\n"));
    return 0;
}


extern void
NAP_syslog_close (void)
{
    NET_udp_socket_close (&sock);
    DEBUGF (NAP_SYSLOG_DEBUG, ("SYSLOG sock closed\n"));
}

extern int
NAP_syslog_puts (enum NAP_syslog_level level, enum NAP_syslog_facility facility, char *msg)
{
    char *data;
    DEBUGF (NAP_SYSLOG_DEBUG, ("SYSLOG puts\n"));
    send_packet = NET_netbuf_alloc_pool (&pool);
    data = NET_netbuf_index (send_packet);
    int pri = (facility * 8) + level;
    int len = ACE_sprintf (data, "<%d>%s", pri, msg);
    if (len >= 0 && len < (NAP_SYSLOG_HEAD_SIZE + NAP_SYSLOG_MSG_SIZE - 1))
    {
        data += len;
        NET_netbuf_trim_len (send_packet, data - NET_netbuf_index (send_packet));
        ACE_err_t err;
        err = NET_udp_send_netbuf (&sock, send_packet);
        if (err != ACE_OK)
        {
            USO_log_printf (USO_LL_ERROR, "SYSLOG send error: %d\n", err);
            return -1;
        }
    }
    else
    {
        USO_log_printf (USO_LL_ERROR, "SYSLOG packet buf overflow: len=%d\n", len);
        return -1;
    }
    return 0;
}

extern int
NAP_syslog_printf (enum NAP_syslog_level level,
                   enum NAP_syslog_facility facility, const char *fmt, ...)
{
    int len, ret = 0;
    ACE_va_list_t args;
    USO_lock (&buf_sync);
    ACE_va_start (args, fmt);
    len = ACE_vsprintf (msg_buf, fmt, args);
    ACE_va_end (args);
    if (len >= 0 && len < sizeof (msg_buf) - 1)
    {
        ret = NAP_syslog_puts (level, facility, msg_buf);
    }
    else
    {
        USO_log_printf (USO_LL_ERROR, "SYSLOG msg_buf overflow: len=%d\n", len);
        ret = -1;
    }
    USO_unlock (&buf_sync);
    return ret;
}
