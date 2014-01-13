/*
 * encoder.h
 *
 *  Created on: 25.12.2013
 *      Author: maik
 */

#ifndef LFR_ENCODER_H_
#define LFR_ENCODER_H_

#include <dev/arch/at91sam7a2/csp.h>
#include <sen/encoder.h>

typedef struct LFR_encoder LFR_encoder_t;

extern LFR_encoder_t enc1;
extern LFR_encoder_t enc2;

extern void
LFR_encoder_init(void);

extern void
LFR_encoder_enable(LFR_encoder_t *enc);

extern void
LFR_encoder_disable(LFR_encoder_t *enc);

#endif /* ENCODER_H_ */
