/*
 * read_line.h
 *
 *  Created on: 25.11.2013
 *      Author: maik
 */

#ifndef CLI_READ_LINE_H_
#define CLI_READ_LINE_H_

#include <ace/stddef.h>
#include <ace/stdio.h>

#define CLI_RX_POLLING_TIME USO_MSEC_2_TICKS(250)

extern char *CLI_fread_line (ACE_FILE * in, char *string, ACE_size_t len);

extern char *CLI_read_line (char *string, ACE_size_t len);



#endif /* READ_LINE_H_ */
