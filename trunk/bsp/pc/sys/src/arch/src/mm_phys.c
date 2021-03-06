#include <ace/stddef.h>
#include <ace/string.h>
#include <arch/mm.h>
#include <arch/multiboot.h>

/*
 * Der Einfachheit halber deklarieren wir die maximal benoetige Bitmapgroesse
 * statisch (Wir brauchen 4 GB / 4 kB = 1M Bits; 1M Bits sind 1M/32 = 32k
 * Eintraege fuer das Array)
 *
 * Willkuerliche Festlegung: 1 = Speicher frei, 0 = Speicher belegt
 */
#define BITMAP_SIZE 32768
static ACE_u32_t bitmap[BITMAP_SIZE];

static void pmm_mark_used(void* page);

extern const void kernel_start;
extern const void kernel_end;

void PC_pmm_init(struct multiboot_info* mb_info)
{
    struct multiboot_mmap* mmap = mb_info->mbs_mmap_addr;
    struct multiboot_mmap* mmap_end = (void*)
        ((ACE_u32_t) mb_info->mbs_mmap_addr + mb_info->mbs_mmap_length);

    /* Per Default ist erst einmal alles reserviert */
    memset(bitmap, 0, sizeof(bitmap));

    /*
     * Nur, was die BIOS-Memory-Map als frei bezeichnet, wird wieder als frei
     * markiert
     */
    while (mmap < mmap_end) {
        if (mmap->type == 1) {
            /* Der Speicherbereich ist frei, entsprechend markieren */
            ACE_u32_t addr = mmap->base;
            ACE_u32_t end_addr = addr + mmap->length;

            while (addr < end_addr) {
                PC_pmm_free((void*) addr);
                addr += 0x1000;
            }
        }
        mmap++;
    }

    /* Den Kernel wieder als belegt kennzeichnen */
    ACE_u32_t addr = (ACE_u32_t) &kernel_start;
    while (addr < (ACE_u32_t) &kernel_end) {
        pmm_mark_used((void*) addr);
        addr += 0x1000;
    }

    /*
     * Die Multibootstruktur auch, genauso wie die Liste von Multibootmodulen.
     * Wir gehen bei beiden davon aus, dass sie maximal 4k gross werden
     */
    struct multiboot_module* modules = mb_info->mbs_mods_addr;

    pmm_mark_used(mb_info);
    pmm_mark_used(modules);

    /* Und die Multibootmodule selber sind auch belegt */
    int i;
    for (i = 0; i < mb_info->mbs_mods_count; i++) {
        addr = modules[i].mod_start;
        while (addr < modules[i].mod_end) {
            pmm_mark_used((void*) addr);
            addr += 0x1000;
        }
    }
}

void* PC_pmm_alloc(void)
{
    int i, j;

    /*
     * Zunaechst suchen wir komplette Eintraege ab. Wenn der Eintrag nicht null
     * ist, ist mindestens ein Bit gesetzt, d.h. hier ist ein Stueck Speicher
     * frei
     */
    for (i = 0; i < BITMAP_SIZE; i++) {
        if (bitmap[i] != 0) {

            /* Jetzt muessen wir nur noch das gesetzte Bit finden */
            for (j = 0; j < 32; j++) {
                if (bitmap[i] & (1 << j)) {
                    bitmap[i] &= ~(1 << j);
                    return (void*)( (i * 32 + j) * 4096);
                }
            }
        }
    }

    /* Scheint wohl nichts frei zu sein... */
    return NULL;
}

static void pmm_mark_used(void* page)
{
    ACE_u32_t index = (ACE_u32_t) page / 4096;
    bitmap[index / 32] &= ~(1 << (index % 32));
}

void PC_pmm_free(void* page)
{
    ACE_u32_t index = (ACE_u32_t) page / 4096;
    bitmap[index / 32] |= (1 << (index % 32));
}
