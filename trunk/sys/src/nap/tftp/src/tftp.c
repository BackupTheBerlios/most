#include <ace/stddef.h>
#include <ace/stdlib.h>
#include <ace/string.h>
#include <net/udp.h>

#include "nap/tftp.h"
#include "nap/debug.h"

/********************************************************************************/
/* Declarations & Defines *******************************************************/
/********************************************************************************/

#define SENDTRIES 5

#define READ_REQUEST 1
#define WRITE_REQUEST 2
#define DATA 3
#define ACK 4
#define ERROR 5

static void init_read_request(void);
static void init_ack(void);
static void send(void);
static void wait_reply(void);
static void check_reply(void);

/********************************************************************************/
/* Definitions ******************************************************************/
/********************************************************************************/

static struct {
	void (*action)(void);
	unsigned short opcode;
	char *mode;
	unsigned short block_nr;
	unsigned short error_code;
	char *error_message;
	int eof;
   int sendcount;
} state; 

static const char* mode[] = { "netascii", "octet", "mail", };

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
    NET_netbuf_t buf_hdr;
    unsigned char data[NAP_TFTP_MAXSIZE];
} _PACKED_;


/********************************************************************************/
/* TFTP *************************************************************************/
/********************************************************************************/
static USO_buf_pool_t pool;
static struct tftp_packet packets[3]; // sollte alloziert werden
static NET_netbuf_t *send_packet, *receive_packet;
static NET_ip_addr_t *server_addr;

static const char *file_name;
static bool_t (*callback)(char *data, size_t length);

static NET_udp_socket_t sock;

extern void
NAP_tftp_open(NET_ip_addr_t *client_addr, NET_ip_addr_t *_server_addr)
{
	server_addr = _server_addr;
    USO_buf_pool_init (&pool, packets, 3,
                         sizeof (struct tftp_packet));
    NET_udp_socket_init (&sock);
    NET_udp_bind (&sock, client_addr, 32798);
	NET_udp_connect (&sock, server_addr, htons(NAP_TFTP_SERVER_PORT));
    NET_udp_socket_open (&sock);
    NET_udp_recv_timeout (&sock, ACE_MSEC_2_TICKS(4000));
}

extern void
NAP_tftp_get(const char* filename, bool_t (*f)(char *, size_t))
{
    file_name = filename;
	callback = f;
	state.action = init_read_request;
	state.block_nr = 0;
	state.eof = 0;
	state.sendcount = 0;
	while(state.action != NULL){
		state.action();
	}
    USO_kputs (USO_LL_INFO, "TFTP done\n");
}

void NAP_tftp_close(void)
{
    NET_udp_socket_close (&sock);
    DEBUGF(NAP_TFTP_DEBUG, ("TFTP sock closed\n") );
}

static void init_read_request(void)
{
	char* data;
	unsigned short temp;
    DEBUGF(NAP_TFTP_DEBUG, ("TFTP init read request\n") );
	send_packet = NET_netbuf_alloc (&pool, 0, NULL);
	data = send_packet->data;
	temp = htons(READ_REQUEST);
	memcpy(data, &temp, sizeof(temp));
	data += sizeof(temp);
	strcpy(data, file_name);
	data += strlen(file_name) + 1;
	strcpy(data, mode[1]); 
	data += strlen(mode[1]) + 1;
	NET_netbuf_len_adjust (send_packet, data - send_packet->data);
	state.action = send;
}  

static void init_ack(void){
	char* data;
	unsigned short temp;
    DEBUGF(NAP_TFTP_DEBUG, ("TFTP init ack\n") );
	send_packet = NET_netbuf_alloc (&pool, 0, NULL);
	data = send_packet->data;
	temp = htons(ACK);
	memcpy(data, &temp, sizeof(temp));
	data += sizeof(temp);
	temp = htons(state.block_nr);
	memcpy(data, &temp, sizeof(temp));
	data += sizeof(temp);
	NET_netbuf_len_adjust (send_packet, data - send_packet->data);
	state.action = send;
}

static void send(void)
{
    DEBUGF(NAP_TFTP_DEBUG, ("TFTP send\n") );
	NET_err_t err;
	err = NET_udp_send (&sock, send_packet);
	if (err == NET_ERR_OK){	
		if (state.eof == 0){
			state.action = wait_reply;
			state.sendcount++;
    	} else {
			state.action = NULL;
		}
	}else{
	    USO_kprintf (USO_LL_ERROR, "TFTP send error %d\n", err);
		state.action = NULL;
	}
}

static void wait_reply(void)
{
    u16_t server_port;
    DEBUGF(NAP_TFTP_DEBUG, ("TFTP wait reply\n") );
	receive_packet = NET_udp_recv (&sock, NULL, &server_port);
	if (receive_packet != NULL)	{
		if (state.block_nr == 0){
			NET_udp_connect (&sock, server_addr, htons(server_port));
			state.action = check_reply;
		} else if ( TRUE /*server_port == servaddr.sin_port*/){
			state.action = check_reply;
		} else {
			USO_kputs (USO_LL_INFO, "TFTP wrong port nr\n");
			NET_netbuf_free (receive_packet);
		}
	} else {
		if (state.sendcount < SENDTRIES){
			if (state.block_nr == 0){
				state.action = init_read_request;
			} else {
				state.action = init_ack;
			}
		}else {
			state.action = NULL;
			USO_kputs (USO_LL_ERROR, "TFTP recfrom failed \n");
		}
	}
} 

static void check_reply(void)
{
	char *data = receive_packet->index;
	long length = receive_packet->len;
	long header = sizeof(state.opcode) + sizeof(unsigned short);
    DEBUGF(NAP_TFTP_DEBUG, ("TFTP check reply\n") );
	if (length >= header) {
		state.sendcount = 0;
		length -= header;
		state.opcode = ntohs(*(unsigned short*)data);
		data += sizeof(unsigned short);
		switch (state.opcode){
		case DATA:{
			unsigned short blocknr = (ntohs(*(unsigned short*)data));
			if ( (state.block_nr + 1) == blocknr){
				state.block_nr++;
				data += sizeof(unsigned short);
				if (length < NAP_TFTP_TRANSFERSIZE){
					state.eof = 1;
				}
				if (callback(data, length) == TRUE){
					state.action = init_ack;
				} else {
					USO_kputs (USO_LL_ERROR, "TFTP callback error\n");
					state.action = NULL;
				}
			} else {
				state.action = wait_reply;
				USO_kprintf (USO_LL_ERROR, "TFTP inval block nr: %d\n", blocknr);
			}
			break;
		}
		case READ_REQUEST:
		case WRITE_REQUEST:
		case ACK:
		case ERROR:
		default:
			USO_kputs (USO_LL_ERROR, "TFTP inval op code\n");
			state.action = NULL;
			break;
			
		} /* end switch opcode*/
	} else {
		USO_kputs (USO_LL_ERROR, "TFTP len < header\n");
		state.action = init_read_request;
	}
    NET_netbuf_free (receive_packet);
}

/* eof */
