/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

#include <ace/stdio.h>
#include <cli/commands.h>

#include "arch/93C46.h"
#include "ee_test.h"

static CLI_exec_t eraseEE;

static void
eraseEE_exec (char *nix)
{
    EE_93C46_eraseall ();
}


static CLI_exec_t write1EE;

static char name1[] = "AABBCCDDEEFFU";

static void
write1EE_exec (char *nix)
{
    EE_93C46_write (0, sizeof (name1), name1);
}


static CLI_exec_t write2EE;

static char name2[] = "MMIICCHHAAEEU";

static void
write2EE_exec (char *nix)
{
    EE_93C46_write (0, sizeof (name2), name2);
}

static CLI_exec_t readEE;

static char name[32];

static void
readEE_exec (char *nix)
{
    EE_93C46_read (0, 14, name);
    name[31] = 0;
    ACE_printf ("%s.\n", name);

}

extern void
ee_test_install(MFS_descriptor_t *test){
    CLI_exec_init (test, &eraseEE, "eraseE", "Erase EEprom", eraseEE_exec);

    CLI_exec_init (test, &write1EE, "write1E", "Write String 1 to EEprom",
                      write1EE_exec);

    CLI_exec_init (test, &write2EE, "write2E", "Write String 2 to EEprom",
                      write2EE_exec);

    CLI_exec_init (test, &readEE, "readE", "Read EEprom", readEE_exec);
}
