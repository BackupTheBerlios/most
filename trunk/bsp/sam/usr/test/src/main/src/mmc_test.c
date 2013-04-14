#include <ace/string.h>
#include <ace/stdlib.h>
#include <cli/exec.h>

#include <mfs/sysfs.h>
#include <mfs/directory.h>
#include <mfs/block.h>
#include <mmc_test.h>


static CLI_exec_t mmc_test;

static void
mmc_test_exec (char *nix)
{
    int err = FALSE;
    char *buffer_A;
    char *buffer_B;
    char *data = NULL;

	MFS_descriptor_t *desc = MFS_open (MFS_resolve(MFS_get_root(), "bsp/mmc"), "d1_p2");
	MFS_block_t *p2 = (MFS_block_t *)desc;

	if (desc == NULL || desc->type != MFS_BLOCK)
    	return;

    buffer_A = ACE_malloc (p2->size);
    buffer_B = ACE_malloc (p2->size);

    if (buffer_A == NULL || buffer_B == NULL)
    {
        if (buffer_A != NULL)
        	ACE_free (buffer_A);
        if (buffer_B != NULL)
        	ACE_free (buffer_B);
        ACE_puts ("TEST MMC out of mem!\n");
        return;
    }

    // Fill first Block (0) with 'A'
    memset (buffer_A, 'A',  p2->size);
    MFS_put  (p2, buffer_A, p2->size, p2->start);

    // Fill second Block (1) with 'B'
    memset (buffer_B, 'B',  p2->size);
    MFS_put  (p2, buffer_B, p2->size, p2->start + 1);

    // Read first Block
	MFS_get (p2, &data, p2->start);

    if (ACE_strncmp (buffer_A, data, p2->size))
        err = TRUE;

	MFS_confirm(p2, p2->start);

    // Read second Block
	MFS_get (p2, &data, p2->start + 1);

    if (ACE_strncmp (buffer_B, data, p2->size))
        err = TRUE;

	MFS_confirm(p2, p2->start + 1);

    if (err == FALSE)
    {
        ACE_puts ("TEST MMC OK!\n");
    }
    else
    {
        ACE_puts ("TEST MMC FAILED!\n");
    }

    ACE_free (buffer_A);
    ACE_free (buffer_B);
	MFS_close_desc (desc);

}


extern void
mmc_test_install (MFS_descriptor_t * test)
{
    CLI_exec_init (test, &mmc_test, "mmc_T", "MMC Test", mmc_test_exec);
}
