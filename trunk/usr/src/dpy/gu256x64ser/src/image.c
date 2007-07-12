/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

#include "dpy/gu256x64ser/display.h"
#include "dpy/gu256x64ser/image.h"
#include "dpy/gu256x64ser/private.h"

/*
 * Bit image display group 
 */

static unsigned char RAMImageDefinitionData[] =
    { US, '(', 'f', '\x01', _aL, _aH, _aE, _sL, _sH, _sE };

extern void
DPY_gu256x64ser_RAMImageDefinition (unsigned long address,
                              unsigned long length, unsigned char *data)
{
    RAMImageDefinitionData[4] = (unsigned char)address;
    RAMImageDefinitionData[5] = (unsigned char)(address >> 8);
    RAMImageDefinitionData[6] = (unsigned char)(address >> 16);
    RAMImageDefinitionData[7] = (unsigned char)length;
    RAMImageDefinitionData[8] = (unsigned char)(length >> 8);
    RAMImageDefinitionData[9] = (unsigned char)(length >> 16);
    DPY_gu256x64ser_WriteArray2Display (RAMImageDefinitionData,
                        sizeof (RAMImageDefinitionData));
    DPY_gu256x64ser_WriteArray2Display (data, (size_t) length);
}

static unsigned char DownloadImageDisplayData[] =
    { US, '(', 'f', '\x10', _m, _aL, _aH, _aE, _ySL, _ySH, _xL, _xH, _yL,
    _yH, '\x01'
};

extern void
DPY_gu256x64ser_DownloadImageDisplay (unsigned char displayMemory,
                                unsigned long imageAddress,
                                unsigned short image,
                                unsigned short x, unsigned short y)
{
    DownloadImageDisplayData[4] = displayMemory;
    DownloadImageDisplayData[5] = (unsigned char)imageAddress;
    DownloadImageDisplayData[6] = (unsigned char)(imageAddress >> 8);
    DownloadImageDisplayData[7] = (unsigned char)(imageAddress >> 16);
    DownloadImageDisplayData[8] = (unsigned char)image;
    DownloadImageDisplayData[9] = (unsigned char)(image >> 8);
    DownloadImageDisplayData[10] = (unsigned char)x;
    DownloadImageDisplayData[11] = (unsigned char)(x >> 8);
    DownloadImageDisplayData[12] = (unsigned char)y;
    DownloadImageDisplayData[13] = (unsigned char)(y >> 8);
    DPY_gu256x64ser_WriteArray2Display (DownloadImageDisplayData,
                        sizeof (DownloadImageDisplayData));
}

static unsigned char RealTimeImageDisplayData[] =
    { US, '(', 'f', '\x11', _xL, _xH, _yL, _yH, '\x01' };

extern void
DPY_gu256x64ser_RealTimeImageDisplay (unsigned short x,
                                unsigned short y, unsigned char *data)
{
    RealTimeImageDisplayData[4] = (unsigned char)x;
    RealTimeImageDisplayData[5] = (unsigned char)(x >> 8);
    RealTimeImageDisplayData[6] = (unsigned char)y;
    RealTimeImageDisplayData[7] = (unsigned char)(y >> 8);
    DPY_gu256x64ser_WriteArray2Display (RealTimeImageDisplayData,
                        sizeof (RealTimeImageDisplayData));
    DPY_gu256x64ser_WriteArray2Display (data, (size_t) (x * y));
}
