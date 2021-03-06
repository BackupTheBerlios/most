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

#include <lalinea.h>
#include <err.h>
#include <lfr.h>

#define MAIN_SLOT_SIZE 3

static USO_slot_t main_slot;

static const long main_event_ids[] = {
    LA2_card_inserted,
    LA2_card_removed,
    0
};

void
LA2_main (void)
{

    USO_log_puts (USO_LL_INFO, "App: "LA2_APPLICATION"\n");

    DEV_digout_set (&LA2_green_led);

    MFS_descriptor_t *app = MFS_resolve("/app");
    MFS_descriptor_t *putboot;
    putboot = MFS_directory_create (app, "putboot");
    LA2_download_install (putboot, LA2_BOOT_START, LA2_BOOT_END);

    LFR_init ();
    LFR_install(app, "lfr");
    LFR_start();
    MFS_close_desc(app);

    
    USO_slot_init (&LA2_event_slots, &main_slot, main_event_ids, MAIN_SLOT_SIZE);

    for (;;)
    {
        ACE_bool_t free;
        USO_node_t *e = USO_slot_get_event (&main_slot);
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
            ACE_printf("? event\n");
            break;
        }

        free = USO_slots_put_event (&LA2_event_slots, &main_slot, e, FALSE);

        if (free == FALSE)
            USO_buf_free (&LA2_event_pool, e);
    }

}
