#include <uso/log.h>
#include <uso/sleep.h>
#include <uso/event.h>
#include <dev/cpu.h>
#include <mfs/sysfs.h>
#include <mfs/directory.h>
#include <nap/ymodem.h>
#include <arch/digio.h>
#include <init/events.h>
#include <init/download.h>

#include <test.h>
#include <err.h>
#include <lfr.h>

#include <tst/ser_test.h>
#include <tst/thread_test.h>
#include <tst/ram_test.h>
#include <tst/net_test.h>
#include <tst/mmc_test.h>
#include <tst/digio_test.h>
#include <tst/debug_test.h>


#define TEST_SLOT_SIZE 3


static USO_slot_t test_slot;

static const long test_event_ids[] = {
    LA2_card_inserted,
    LA2_card_removed,
    0
};

static DEV_digout_t *runninglight[] = {
    &LA2_green_led,
    &LA2_red_led,
    NULL
};

void
LA2_main (void)
{

    USO_log_puts (USO_LL_INFO, "App: "LA2_APPLICATION" -- Check Download = 1 -- \n");
    USO_log_printf (USO_LL_INFO, "Sizeof(char, short, int, long) = %d %d %d %d.\n", sizeof (char), sizeof (short), sizeof (int), sizeof (long));

    NAP_ymodem_install();

    MFS_descriptor_t *app = MFS_resolve(MFS_get_root(), "app");
    MFS_descriptor_t *putboot;
    putboot = MFS_directory_create (app, "putboot");

    LA2_download_install (putboot, LA2_BOOT_START, LA2_BOOT_END);

    MFS_descriptor_t *test;
    test = MFS_directory_create (app, "test");
    TST_ram_test_install (test);
    TST_ser_test_install (test);
    TST_thread_test_install (test);
    TST_net_test_install (test);
    TST_mmc_test_install (test);
    TST_debug_test_install (test);
    TST_digio_test_init (&LA2_pushbutton ,runninglight);
    TST_digio_test_install (test);

    LFR_init ();
    LFR_install(app);

    USO_slot_init (&LA2_event_slots, &test_slot, test_event_ids, TEST_SLOT_SIZE);

    for (;;)
    {
        ACE_bool_t free;
        USO_node_t *e = USO_slot_get_event (&test_slot);
        enum LA2_event event = e->data;

        switch (event)
        {
        case LA2_card_inserted:
            ACE_printf("card_inserted\n");
            if (DEV_digin_isset (&LA2_card_write) == TRUE)
            {
                ACE_printf("card is write enabled\n");
            } else {
                ACE_printf("card is write protected\n");
            }
            break;
        case LA2_card_removed:
            ACE_printf("card_removed\n");
            break;
        default:
            ACE_printf("unknown event\n");
            break;
        }

        free = USO_slots_put_event (&LA2_event_slots, &test_slot, e, FALSE);

        if (free == FALSE)
            USO_buf_free (&LA2_event_pool, e);
    }

}
