/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

#include "dpy/gu256x64ser/display.h"
#include "dpy/gu256x64ser/memory.h"
#include "dpy/gu256x64ser/private.h"

/*
 * Memory re-write command group 
 */

static const unsigned char MemoryRewriteModeData[] =
    { FS, '|', 'M', '\xD0', 'M', 'O', 'D', 'E', 'I', 'N' };

extern void
DPY_gu256x64ser_MemoryRewriteMode (void)
{
    DPY_gu256x64ser_WriteArray2Display (MemoryRewriteModeData, sizeof (MemoryRewriteModeData));
}
