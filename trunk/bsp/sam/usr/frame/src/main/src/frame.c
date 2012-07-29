#include <uso/log.h>
#include <uso/sleep.h>
#include <mfs/directory.h>
#include <mfs/sysfs.h>
#include <nap/syslog.h>

#include <frame.h>

void
SAM_main (void)
{
    USO_log_puts (USO_LL_INFO, SAM_APPLICATION " " ACE_MOST_BUILD "\n");
    USO_log_puts (USO_LL_INFO, " -- 1 --\n");      /* Increase number to determine a successful download */
}
