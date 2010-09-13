#include "dev/arch/at91/pwm.h"
#include "dev/arch/at91/bits.h"
#include "dev/arch/at91/AT91SAM7X256.h"

AT91PS_PWMC     w_pPWM      = AT91C_BASE_PWMC;
AT91PS_PMC      w_pPMC      = AT91C_BASE_PMC;
AT91PS_PWMC_CH  w_pPWM_CH0  = AT91C_BASE_PWMC_CH0;
AT91PS_PIO      w_pPioA     = AT91C_BASE_PIOA;
AT91PS_PIO      w_pPioB     = AT91C_BASE_PIOB;

extern void DEV_at91_pwm_init(void)
{
  // Enable clock for interface
  w_pPMC->PMC_PCER = 1 << AT91C_ID_PWMC;

  // Set second functionality of pin
  w_pPioB->PIO_PDR = BIT19;
  w_pPioB->PIO_ASR = BIT19;
  w_pPioB->PIO_BSR = 0;



  // Enable clock for PWM
  w_pPWM->PWMC_MR = 0x00010001;

  // Frequency for PWM

  // Period for PWM
  w_pPWM_CH0->PWMC_CPRDR = 1023;

  // Duty for PWM
  w_pPWM_CH0->PWMC_CDTYR = 512;

  // Channel mode register.
  w_pPWM_CH0->PWMC_CMR = 0x0001;

  // Enable PWM channel
  w_pPWM->PWMC_ENA = 1;
}

extern void DEV_at91_pwm_set(ACE_u32_t val)
{
  // set new value
  w_pPWM_CH0->PWMC_CUPDR = val;
}
