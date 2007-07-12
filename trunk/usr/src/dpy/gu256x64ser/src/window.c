/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

#include "dpy/gu256x64ser/display.h"
#include "dpy/gu256x64ser/window.h"
#include "dpy/gu256x64ser/private.h"

/*
 * Window command group 
 */

static unsigned char WriteScreenModeData[] = { US, '(', 'w', '\x10', _a };

extern void
DPY_gu256x64ser_WriteScreenMode (unsigned char mode)
{
    WriteScreenModeData[4] = mode;
    DPY_gu256x64ser_WriteArray2Display (WriteScreenModeData, sizeof (WriteScreenModeData));
}
