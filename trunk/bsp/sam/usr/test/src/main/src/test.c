#include <uso/thread.h>
#include <uso/sleep.h>
#include <dev/arch/at91/debug.h>

#include "arch/OLIMEX_SAM7_EX256.h"
#include "init/init.h"
#include "init/main.h"

void SAM_main(void)
{
    int cycle = 1;
    for (;;)
    {
        dbgu_print_ascii("Test Version 0.0.1: ");
        dbgu_print_hex8(cycle);
        dbgu_print_ascii("\n");
        cycle++;
        USO_sleep(1000);    
    }
}
