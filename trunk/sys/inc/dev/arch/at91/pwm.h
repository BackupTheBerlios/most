#ifndef DEV_AT91_PWM_H
#define DEV_AT91_PWM_H

#include <ace/stddef.h>
#include "dev/pwm.h"

extern void DEV_at91_pwm_init(void);

extern void DEV_at91_pwm_set(ACE_u32_t val);

#endif
