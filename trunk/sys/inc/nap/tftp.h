#ifndef NAP_TFTP_H
#define NAP_TFTP_H

/** @addtogroup nap
 * @{
 */

/** @defgroup tftp tftp.h
 *
 * Trivial file transfer protocol for download.
 *
 * @{
 */

/********************************************************************************/
/* Interface ********************************************************************/
/********************************************************************************/
#define NAP_TFTP_SERVER_PORT 69         /**< TFTP server port. */
#define NAP_TFTP_TRANSFERSIZE 512       /**< TFTP transfer size. */
#define NAP_TFTP_MAXSIZE (2*sizeof(unsigned short)+NAP_TFTP_TRANSFERSIZE) /**< TFTP maxsize. */

/**
 * Open a TFTP connection.
 * @param client_addr : Client IP address received with bootp.
 * @param server_addr : Server IP address received with bootp.
 * @return -1 if fails, else 0.
 */
int NAP_tftp_open (void);

/**
 * Get a file from server.
 * @param filename : Name of the file..
 * @param f : Callback function, will be called for each received part of the file.
 * @return -1 if fails, else 0.
 */
int NAP_tftp_get (const char *filename, ACE_bool_t (*f) (char *, ACE_size_t, MFS_descriptor_t*), MFS_descriptor_t *out );

/**
 * Close TFTP connection (socket).
 */
void NAP_tftp_close (void);

extern void NAP_tftp_install (NET_ip_addr_t * client_address, NET_ip_addr_t * server_address);

/** @}
 */

/** @}
 */

#endif
