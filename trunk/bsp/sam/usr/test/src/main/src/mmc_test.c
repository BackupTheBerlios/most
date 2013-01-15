#include <ace/string.h>
#include <ace/stdlib.h>
#include <dev/mmc.h>
#include <cli/exec.h>

#include "arch/spi.h"
#include "mmc_test.h"


static CLI_exec_t mmc_test;

static void
mmc_test_exec (char *nix)
{
    int err = FALSE;
    char *buffer;
    char *buffer_test;

    buffer = ACE_malloc (DEV_MMC_BLOCK_SIZE);
    buffer_test = ACE_malloc (DEV_MMC_BLOCK_SIZE);

    if (buffer == NULL || buffer_test == NULL)
    {
        ACE_puts ("TEST MMC out of mem!\n");
        return;
    }
    // Fill first Block (0) with 'A'
    memset (buffer, 'A', DEV_MMC_BLOCK_SIZE);
    DEV_mmc_write_block (0, buffer);
    // Fill second Block (1)-AbsAddr 512 with 'B'
    memset (buffer, 'B', DEV_MMC_BLOCK_SIZE);
    DEV_mmc_write_block (1 * DEV_MMC_BLOCK_SIZE, buffer);

    // Read first Block back to buffer
    memset (buffer, '\0', DEV_MMC_BLOCK_SIZE);
    DEV_mmc_read_block (0, DEV_MMC_BLOCK_SIZE, buffer);

    memset (buffer_test, 'A', DEV_MMC_BLOCK_SIZE);

    if (ACE_strncmp (buffer, buffer_test, DEV_MMC_BLOCK_SIZE))
        err = TRUE;

    // Read second Block back to buffer
    memset (buffer, '\0', DEV_MMC_BLOCK_SIZE);
    DEV_mmc_read_block (1 * DEV_MMC_BLOCK_SIZE, DEV_MMC_BLOCK_SIZE, buffer);

    memset (buffer_test, 'B', DEV_MMC_BLOCK_SIZE);

    if (ACE_strncmp (buffer, buffer_test, DEV_MMC_BLOCK_SIZE))
        err = TRUE;

    if (err == FALSE)
    {
        ACE_puts ("TEST MMC OK!\n");
    }
    else
    {
        ACE_puts ("TEST MMC FAILED!\n");
    }

    ACE_free (buffer);
    ACE_free (buffer_test);

}


extern void
mmc_test_install (MFS_descriptor_t * test)
{
    CLI_exec_init (test, &mmc_test, "mmc_T", "MMC Test", mmc_test_exec);
}
