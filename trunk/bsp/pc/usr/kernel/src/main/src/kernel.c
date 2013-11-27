#include <ace/stdio.h>
#include <ace/stdlib.h>
#include <uso/log.h>
#include <uso/sleep.h>
#include <uso/event.h>
#include <dev/cpu.h>
#include <cli/exec.h>
#include <cli/read_line.h>
#include <mfs/vfs.h>
#include <mfs/directory.h>
#include <mfs/sysfs.h>

#include <kernel.h>
#include <err.h>

static CLI_exec_t test;


extern ACE_err_t
PC_test_exe (char *arg)
{
    int i, n;
    long l;
    char string[100];
    ACE_printf("Enter i:");
    if (CLI_read_line (string, sizeof(string)) != NULL)
    {
        n = ACE_sscanf(string, "%i", &i);
        (n > 0) ? ACE_printf("\n%i\n", i) : ACE_puts("inval\n");
    }

    ACE_printf("Enter l:");
    if (CLI_read_line (string, sizeof(string)) != NULL)
    {
        n = ACE_sscanf(string, "%li", &l);
        (n > 0) ? ACE_printf("\n%li\n", l) : ACE_puts("inval\n");
    }
    char c = i;
    unsigned char u = i;
    ACE_printf("c = %i u = %i", c, u);
    return ACE_OK;
}

void
PC_main (void)
{

    USO_log_puts (USO_LL_INFO, "App: "PC_KERNEL"\n");

    MFS_descriptor_t *app = MFS_resolve("/app");
    MFS_descriptor_t *core;
    core = MFS_directory_create (app, "core");
    CLI_exec_init(app, &test, "test", "scanf test", PC_test_exe);
    MFS_close_desc(app);
    
    for (;;)
    {
        USO_sleep (USO_MSEC_2_TICKS (1500));
        USO_log_puts (USO_LL_PROTOCOL, "M");
    }

}
