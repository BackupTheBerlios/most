/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

#include <ace/stdio.h>
#include <cli/exec.h>

#include <dev/chips/flash_29F040.h>
#include <flash_test.h>

static volatile unsigned char *const sector = (volatile unsigned char *)0x50000;

static CLI_exec_t erasefl;

static void
erasefl_exec (char *nix)
{
    enum FLASH_29F040_err_code err;
    err = FLASH_29F040_sector_erase_s (sector);
    ACE_printf ("Flash erased = %d.\n", err);
}

static CLI_exec_t printfl;

static void
printfl_exec (char *nix)
{
    int i;
    volatile unsigned char *data;
    for (i = 8, data = sector; i; --i, data += 8)
    {
        ACE_printf
            ("%2x %02x %02x %02x %02x %02x %02x %02x %c%c%c%c%c%c%c%c.\n",
             *data, *(data + 1), *(data + 2), *(data + 3), *(data + 4),
             *(data + 5), *(data + 6), *(data + 7), *data, *(data + 1),
             *(data + 2), *(data + 3), *(data + 4), *(data + 5), *(data + 6), *(data + 7));
    }
}

static unsigned char fldata[] = "Dieser String dient ausschlieslich zum Testen der Flash Routinen!";

static CLI_exec_t writefl;

static void
writefl_exec (char *nix)
{
    enum FLASH_29F040_err_code err;
    err = FLASH_29F040_programm_s (sector, fldata, sizeof (fldata));
    ACE_printf ("Flash programmed = %d.\n", err);
}

extern void
flash_test_install (MFS_descriptor_t * test)
{
    CLI_exec_init (test, &erasefl, "erase_F", "Erase Flash", erasefl_exec);

    CLI_exec_init (test, &printfl, "print_F", "Print Flash", printfl_exec);

    CLI_exec_init (test, &writefl, "write_F", "Write Flash", writefl_exec);
}
