#include <uso/log.h>
#include <uso/sleep.h>
#include <uso/event.h>
#include <dev/cpu.h>
#include <mfs/sysfs.h>
#include <mfs/directory.h>
#include <nap/ymodem.h>
#include <arch/digio.h>
#include <init/events.h>

#include <boot.h>
#include <err.h>

#define BOOT_SLOT_SIZE 3

static USO_slot_t boot_slot;

static const long boot_event_ids[] = {
    LA2_card_inserted,
    LA2_card_removed,
    0
};

void
LA2_main (void)
{

    USO_log_puts (USO_LL_INFO, "App: "LA2_APPLICATION" -- 4 -- \n");

    DEV_digout_set (&LA2_green_led);

    MFS_descriptor_t *app = MFS_resolve(MFS_get_root(), "app");
    MFS_descriptor_t *boot;
    boot = MFS_directory_create (app, "boot");

    USO_slot_init (&LA2_event_slots, &boot_slot, boot_event_ids, BOOT_SLOT_SIZE);

    for (;;)
    {
        ACE_bool_t free;
        USO_node_t *e = USO_slot_get_event (&boot_slot);
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

        free = USO_slots_put_event (&LA2_event_slots, &boot_slot, e, FALSE);

        if (free == FALSE)
            USO_buf_free (&LA2_event_pool, e);
    }

}
