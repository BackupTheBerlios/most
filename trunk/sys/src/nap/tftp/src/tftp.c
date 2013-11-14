#include <ace/stdio.h>
#include <ace/stddef.h>
#include <ace/stdlib.h>
#include <ace/string.h>
#include <uso/scheduler.h>
#include <net/udp_sock.h>
#include <cli/exec.h>
#include <mfs/directory.h>
#include <mfs/sysfs.h>
#include <mfs/block.h>
#include <nap/tftp.h>
#include <nap/debug.h>

/********************************************************************************/
/* Declarations & Defines *******************************************************/
/********************************************************************************/

#define SENDTRIES 5

#define READ_REQUEST 1
#define WRITE_REQUEST 2
#define DATA 3
#define ACK 4
#define ERROR 5

static void init_read_request (void);
static void init_ack (void);
static void send (void);
static void wait_reply (void);
static void check_reply (void);

/********************************************************************************/
/* Definitions ******************************************************************/
/********************************************************************************/

static struct
{
    void (*action) (void);
    unsigned short opcode;
    char *mode;
    unsigned short block_nr;
    unsigned short error_code;
    char *error_message;
    int eof;
    int sendcount;
    MFS_descriptor_t *out;
} state;

static const char *mode[] = { "netascii", "octet", "mail", };

/*
static const char* error[] = {
"Unknown error",
"File not found",
"Access violation",
"Allocation error",
"Operation not supported",
"Transfer ID unknown",
"File already exists",
"No such user",
};
*/

struct tftp_packet
{
    unsigned char data[NAP_TFTP_MAXSIZE];
} ACE_PACKED_;


/********************************************************************************/
/* TFTP *************************************************************************/
/********************************************************************************/
static USO_buf_pool_t pool;
static struct tftp_packet *packets = NULL;
static NET_netbuf_t *send_packet = NULL;
static NET_netbuf_t *receive_packet = NULL;
static NET_ip_addr_t *server_addr;
static NET_ip_addr_t *client_addr;
static ACE_u16_t server_port;
static ACE_u16_t client_port = 3279;

static const char *file_name;
static ACE_bool_t (*callback) (char *data, ACE_size_t length, MFS_descriptor_t *out);

static NET_udp_socket_t sock;

extern int
NAP_tftp_open (void)
{
    packets = ACE_malloc (sizeof (struct tftp_packet));
    if (packets != NULL)
    {
        USO_buf_pool_init (&pool, packets, 1, sizeof (struct tftp_packet));
        NET_udp_socket_init (&sock);
        NET_udp_bind (&sock, client_addr, client_port);
        NET_udp_connect (&sock, server_addr, NAP_TFTP_SERVER_PORT);
        NET_udp_socket_open (&sock);
        NET_udp_recv_timeout (&sock, USO_MSEC_2_TICKS (4000));
        DEBUGF (NAP_TFTP_DEBUG, ("TFTP sock open\n"));
    }
    else
    {
        USO_log_puts (USO_LL_ERROR, "TFTP mem error.\n");
        return -1;
    }
    return 0;
}

extern int
NAP_tftp_get (const char *filename, ACE_bool_t (*f) (char *, ACE_size_t,MFS_descriptor_t*), MFS_descriptor_t *out)
{
    file_name = filename;
    callback = f;
    state.action = init_read_request;
    state.block_nr = 0;
    state.eof = 0;
    state.sendcount = 0;
    state.out = out;
    while (state.action != NULL)
    {
        state.action ();
    }
    if (state.eof == 1)
    {
        USO_log_puts (USO_LL_INFO, "TFTP done\n");
        return 0;
    }
    else
    {
        USO_log_puts (USO_LL_WARNING, "TFTP failed\n");
    }
    return -1;
}

void
NAP_tftp_close (void)
{
    NET_udp_socket_close (&sock);
    DEBUGF (NAP_TFTP_DEBUG, ("TFTP sock closed\n"));
    if (packets != NULL)
    {
        ACE_free (packets);
        packets = NULL;
    }
}

static void
init_read_request (void)
{
    char *data;
    unsigned short temp;
    DEBUGF (NAP_TFTP_DEBUG, ("TFTP init read request\n"));
    send_packet = NET_netbuf_alloc_pool (&pool);
    data = NET_netbuf_index (send_packet);
    temp = ACE_htons (READ_REQUEST);
    memcpy (data, &temp, sizeof (temp));
    data += sizeof (temp);
    ACE_strcpy (data, file_name);
    data += ACE_strlen (file_name) + 1;
    ACE_strcpy (data, mode[1]);
    data += ACE_strlen (mode[1]) + 1;
    NET_netbuf_trim_len (send_packet, data - NET_netbuf_index (send_packet));
    state.action = send;
}

static void
init_ack (void)
{
    char *data;
    unsigned short temp;
    DEBUGF (NAP_TFTP_DEBUG, ("TFTP init ack\n"));
    send_packet = NET_netbuf_alloc_pool (&pool);
    data = NET_netbuf_index (send_packet);
    temp = ACE_htons (ACK);
    memcpy (data, &temp, sizeof (temp));
    data += sizeof (temp);
    temp = ACE_htons (state.block_nr);
    memcpy (data, &temp, sizeof (temp));
    data += sizeof (temp);
    NET_netbuf_trim_len (send_packet, data - NET_netbuf_index (send_packet));
    state.action = send;
}

