/*
 * ymodem.h
 *
 *  Created on: 23.12.2012
 *      Author: maik
 */

#include <mfs/descriptor.h>

#ifndef NAP_YMODEM_H_
#define NAP_YMODEM_H_

#define NAP_YMODEM_PACKET_SEQNO_INDEX      (1)
#define NAP_YMODEM_PACKET_SEQNO_COMP_INDEX (2)

#define NAP_YMODEM_PACKET_HEADER           (3)     /* start, block, block-complement */
#define NAP_YMODEM_PACKET_TRAILER_CRC      (2)     /* CRC bytes */
#define NAP_YMODEM_PACKET_TRAILER          (1)
#define NAP_YMODEM_PACKET_OVERHEAD_CRC     (NAP_YMODEM_PACKET_HEADER + NAP_YMODEM_PACKET_TRAILER_CRC)
#define NAP_YMODEM_PACKET_OVERHEAD         (NAP_YMODEM_PACKET_HEADER + NAP_YMODEM_PACKET_TRAILER)
#define NAP_YMODEM_PACKET_SIZE             (128)
#define NAP_YMODEM_PACKET_1K_SIZE          (1024)

#define NAP_YMODEM_FILE_NAME_LENGTH        (64)
#define NAP_YMODEM_FULL_FILE_NAME_LENGTH   (256)
#define NAP_YMODEM_FILE_SIZE_LENGTH        (16)


#define NAP_YMODEM_INITIAL_TIME    (15)
#define NAP_YMODEM_QUIT_TIME       (10)
#define NAP_YMODEM_NAK_TIMEOUT     (5)

/* Number of attempts at soliciting CRC mode from sender before falling
 * back to arithmetic checksum:
 */
#define NAP_YMODEM_MAX_CRC_TRIES (10)

/* Number of consecutive receive errors we will tolerate before giving
 * up:
 */
#define NAP_YMODEM_MAX_ERRORS    (5)

/* Returns the length of the file received, or 0 on error. */
extern ACE_size_t NAP_ymodem_receive(char *name, ACE_size_t (*f)(char *data, int length, MFS_descriptor_t *desc), MFS_descriptor_t *desc);

extern void NAP_ymodem_install (void);

#endif /* NAP_YMODEM_H_ */
