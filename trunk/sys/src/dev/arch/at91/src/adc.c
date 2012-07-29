#include "dev/arch/at91/AT91SAM7X256.h"
#include "dev/arch/at91/adc.h"
#include "dev/adc.h"

AT91PS_ADC a_pADC = AT91C_BASE_ADC;
AT91PS_PMC a_pPMC = AT91C_BASE_PMC;

extern void
DEV_at91_ADC_init (void)
{

    // Enable clock for interface
    a_pPMC->PMC_PCER = 1 << AT91C_ID_ADC;

    // Reset
    a_pADC->ADC_CR = 0x1;
    a_pADC->ADC_CR = 0x0;

    // Set maximum startup time and hold time
    a_pADC->ADC_MR = 0x0F1F0F00;

}


extern ACE_u32_t
DEV_at91_ADC_get (enum DEV_adc_channel channel)
{

    // variable
    unsigned int result = 0;

    // Enable desired chanel
    a_pADC->ADC_CHER = (1 << channel);

    // Start conversion
    a_pADC->ADC_CR = 0x2;

    // wait for end of convertion
    while (!(a_pADC->ADC_SR & (1 << channel)));

    switch (channel)
    {
    case DEV_ADC_CHN_0:
        result = a_pADC->ADC_CDR0;
        break;
    case DEV_ADC_CHN_1:
        result = a_pADC->ADC_CDR1;
        break;
    case DEV_ADC_CHN_2:
        result = a_pADC->ADC_CDR2;
        break;
    case DEV_ADC_CHN_3:
        result = a_pADC->ADC_CDR3;
        break;
    case DEV_ADC_CHN_4:
        result = a_pADC->ADC_CDR4;
        break;
    case DEV_ADC_CHN_5:
        result = a_pADC->ADC_CDR5;
        break;
    case DEV_ADC_CHN_6:
        result = a_pADC->ADC_CDR6;
        break;
    case DEV_ADC_CHN_7:
        result = a_pADC->ADC_CDR7;
        break;
    }

    return result;

}
