#include <dev/arch/at91sam7a2/csp.h>

#include <arch/cpu.h>
#include <arch/exceptions.h>

extern void LA2_cpu_init(void)
{
		int i;
		CSP_GIC_T*    pGic;

        /* Disable PLL */
        CSP_CM_SET_CD(((CSP_CM_T*)CM_BASE_ADDRESS), PLLSLCT);

        /* PLL = 10 ==> Coreclock = 6Mhz*10/2 = 30 Mhz */
        CSP_CM_SET_PDIV(((CSP_CM_T*)CM_BASE_ADDRESS), (PLLDIV2 | 10) );

        /* Enable PLL */
        CSP_CM_SET_CE(((CSP_CM_T*)CM_BASE_ADDRESS), PLLSLCT);

        //-----------------------------------------------------------------------------
        // Reset the Interrupt Controller
        //-------------------------------
        // Normally, the code is executed only if a reset has been actually performed.
        // So, the GIC initialization resumes at setting up the default vectors.
        //-----------------------------------------------------------------------------

        CSP_GICInit();

        // Load System pGIC Base address
        pGic = (CSP_GIC_T*)   GIC_BASE_ADDRESS;

        // Set up the default interrupts handler vectors
        pGic->SVR[0] = (int) LA2_default_fiq_handler ;
        for (i=1;i < 31; i++)
        {
                pGic->SVR[i] = (int) LA2_default_irq_handler ;
        }
        pGic->SPU  = (int) LA2_default_spurious_handler ;
}

extern void
LA2_wdt_trigger (void)
{
    /* todo */
}

