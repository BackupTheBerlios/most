#include <ace/string.h>
#include <ace/stdlib.h>
#include <uso/log.h>
#include <dev/mmc.h>
#include <cli/commands.h>

#include "arch/spi.h"
#include "mmc_test.h"

static char *mmc_buffer_test_1;
static char *mmc_buffer_test_2;

static CLI_exec_t mmc_test;

static void mmc_test_exec(char* nix)
{
	int err = FALSE;
	if (DEV_mmc_spi_init((DEV_spi_dev_t *)&SAM_mmc) < 0)
	{
	    USO_kputs (USO_LL_ERROR, "MMC spi init failed!\n");
		return;
	}

	mmc_buffer_test_1 = ACE_malloc(512);
	mmc_buffer_test_2 = ACE_malloc(512);

	if (mmc_buffer_test_1 == NULL || mmc_buffer_test_2 == NULL){
	    USO_kputs (USO_LL_ERROR, "TEST MMC out of mem!\n");
	    return;
	}
	// variable init
	memset(mmc_buffer_test_1,'A',512);
	memset(mmc_buffer_test_2,'B',512);

	if (DEV_mmc_init() != MMC_SUCCESS) {
	    USO_kputs (USO_LL_ERROR, "MMC card not found!\n");
		return;
	}

	memset(&mmc_buffer,0,512);

	DEV_mmc_read_register (10, 16);
	mmc_buffer[7]=0;

	// Fill first Block (0) with 'A'
	memset(&mmc_buffer,'A',512);    //set breakpoint and trace mmc_buffer contents
	DEV_mmc_write_block(0);
	// Fill second Block (1)-AbsAddr 512 with 'B'
	memset(&mmc_buffer,'B',512);
	DEV_mmc_write_block(512);

	// Read first Block back to buffer
	memset(&mmc_buffer,'\0',512);
	DEV_mmc_read_block(0,512);

	if(ACE_strncmp(&mmc_buffer[0], mmc_buffer_test_1, 512))
		err=TRUE;

	// Read second Block back to buffer
	memset(&mmc_buffer,'\0',512);
	DEV_mmc_read_block(512,512);

	if(ACE_strncmp(&mmc_buffer[0], mmc_buffer_test_2, 512))
		err=TRUE;

	memset(&mmc_buffer,'\0',512);

	if(err == FALSE) {
	    USO_kputs (USO_LL_INFO, "TEST MMC OK!\n");
	}
	else {
	    USO_kputs (USO_LL_INFO, "TEST MMC FAILED!\n");
	}

	ACE_free (mmc_buffer_test_1);
	ACE_free (mmc_buffer_test_2);

}

extern void
mmc_test_install(MFS_descriptor_t *test)
{
    CLI_exec_init (test, &mmc_test, "mmc_T", "MMC Test", mmc_test_exec);
}