static void
send (void)
{
    DEBUGF (NAP_TFTP_DEBUG, ("TFTP send\n"));
    ACE_err_t err;
    err = NET_udp_send_netbuf (&sock, send_packet);
    if (err == ACE_OK)
    {
        if (state.eof == 0)
        {
            state.action = wait_reply;
            state.sendcount++;
        }
        else
        {
            state.action = NULL;
        }
    }
    else
    {
        USO_log_printf (USO_LL_ERROR, "TFTP send error %d\n", err);
        state.action = NULL;
    }
}

static void
wait_reply (void)
{
    ACE_u16_t port;
    DEBUGF (NAP_TFTP_DEBUG, ("TFTP wait reply\n"));
    receive_packet = NET_udp_recv_netbuf (&sock, NULL, &port);
    if (receive_packet != NULL)
    {
        if (state.block_nr == 0)
        {
            server_port = port;
            NET_udp_connect (&sock, server_addr, server_port);
            state.action = check_reply;
        }
        else if (server_port == port)
        {
            state.action = check_reply;
        }
        else
        {
            USO_log_puts (USO_LL_WARNING, "TFTP wrong port nr\n");
            NET_netbuf_free (receive_packet);
        }
    }
    else
    {
        if (state.sendcount < SENDTRIES)
        {
            if (state.block_nr == 0)
            {
                state.action = init_read_request;
            }
            else
            {
                state.action = init_ack;
            }
        }
        else
        {
            state.action = NULL;
            USO_log_puts (USO_LL_ERROR, "TFTP recf failed \n");
        }
    }
}

static void
check_reply (void)
{
    char *data = NET_netbuf_index (receive_packet);
    long length = NET_netbuf_tot_len (receive_packet);
    long header = sizeof (state.opcode) + sizeof (unsigned short);
    DEBUGF (NAP_TFTP_DEBUG, ("TFTP check reply\n"));
    if (length >= header)
    {
        state.sendcount = 0;
        length -= header;
        state.opcode = ACE_ntohs (*(unsigned short *)data);
        data += sizeof (unsigned short);
        switch (state.opcode)
        {
        case DATA:
            {
                unsigned short blocknr = (ACE_ntohs (*(unsigned short *)data));
                if ((state.block_nr + 1) == blocknr)
                {
                    state.block_nr++;
                    data += sizeof (unsigned short);
                    if (length < NAP_TFTP_TRANSFERSIZE)
                    {
                        state.eof = 1;
                    }
                    if (callback (data, length, state.out) == TRUE)
                    {
                        state.action = init_ack;
                    }
                    else
                    {
                        USO_log_puts (USO_LL_ERROR, "TFTP callback error\n");
                        state.action = NULL;
                    }
                }
                else
                {
                    state.action = wait_reply;
                    USO_log_printf (USO_LL_WARNING, "TFTP inval block nr: %d\n", blocknr);
                }
                break;
            }
        case READ_REQUEST:
        case WRITE_REQUEST:
        case ACK:
        case ERROR:
        default:
            USO_log_puts (USO_LL_WARNING, "TFTP inval op code\n");
            state.action = NULL;
            break;

        }                       /* end switch opcode */
    }
    else
    {
        USO_log_puts (USO_LL_WARNING, "TFTP len < header\n");
        state.action = init_read_request;
    }
    NET_netbuf_free (receive_packet);
}

static CLI_exec_t tftp_get;

static ACE_bool_t
tftp_write_rx_data (char *data, ACE_size_t len, MFS_descriptor_t *out)
{
    ACE_size_t written = ACE_fwrite (out, data, len);
    if (written != len) return FALSE;
    return TRUE;
}

static ACE_err_t
tftp_get_exec (char *file)
{
    ACE_err_t err = ACE_OK;;
    if (file)
    {
        if (NAP_tftp_open () >= 0)
        {
            MFS_descriptor_t *out = USO_current()->out;
            int err = 0;
            if (out->type == MFS_STREAM) {
                err = NAP_tftp_get (file, tftp_write_rx_data, out);
            }

            if (err >= 0)
            {
                USO_log_puts (USO_LL_INFO, "Tftp get done\n");
            }
            else
            {
                USO_log_puts (USO_LL_ERROR, "Tftp get failed\n");
            }
            NAP_tftp_close ();
        }
        else
        {
            USO_log_puts (USO_LL_ERROR, "Tftp open failed\n");
        }
    }
    else
    {
        err = DEF_ERR_ARG;
    }
    return err;
}

extern void
NAP_tftp_install (NET_ip_addr_t * client_address, NET_ip_addr_t * server_address)
{
    server_addr = server_address;
    client_addr = client_address;
    MFS_descriptor_t *dir = MFS_resolve("/sys/cli/exe");
    CLI_exec_init (dir, &tftp_get, "tftp_g", "TFTP get", tftp_get_exec);
    MFS_close_desc(dir);
}
