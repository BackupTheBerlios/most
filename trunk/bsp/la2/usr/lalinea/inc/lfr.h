/*
 * lfr.h
 *
 *  Created on: 05.08.2013
 *      Author: maik
 */

#ifndef LFR_H_
#define LFR_H_

#include <mfs/descriptor.h>

extern void LFR_init (void);

extern void LFR_install(MFS_descriptor_t *dir, char *name);

extern void LFR_start (void);

#endif /* LFR_H_ */
