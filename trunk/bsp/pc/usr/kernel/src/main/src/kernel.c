#include <uso/log.h>
#include <uso/sleep.h>
#include <uso/event.h>
#include <dev/cpu.h>
#include <mfs/sysfs.h>
#include <mfs/directory.h>
#include <nap/ymodem.h>

#include <kernel.h>
#include <err.h>

void
PC_kernel_main (void)
{

    USO_log_puts (USO_LL_INFO, "Kernel: "PC_KERNEL" -- V 0.0.0 -- \n");

    MFS_descriptor_t *app = MFS_resolve(MFS_get_root(), "app");
    MFS_descriptor_t *core;
    core = MFS_directory_create (app, "core");

    for (;;)
    {
        USO_sleep (USO_MSEC_2_TICKS (1500));
        USO_log_puts (USO_LL_PROTOCOL, "M");
    }

}
