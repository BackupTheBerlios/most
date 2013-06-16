/*
 * receive.c
 *
 *  Created on: 20.11.2012
 *      Author: maik
 *
 * use USO_log or DEBUGF only if it doesn't write to stdout, otherwise it will break the ymodem connection !
 */

#include <ace/stddef.h>
#include <ace/stdio.h>
#include <ace/string.h>
#include <ace/stdlib.h>
#include <uso/sleep.h>
#include <uso/scheduler.h>
#include <dev/serial.h>
#include <cli/tty.h>
#include <mfs/descriptor.h>
#include <nap/ymodem.h>

static int
receive_packet(char *data, int *length)
{
	unsigned int packet_size;
	int c;

	*length = 0;

	c = ACE_getc();
	if(c == ACE_EOF)
		return -1;

	switch (c) {
	case ACE_SOH:
		packet_size = NAP_YMODEM_PACKET_SIZE;
		break;
	case ACE_STX:
		packet_size = NAP_YMODEM_PACKET_1K_SIZE;
		break;
	case ACE_EOT:
		return 0;
	case ACE_CAN:
		if ( (c = ACE_getc()) == ACE_CAN) {
			*length = -1;
			return 0;
		}
		/* no break */
	/* first octet invalid ! (retry) */
	default:
		*length = -1;
		return -1;
	}

	*data = c;

	unsigned int overhead = (packet_size == NAP_YMODEM_PACKET_SIZE) ? NAP_YMODEM_PACKET_OVERHEAD : NAP_YMODEM_PACKET_OVERHEAD_CRC;
	ACE_size_t size = packet_size + overhead - 1;

	if (ACE_read (data + 1, size) < size)
		return -1;

	/* check complement sequence number */
	if (data[NAP_YMODEM_PACKET_SEQNO_INDEX] != ( (data[NAP_YMODEM_PACKET_SEQNO_COMP_INDEX] ^ 0xff) & 0xff) )
		return 1;

	/* check checksum or CRC */
	/* todo */

	*length = packet_size;
	return 0;
}

static ACE_bool_t
filename_packet(char *packet_data, int packet_len, char* name, ACE_size_t *size, ACE_size_t files_received)
{
	int i;
	char file_size[NAP_YMODEM_FILE_SIZE_LENGTH];
	char *file;

	/* check for empty packet */
	for (i = NAP_YMODEM_PACKET_HEADER; i < NAP_YMODEM_PACKET_HEADER + 4; ++i){
		if(packet_data[i] != 0){
			break;
		}
	}

	if (i < NAP_YMODEM_PACKET_HEADER + 4) {  /* filename packet has data */
		file = &packet_data[i];

		if (files_received >= 1){ /* just support one file */
			ACE_putc(ACE_CAN);
			ACE_putc(ACE_CAN);
			return TRUE;
		}

		if (name != NULL){
			ACE_strncpy(name, file, NAP_YMODEM_FILE_NAME_LENGTH - 1);
			name[NAP_YMODEM_FILE_NAME_LENGTH - 1] = '\0';
		}

		file += ACE_strnlen(file, (packet_len == NAP_YMODEM_PACKET_SIZE) ?
				NAP_YMODEM_FILE_NAME_LENGTH : NAP_YMODEM_FULL_FILE_NAME_LENGTH);

		for (++file, i = 0; *file != ' ' && i < (NAP_YMODEM_FILE_SIZE_LENGTH - 1);){
			file_size[i++] = *file++;
		}
		file_size[i++] = '\0';
		*size = ACE_atol(file_size);

		ACE_putc(ACE_ACK);
		/* procced with X-Modem filetransfer */
		ACE_putc(ACE_NAK);
		return FALSE;

	} else {  /* filename packet is empty; end session */
		ACE_putc(ACE_ACK);
		return TRUE;
	}

}

