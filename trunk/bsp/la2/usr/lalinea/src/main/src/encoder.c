/*
 * encoder.c
 *
 *  Created on: 25.12.2013
 *      Author: maik
 */

#include <mfs/descriptor.h>
#include <arch/cpu.h>
#include <encoder.h>

/* to avoid false pulse detection, check relation between high and low cycle,
 * sometimes reg-A and reg-B contain same value.
 * normally the duty fact is somewhat between 20 and 30 but give more space because of acceleration.
 */
#define DUTY_FACT_MIN  5
#define DUTY_FACT_MAX  50
#define WHEEL_DIAMETER_MM  66
#define MOTOR_GEAR_FACT    75
#define ENC_RESOLUTION      1

struct LFR_encoder
{
    SEN_encoder_t enc;
    CSP_GPT_T * GPT;
    ACE_u16_t volatile reg_value[3];
};


LFR_encoder_t enc1;
LFR_encoder_t enc2;

static void
encoder_interrupt (LFR_encoder_t *enc)
{
    ACE_u32_t state = CSP_GPT_GET_SR(enc->GPT);
    if (state & (LDRBS)){
        enc->reg_value[0] = CSP_GPT_GET_RA(enc->GPT);
        enc->reg_value[1] = CSP_GPT_GET_RB(enc->GPT);
        ACE_u16_t duty_fact = (enc->reg_value[1] / enc->reg_value[0]);
        if ( duty_fact > DUTY_FACT_MIN && duty_fact < DUTY_FACT_MAX){
            SEN_encoder_tick(&enc->enc, enc->reg_value[1]);
        } else {
            SEN_encoder_err(&enc->enc);
        }
    }
}

static void
encoder1_interrupt (void)
{
    encoder_interrupt(&enc1);
}

static void
encoder2_interrupt (void)
{
    encoder_interrupt(&enc2);
}

static void
encoder_init(LFR_encoder_t *enc, CSP_GPT_T * GPT, void (*callback)(void))
{
    enc->GPT = GPT;
    enc->reg_value[0] = 0;
    enc->reg_value[1] = 0;
    enc->reg_value[2] = 10000;
    CSP_GPTInit(enc->GPT, CLKS_MCK1024 | ETRGEDG_FALLING | ABETRG_TIOA | LDRA_RISING | LDRB_FALLING, (ACE_u16_t*)enc->reg_value);
    CSP_GPTConfigInterrupt(enc->GPT, (HIGH_LEVEL_SENSITIVE|PRIOR_2), LDRBS, (U32_T)callback);
    SEN_encoder_init(&enc->enc, LA2_SYSTEM_CLOCK / 1024, ENC_RESOLUTION, MOTOR_GEAR_FACT, WHEEL_DIAMETER_MM);
}

extern void
LFR_encoder_init(void)
{
    encoder_init(&enc1, GPT0CH0, encoder1_interrupt);
    encoder_init(&enc2, GPT0CH1, encoder2_interrupt);
}

extern void
LFR_encoder_enable(LFR_encoder_t *enc)
{
    CSP_GPTEnable(enc->GPT);
}

extern void
LFR_encoder_disable(LFR_encoder_t *enc)
{
    CSP_GPTDisable(enc->GPT);
}

