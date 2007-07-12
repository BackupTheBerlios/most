/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

#include "dpy/gu256x64ser/display.h"
#include "dpy/gu256x64ser/font.h"
#include "dpy/gu256x64ser/private.h"

/*
 * Font command group 
 */

static unsigned char FontSizeSelectData[] = { US, '(', 'g', '\x01', _m };

extern void
DPY_gu256x64ser_FontSizeSelect (unsigned char fontSize)
{
    FontSizeSelectData[4] = fontSize;
    DPY_gu256x64ser_WriteArray2Display (FontSizeSelectData, sizeof (FontSizeSelectData));
}

static unsigned char ModeData16x16dot[] = { US, '(', 'g', '\x02', _m };

extern void
DPY_gu256x64ser_16x16dotMode (unsigned char specify)
{
    ModeData16x16dot[4] = specify;
    DPY_gu256x64ser_WriteArray2Display (ModeData16x16dot, sizeof (ModeData16x16dot));
}

static unsigned char CharDefinition16x16Data[] =
    { US, '(', 'g', '\x10', _c1, _c2 };

extern void
DPY_gu256x64ser_16x16CharDefinition (unsigned short charCode, unsigned char *data)
{
    CharDefinition16x16Data[4] = (unsigned char)charCode;
    CharDefinition16x16Data[5] = (unsigned char)(charCode >> 8);
    DPY_gu256x64ser_WriteArray2Display (CharDefinition16x16Data,
                        sizeof (CharDefinition16x16Data));
    DPY_gu256x64ser_WriteArray2Display (data, 2 * 16);
}

static unsigned char Cancel16x16DownloadedCharData[] =
    { US, '(', 'g', '\x11', _c1, _c2 };

extern void
DPY_gu256x64ser_Cancel16x16DownloadedChar (unsigned short charCode)
{
    Cancel16x16DownloadedCharData[4] = (unsigned char)charCode;
    Cancel16x16DownloadedCharData[5] = (unsigned char)(charCode >> 8);
    DPY_gu256x64ser_WriteArray2Display (Cancel16x16DownloadedCharData,
                        sizeof (Cancel16x16DownloadedCharData));
}

static unsigned char FontMagnifiedDisplayData[] =
    { US, '(', 'g', '\x40', _x, _y };

extern void
DPY_gu256x64ser_FontMagnifiedDisplay (unsigned char x, unsigned char y)
{
    FontMagnifiedDisplayData[4] = x;
    FontMagnifiedDisplayData[5] = y;
    DPY_gu256x64ser_WriteArray2Display (FontMagnifiedDisplayData,
                        sizeof (FontMagnifiedDisplayData));
}

static unsigned char CharBoldDisplayData[] = { US, '(', 'g', '\x41', _b };

extern void
DPY_gu256x64ser_CharBoldDisplay (unsigned char bold)
{
    CharBoldDisplayData[4] = bold;
    DPY_gu256x64ser_WriteArray2Display (CharBoldDisplayData, sizeof (CharBoldDisplayData));
}
