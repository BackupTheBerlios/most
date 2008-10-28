#include <uso/thread.h>
#include <uso/sleep.h>
#include <dev/arch/at91/debug.h>

#include "arch/OLIMEX_SAM7_EX256.h"
#include "init/init.h"
#include "init/main.h"

static void debug_test(int cycle) _SECTION_ (".ramcode");

void SAM_main(void)
{
    int cycle;
    cycle = 1;
    for (;;)
    {
        dbgu_print_ascii("Test 0.1.0 ");
        dbgu_print_hex8(cycle);
        dbgu_print_ascii("\n");
        debug_test(cycle);
        cycle++;
        USO_sleep(1000);    
    }
}

static void debug_test(int cycle)
{
    int a, b;
    a = cycle;
    ++cycle;
    b = cycle;
    a += b;
}
