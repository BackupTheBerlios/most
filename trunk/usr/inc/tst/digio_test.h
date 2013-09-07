/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

#ifndef TST_DIGIO_TEST_H
#define TST_DIGIO_TEST_H

#include <mfs/vfs.h>
#include <dev/digin.h>
#include <dev/digout.h>

extern void
TST_digio_test_init (DEV_digin_t *in, DEV_digout_t *out[]);

extern void TST_digio_test_install (MFS_descriptor_t * test);

#endif
