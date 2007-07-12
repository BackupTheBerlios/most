/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

#include <ace/string.h>

#include "dpy/gu256x64ser/display.h"
#include "dpy/gu256x64ser/anim.h"
#include "dpy/gu256x64ser/action.h"
#include "dpy/gu256x64ser/font.h"
#include "dpy/gu256x64ser/private.h"

#define BRIGHTNESS '\x04'

extern void
DPY_gu256x64ser_Initialize (void)
{
    DPY_gu256x64ser_InitializeDisplay ();
    DPY_gu256x64ser_BrightnessLevel (BRIGHTNESS);
    DPY_gu256x64ser_CharCommand (CLR);
    DPY_gu256x64ser_FontSizeSelect (2);
    DPY_gu256x64ser_FontMagnifiedDisplay (1, 2);
    DPY_gu256x64ser_CharBoldDisplay (0);
}

static void
gu256x64ser_Center (const char *text, int hide)
{
    int space, rest;
    int maxSize = 32;
    int size = strlen (text);
    if (size > maxSize)
        size = maxSize;
    space = (maxSize - size) / 2;
    rest = maxSize - space;
    DPY_gu256x64ser_CharCommand (CLR);
    DPY_gu256x64ser_CursorSet (space * 8 + hide * 256, 2);
    while ((DPY_gu256x64ser_CharCommand (*(const unsigned char *)text++)) && --rest);

}

extern void
DPY_gu256x64ser_Standard (const char *text)
{
    gu256x64ser_Center (text, 0);
    DPY_gu256x64ser_Wait (4);
}

extern void
DPY_gu256x64ser_Travel (const char *text)
{
    gu256x64ser_Center (text, 1);
    DPY_gu256x64ser_Scroll ((unsigned short)2 * 8, (unsigned short)0x80,
                      (unsigned char)1);
    DPY_gu256x64ser_Scroll ((unsigned short)2 * 8, (unsigned short)0x80,
                      (unsigned char)1);
}

extern void
DPY_gu256x64ser_Strobe (const char *text)
{
    gu256x64ser_Center (text, 0);
    DPY_gu256x64ser_Blink (1, 5, 5, 10);
}

extern void
DPY_gu256x64ser_Snow (const char *text)
{
    gu256x64ser_Center (text, 1);
    DPY_gu256x64ser_Random (2, (unsigned short)0x0800);
}
