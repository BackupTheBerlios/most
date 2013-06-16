/*
 * pwm.h
 *
 *  Created on: 29.04.2010
 *      Author: maik
 */

#ifndef DEV_PWM_H_
#define DEV_PWM_H_

#include <ace/stddef.h>

/** @addtogroup dev
 * @{
 */

/** @defgroup pwm pwm.h
 *
 * @{
 */

extern short DEV_pwm_sin[];

struct DEV_pwm_interface
{
    void (*set_period) (ACE_u8_t, ACE_u16_t, ACE_u16_t);
    void (*set_duty) (ACE_u8_t, ACE_u16_t, ACE_u16_t);
    void (*enable) (ACE_u8_t);
    void (*disable) (ACE_u8_t);
};

struct DEV_pwm
{
    struct DEV_pwm_interface *interface;
    ACE_u8_t channel;
    ACE_u16_t period;
    ACE_u16_t duty;
};

typedef struct DEV_pwm DEV_pwm_t;

extern void DEV_pwm_init (DEV_pwm_t * pwm, struct DEV_pwm_interface *interface, ACE_u8_t channel);

extern void DEV_pwm_set_period (DEV_pwm_t * pwm, ACE_u16_t period, ACE_u16_t duty);
extern void DEV_pwm_set_duty (DEV_pwm_t * pwm, ACE_u16_t duty);
extern void DEV_pwm_enable (DEV_pwm_t * pwm);
extern void DEV_pwm_disable (DEV_pwm_t * pwm);


/** @}
 */

/** @}
 */

#endif /* DEV_PWM_H_ */