extern ACE_size_t
NAP_ymodem_receive(char *name, ACE_size_t (*process_packet)(char *data, int length, MFS_descriptor_t *desc), MFS_descriptor_t *desc)
{
	ACE_bool_t file_done, session_done;
	ACE_size_t file_size = 0, errors = 0;
	ACE_size_t files_received, packets_received;
	ACE_size_t rx_data_size = 0;

	char *packet_data = ACE_malloc(NAP_YMODEM_PACKET_1K_SIZE + NAP_YMODEM_PACKET_OVERHEAD_CRC);
	if (packet_data == NULL){
		return 0;
	}

	MFS_control_desc (USO_current()->out, MFS_CTRL_TTY_OUT_MODE, CLI_TTY_MODE_RAW);
	MFS_control_desc (USO_current()->in, MFS_CTRL_TTY_IN_MODE, CLI_TTY_MODE_RAW);
	MFS_control_desc (USO_current()->in, MFS_CTRL_SER_RX_TIMEOUT, NAP_YMODEM_NAK_TIMEOUT);

	USO_sleep(USO_SEC_2_TICKS(NAP_YMODEM_INITIAL_TIME));
	ACE_putc(ACE_NAK);

	for (files_received = 0, session_done = FALSE; session_done == FALSE; ) {


		for (packets_received = 0, file_done = FALSE; file_done == FALSE; ) {
			int packet_length;

			packet_length = 0;

			switch (receive_packet(packet_data, &packet_length)) {

			case 0: /* packet received */
			{
				errors = 0;
				switch (packet_length) {
				case -1:  /* abort */
					ACE_putc(ACE_ACK);
					file_done = TRUE;
					session_done = TRUE;
					break;
				case 0:   /* end of transmission */
					ACE_putc(ACE_ACK);
					file_done = TRUE;
					++files_received;
					break;
				default:  /* normal packet */
					/* check sequence number */
					if ((packet_data[NAP_YMODEM_PACKET_SEQNO_INDEX] & 0xff) !=
						(packets_received & 0xff)) {
						ACE_putc(ACE_NAK); /* sequence number false */
					} else { /* sequence number ok */
						if (packets_received == 0) {
							/* filename packet */
							ACE_bool_t end;
							end = filename_packet(packet_data, packet_length, name, &file_size, files_received);
							file_done = end;
							session_done = end;
						} else {
							/* data packet */
							int written_length, packet_data_length;
							packet_data_length = packet_length;
							if (rx_data_size > file_size){ /** received to much data packets */
								ACE_putc(ACE_CAN);
								ACE_putc(ACE_CAN);
								file_done = TRUE;
								session_done = TRUE;
								break;
							}
							rx_data_size += packet_length;
							if (rx_data_size > file_size){
								packet_data_length -= (rx_data_size - file_size);
								rx_data_size = file_size;
							}
							written_length = process_packet (packet_data + NAP_YMODEM_PACKET_HEADER, packet_data_length, desc);
							if (written_length < packet_data_length){ /* process data error */
								ACE_putc(ACE_CAN);
								ACE_putc(ACE_CAN);
								file_done = TRUE;
								session_done = TRUE;
								break;
							}
							ACE_putc(ACE_ACK); /* data processed */
						}
						++packets_received;
					} /* sequence number */
					break;
				} /* switch packet length */
				break;
			} /* packet received case */

			case 1: /* inval seq nr */
			case -1: /* receive error */
			default:
			{
				if (++errors >= (packets_received == 0 ? NAP_YMODEM_MAX_CRC_TRIES : NAP_YMODEM_MAX_ERRORS)) {
					ACE_putc(ACE_CAN);
					ACE_putc(ACE_CAN);
					file_done = TRUE;
					session_done = TRUE;
					break;
				}
				ACE_putc(ACE_NAK);
				break;
			} /* error case */
			} /* switch packet_receive() code */
		}  /* receive file */
	}  /* receive session */

	USO_sleep(USO_SEC_2_TICKS(NAP_YMODEM_QUIT_TIME));

	MFS_control_desc (USO_current()->in, MFS_CTRL_SER_RX_TIMEOUT, 0);
	MFS_control_desc (USO_current()->in, MFS_CTRL_TTY_IN_MODE, CLI_TTY_MODE_COOKED);
	MFS_control_desc (USO_current()->out, MFS_CTRL_TTY_OUT_MODE, CLI_TTY_MODE_COOKED);

	ACE_free(packet_data);

	return ACE_MIN(file_size,rx_data_size);
}
