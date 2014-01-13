/*
 * ultra_sonic.c
 *
 *  Created on: 04.01.2014
 *      Author: maik
 */


#include <uso/cpu.h>
#include <arch/cpu.h>
#include <arch/pins.h>
#include <arch/digio.h>
#include <usonic_ranger.h>


struct LFR_ultra_sonic
{
    SEN_usonic_ranger_t ranger;
    CSP_GPT_T * GPT;
    ACE_u16_t volatile reg_value[3];
};

LFR_ultra_sonic_t usonic;

static void
usonic_interrupt (LFR_ultra_sonic_t *usonic)
{
    ACE_u32_t state = CSP_GPT_GET_SR(usonic->GPT);
    if (state & (LDRBS)){
        usonic->reg_value[0] = CSP_GPT_GET_RA(usonic->GPT);
        usonic->reg_value[1] = CSP_GPT_GET_RB(usonic->GPT);
        if(usonic->reg_value[1] > usonic->reg_value[0]){
            SEN_usonic_ranger_pulse (&usonic->ranger, usonic->reg_value[1] - usonic->reg_value[0]);
        } else {
            SEN_usonic_ranger_err (&usonic->ranger);
        }
    }
}

static void
usonic1_interrupt (void)
{
    usonic_interrupt(&usonic);
}

static void
ultra_sonic_init(LFR_ultra_sonic_t *usonic, DEV_digout_t *out, CSP_GPT_T * GPT, void (*interrupt_func)(void))
{
    usonic->GPT = GPT;
    usonic->reg_value[0] = 0;
    usonic->reg_value[1] = 0;
    usonic->reg_value[2] = 10000;
    CSP_GPTInit(usonic->GPT, CLKS_MCK128 | ETRGEDG_FALLING | ABETRG_TIOA | LDRA_RISING | LDRB_FALLING, (ACE_u16_t*)usonic->reg_value);
    CSP_GPTConfigInterrupt(usonic->GPT, (HIGH_LEVEL_SENSITIVE|PRIOR_2), LDRBS, (U32_T)interrupt_func);
    SEN_usonic_ranger_init(&usonic->ranger, out, LA2_SYSTEM_CLOCK / 128, LA2_pins_cfg_out, LA2_pins_cfg_in, ULTRA_SONIC);
}

extern void
LFR_ultra_sonic_init(void)
{
    ultra_sonic_init(&usonic, &LA2_ultra_sonic, GPT0CH2, usonic1_interrupt);
}

extern void
LFR_ultra_sonic_enable(LFR_ultra_sonic_t *usonic)
{
    CSP_GPTEnable(usonic->GPT);
}

extern void
LFR_ultra_sonic_disable(LFR_ultra_sonic_t *usonic)
{
    CSP_GPTDisable(usonic->GPT);
}

