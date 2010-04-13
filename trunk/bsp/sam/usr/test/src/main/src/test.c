#include <uso/log.h>
#include <uso/thread.h>
#include <uso/sleep.h>
#include <dev/arch/cpu.h>

#include "arch/digio.h"
#include "init/init.h"
#include "init/main.h"

static void debug_test(int cycle) ACE_SECTION_ (".ramcode");
static void debug_test(int cycle)
{
    int a, b;
    a = cycle;
    ++cycle;
    b = cycle;
    a += b;
}

void SAM_main(void)
{
    int cycle;
    USO_kputs (USO_LL_INFO, "Test 0.1.1\n");
    DEV_digout_set (&SAM_green_led);
 
    cycle = 1;
    for (;;)
    {
        debug_test(cycle);
        cycle++;
        USO_sleep(1000);    
    }
}

