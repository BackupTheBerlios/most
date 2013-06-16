/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

#include <ace/stdio.h>
#include <cli/exec.h>
#include <dev/arch/at91sam7x/flashd.h>
#include <dev/arch/at91sam7x/AT91SAM7X256.h>

#include <arch/cpu.h>
#include <flash_test.h>

static unsigned int buffer[AT91C_IFLASH_PAGE_SIZE / 4];
static unsigned int lastPageAddress;
static volatile unsigned int *lastPageData;

static CLI_exec_t lockfl;

static void
lockfl_exec (char *nix)
{
    unsigned char error;
    unsigned char pageLocked;
    // Lock page
    ACE_printf ("Locking last page: ");
    error = DEV_at91_FLASHD_lock (lastPageAddress, lastPageAddress + AT91C_IFLASH_PAGE_SIZE, 0, 0);
    ACE_printf ("Lock page: err = 0x%02X\n", error);

    // Check that associated region is locked
    ACE_printf ("Checking lock status: ");
    pageLocked =
        DEV_at91_FLASHD_is_locked (lastPageAddress, lastPageAddress + AT91C_IFLASH_PAGE_SIZE);
    ACE_printf ("Lock status = %d\n", pageLocked);

    // Unlock page
    ACE_printf ("Unlocking last page: ");
    error =
        DEV_at91_FLASHD_unlock (lastPageAddress, lastPageAddress + AT91C_IFLASH_PAGE_SIZE, 0, 0);
    ACE_printf ("Unlock page: err = 0x%02X\n", error);

    // Check that associated region is unlocked
    ACE_printf ("Checking lock status: ");
    pageLocked =
        DEV_at91_FLASHD_is_locked (lastPageAddress, lastPageAddress + AT91C_IFLASH_PAGE_SIZE);
    ACE_printf ("Lock status = %d\n", pageLocked);
}

static CLI_exec_t printfl;

static void
printfl_exec (char *nix)
{
    int i;
    ACE_printf ("Checking page contents ");
    for (i = 0; i < (AT91C_IFLASH_PAGE_SIZE / 4); i++)
    {
        ACE_printf (".");
        if (lastPageData[i] != (1 << (i % 32)))
        {
            ACE_printf ("E");
        }
    }
    ACE_printf (" done\n");
}


static CLI_exec_t writefl;

static void
writefl_exec (char *nix)
{
    unsigned char error;
    int i;
    // Write page with walking bit pattern (0x00000001, 0x00000002, ...)
    ACE_printf ("Writing last page with walking bit pattern\n");
    for (i = 0; i < (AT91C_IFLASH_PAGE_SIZE / 4); i++)
    {

        buffer[i] = 1 << (i % 32);
    }
    error = DEV_at91_FLASHD_write (lastPageAddress, buffer, AT91C_IFLASH_PAGE_SIZE);
    ACE_printf ("Flash programmed: err = 0x%02X\n", error);
}

extern void
flash_test_install (MFS_descriptor_t * test)
{
    // Performs tests on last page (to avoid overriding existing program).
    lastPageAddress = (unsigned int)(AT91C_IFLASH + AT91C_IFLASH_SIZE - AT91C_IFLASH_PAGE_SIZE);
    lastPageData = (volatile unsigned int *)lastPageAddress;

    CLI_exec_init (test, &lockfl, "lock_F", "Lock Flash", lockfl_exec);
    CLI_exec_init (test, &printfl, "print_F", "Print Flash", printfl_exec);
    CLI_exec_init (test, &writefl, "write_F", "Write Flash", writefl_exec);
}
