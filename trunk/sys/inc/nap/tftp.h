#ifndef NAP_TFTP_H
#define NAP_TFTP_H

/********************************************************************************/
/* Interface ********************************************************************/
/********************************************************************************/
#define NAP_TFTP_SERVER_PORT 69
#define NAP_TFTP_TRANSFERSIZE 512
#define NAP_TFTP_MAXSIZE (2*sizeof(unsigned short)+NAP_TFTP_TRANSFERSIZE)

int NAP_tftp_open(NET_ip_addr_t *client_addr, NET_ip_addr_t *server_addr);
void NAP_tftp_get(const char* filename, bool_t (*f)(char *, size_t));
void NAP_tftp_close(void);

#endif
