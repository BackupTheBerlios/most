/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

#include "dpy/gu256x64ser/display.h"
#include "dpy/gu256x64ser/private.h"

extern void
DPY_gu256x64ser_WriteArray2Display (const unsigned char *data, ACE_size_t len)
{
    while (len--)
        DPY_gu256x64ser_WriteChar2Display (*data++);
}

extern int
DPY_gu256x64ser_CharCommand (const unsigned char command)
{
    if ((command >= BS && command <= CR) || (command >= '\x20'))
    {
        DPY_gu256x64ser_WriteChar2Display (command);
        return 1;
    }
    else
        return 0;
}


static unsigned char CursorSetData[] = { US, '$', _xL, _xH, _yL, _yH };

extern void
DPY_gu256x64ser_CursorSet (unsigned short x, unsigned short y)
{
    CursorSetData[2] = (unsigned char)x;
    CursorSetData[3] = (unsigned char)(x >> 8);
    CursorSetData[4] = (unsigned char)y;
    CursorSetData[5] = (unsigned char)(y >> 8);
    DPY_gu256x64ser_WriteArray2Display (CursorSetData, sizeof (CursorSetData));
}

static const unsigned char InitializeDisplayData[] = { ESC, '@' };

extern void
DPY_gu256x64ser_InitializeDisplay (void)
{
    DPY_gu256x64ser_WriteArray2Display (InitializeDisplayData, sizeof (InitializeDisplayData));
}

static unsigned char DownloadRegisterData[] = { ESC, '%', _n };

extern void
DPY_gu256x64ser_DownloadRegister (unsigned char enable)
{
    DownloadRegisterData[2] = enable;
    DPY_gu256x64ser_WriteArray2Display (DownloadRegisterData, sizeof (DownloadRegisterData));
}

static unsigned char DownloadCharDefinitionData[] = { ESC, '&', _a, _c1, _c2 };

extern void
DPY_gu256x64ser_DownloadCharDefinition (unsigned char type,
                                  unsigned char start,
                                  unsigned char end, unsigned char *data)
{
    unsigned char size, x;
    if (type == 1)
        size = 6;
    else if (type == 2)
        size = 8;
    else
        return;
    DownloadCharDefinitionData[2] = type;
    DownloadCharDefinitionData[3] = start;
    DownloadCharDefinitionData[4] = end;
    DPY_gu256x64ser_WriteArray2Display (DownloadCharDefinitionData,
                        sizeof (DownloadCharDefinitionData));
    for (x = 1; x <= size; ++x)
    {
        DPY_gu256x64ser_WriteChar2Display (x);
        DPY_gu256x64ser_WriteChar2Display (*data++);
    }
}

static unsigned char DeleteDownloadedCharData[] = { ESC, '?', _a, _c };

extern void
DPY_gu256x64ser_DeleteDownloadedChar (unsigned char select, unsigned char code)
{
    DeleteDownloadedCharData[2] = select;
    DeleteDownloadedCharData[3] = code;
    DPY_gu256x64ser_WriteArray2Display (DeleteDownloadedCharData,
                        sizeof (DeleteDownloadedCharData));
}

static unsigned char InternalFontSetData[] = { ESC, 'R', _n };

extern void
DPY_gu256x64ser_InternalFontSet (unsigned char fontSet)
{
    InternalFontSetData[2] = fontSet;
    DPY_gu256x64ser_WriteArray2Display (InternalFontSetData, sizeof (InternalFontSetData));
}

static unsigned char CharCodeTypeData[] = { ESC, 't', _n };

extern void
DPY_gu256x64ser_CharCodeType (unsigned char codeType)
{
    CharCodeTypeData[2] = codeType;
    DPY_gu256x64ser_WriteArray2Display (CharCodeTypeData, sizeof (CharCodeTypeData));
}

static const unsigned char OverwriteModeData[] = { US, MD1 };

extern void
DPY_gu256x64ser_OverwriteMode (void)
{
    DPY_gu256x64ser_WriteArray2Display (OverwriteModeData, sizeof (OverwriteModeData));
}

static const unsigned char VerticalScrollModeData[] = { US, MD2 };

extern void
DPY_gu256x64ser_VerticalScrollMode (void)
{
    DPY_gu256x64ser_WriteArray2Display (VerticalScrollModeData,
                        sizeof (VerticalScrollModeData));
}

static const unsigned char HorizontalScrollModeData[] = { US, MD3 };

extern void
DPY_gu256x64ser_HorizontalScrollMode (void)
{
    DPY_gu256x64ser_WriteArray2Display (HorizontalScrollModeData,
                        sizeof (HorizontalScrollModeData));
}

static unsigned char HorizontalScrollSpeedData[] = { US, 's', _n };

extern void
DPY_gu256x64ser_HorizontalScrollSpeed (unsigned char speed)
{
    HorizontalScrollSpeedData[2] = speed;
    DPY_gu256x64ser_WriteArray2Display (HorizontalScrollSpeedData,
                        sizeof (HorizontalScrollSpeedData));
}

static unsigned char ReverseDisplayData[] = { US, 'r', _n };

extern void
DPY_gu256x64ser_ReverseDisplay (unsigned char mode)
{
    ReverseDisplayData[2] = mode;
    DPY_gu256x64ser_WriteArray2Display (ReverseDisplayData, sizeof (ReverseDisplayData));
}

static unsigned char MixtureDisplayModeData[] = { US, 'w', _n };

extern void
DPY_gu256x64ser_MixtureDisplayMode (unsigned char mode)
{
    MixtureDisplayModeData[2] = mode;
    DPY_gu256x64ser_WriteArray2Display (MixtureDisplayModeData,
                        sizeof (MixtureDisplayModeData));
}

static unsigned char BrightnessLevelData[] = { US, 'X', _n };

extern void
DPY_gu256x64ser_BrightnessLevel (unsigned char level)
{
    BrightnessLevelData[2] = level;
    DPY_gu256x64ser_WriteArray2Display (BrightnessLevelData, sizeof (BrightnessLevelData));
}

static unsigned char RAMMacroDefinitionData[] = { US, ':', _pL, _pH };

extern void
DPY_gu256x64ser_RAMMacroDefinition (unsigned short length, unsigned char *data)
{
    RAMMacroDefinitionData[2] = (unsigned char)length;
    RAMMacroDefinitionData[3] = (unsigned char)(length >> 8);
    DPY_gu256x64ser_WriteArray2Display (RAMMacroDefinitionData,
                        sizeof (RAMMacroDefinitionData));
    DPY_gu256x64ser_WriteArray2Display (data, (ACE_size_t) length);
}

static unsigned char MacroExecutionData[] = { US, '^', _n, _t1, _t2 };

extern void
DPY_gu256x64ser_MacroExecution (unsigned char definitionNr,
                          unsigned char displayTime, unsigned char idleTime)
{
    MacroExecutionData[2] = definitionNr;
    MacroExecutionData[3] = displayTime;
    MacroExecutionData[4] = idleTime;
    DPY_gu256x64ser_WriteArray2Display (MacroExecutionData, sizeof (MacroExecutionData));
}
