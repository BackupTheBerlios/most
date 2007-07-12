/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

#include "dpy/gu256x64ser/display.h"
#include "dpy/gu256x64ser/user.h"
#include "dpy/gu256x64ser/private.h"

/*
 * User set up mode command group 
 */

static const unsigned char UserSetUpModeStartData[] =
    { US, '(', 'e', '\x01', 'I', 'N' };

extern void
DPY_gu256x64ser_UserSetUpModeStart (void)
{
    DPY_gu256x64ser_WriteArray2Display (UserSetUpModeStartData,
                        sizeof (UserSetUpModeStartData));
}

static const unsigned char UserSetUpModeEndData[] =
    { US, '(', 'e', '\x02', 'O', 'U', 'T' };

extern void
DPY_gu256x64ser_UserSetUpModeEnd (void)
{
    DPY_gu256x64ser_WriteArray2Display (UserSetUpModeEndData, sizeof (UserSetUpModeEndData));
}

static unsigned char MemorySwSettingData[] = { US, '(', 'e', '\x03', _a, _b };

extern void
DPY_gu256x64ser_MemorySwSetting (unsigned char memorySw, unsigned char data)
{
    MemorySwSettingData[4] = memorySw;
    MemorySwSettingData[5] = data;
    DPY_gu256x64ser_WriteArray2Display (MemorySwSettingData, sizeof (MemorySwSettingData));
}

static unsigned char MemorySwDataSendData[] = { US, '(', 'e', '\x04', _a };

extern void
DPY_gu256x64ser_MemorySwDataSend (unsigned char data)
{
    MemorySwDataSendData[4] = data;
    DPY_gu256x64ser_WriteArray2Display (MemorySwDataSendData, sizeof (MemorySwDataSendData));
}

static unsigned char FROMImageDefinitionData[] =
    { US, '(', 'e', '\x10', _aL, _aH, _aE, _sL, _sH, _sE };

extern void
DPY_gu256x64ser_FROMImageDefinition (unsigned long address,
                               unsigned long length, unsigned char *data)
{
    FROMImageDefinitionData[4] = (unsigned char)address;
    FROMImageDefinitionData[5] = (unsigned char)(address >> 8);
    FROMImageDefinitionData[6] = (unsigned char)(address >> 16);
    FROMImageDefinitionData[7] = (unsigned char)length;
    FROMImageDefinitionData[8] = (unsigned char)(length >> 8);
    FROMImageDefinitionData[9] = (unsigned char)(length >> 16);
    DPY_gu256x64ser_WriteArray2Display (FROMImageDefinitionData,
                        sizeof (FROMImageDefinitionData));
    DPY_gu256x64ser_WriteArray2Display (data, (size_t) length);
}

static unsigned char SaveDownloadedCharData[] = { US, '(', 'e', '\x11', _a };

extern void
DPY_gu256x64ser_SaveDownloadedChar (unsigned char fontType)
{
    SaveDownloadedCharData[4] = fontType;
    DPY_gu256x64ser_WriteArray2Display (SaveDownloadedCharData,
                        sizeof (SaveDownloadedCharData));
}

static unsigned char FROMMacroDefinitionData[] =
    { US, '(', 'e', '\x12', _a, _pL, _pH, _t1, _t2 };

extern void
DPY_gu256x64ser_FROMMacroDefinition (unsigned char registration,
                               unsigned short length,
                               unsigned char displayTime,
                               unsigned char idleTime, unsigned char *data)
{
    FROMMacroDefinitionData[4] = registration;
    FROMMacroDefinitionData[5] = (unsigned char)length;
    FROMMacroDefinitionData[6] = (unsigned char)(length >> 8);
    FROMMacroDefinitionData[7] = displayTime;
    FROMMacroDefinitionData[8] = idleTime;
    DPY_gu256x64ser_WriteArray2Display (FROMMacroDefinitionData,
                        sizeof (FROMMacroDefinitionData));
    DPY_gu256x64ser_WriteArray2Display (data, (size_t) length);
}

static unsigned char DownloadCharTransferData[] = { US, '(', 'e', '\x21', _a };

extern void
DPY_gu256x64ser_DownloadCharTransfer (unsigned char fontType)
{
    DownloadCharTransferData[4] = fontType;
    DPY_gu256x64ser_WriteArray2Display (DownloadCharTransferData,
                        sizeof (DownloadCharTransferData));
}

static unsigned char DisplayStatusSendData[] =
    { US, '(', 'e', '\x40', _a, _b, _c };

extern void
DPY_gu256x64ser_DisplayStatusSendData (unsigned char status,
                                 unsigned char address, unsigned char length)
{
    DisplayStatusSendData[4] = status;
    if (status == '\x20')
    {
        DisplayStatusSendData[5] = address;
        DisplayStatusSendData[6] = length;
        DPY_gu256x64ser_WriteArray2Display (DisplayStatusSendData,
                            sizeof (DisplayStatusSendData));
    }
    else
    {
        DPY_gu256x64ser_WriteArray2Display (DisplayStatusSendData,
                            sizeof (DisplayStatusSendData) - 2);
    }
}
