/*
 * ultra_sonic.h
 *
 *  Created on: 04.01.2014
 *      Author: maik
 */

#ifndef LFR_ULTRA_SONIC_H_
#define LFR_ULTRA_SONIC_H_

#include <sen/usonic_ranger.h>

typedef struct LFR_ultra_sonic LFR_ultra_sonic_t;

extern LFR_ultra_sonic_t usonic;

extern void
LFR_ultra_sonic_init(void);

extern void
LFR_ultra_sonic_enable(LFR_ultra_sonic_t *usonic);

extern void
LFR_ultra_sonic_disable(LFR_ultra_sonic_t *usonic);


#endif /* ULTRA_SONIC_H_ */
