/*
 * switch.h
 *
 *  Created on: 22.11.2013
 *      Author: maik
 */

#ifndef CLI_SWITCH_H_
#define CLI_SWITCH_H_

#include <ace/stddef.h>
#include <mfs/vfs.h>
#include <mfs/stream.h>

extern void CLI_switch_init(void);

extern void CLI_switch_set(int i, MFS_descriptor_t *tty);

extern ACE_bool_t CLI_switch_select(int i);


#endif /* SWITCH_H_ */
