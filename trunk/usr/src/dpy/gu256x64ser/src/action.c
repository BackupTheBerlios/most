/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

#include "dpy/gu256x64ser/display.h"
#include "dpy/gu256x64ser/action.h"
#include "dpy/gu256x64ser/private.h"

/*
 * Display action command group 
 */

static unsigned char WaitData[] = { US, '(', 'a', '\x01', _t };

extern void
DPY_gu256x64ser_Wait (unsigned char time)
{
    WaitData[4] = time;
    DPY_gu256x64ser_WriteArray2Display (WaitData, sizeof (WaitData));
}

static unsigned char ScrollData[] = { US, '(', 'a', '\x10', _wL, _wH, _cL, _cH, _s };

extern void
DPY_gu256x64ser_Scroll (unsigned short screenShift, unsigned short reptition, unsigned char speed)
{
    ScrollData[4] = (unsigned char)screenShift;
    ScrollData[5] = (unsigned char)(screenShift >> 8);
    ScrollData[6] = (unsigned char)reptition;
    ScrollData[7] = (unsigned char)(reptition >> 8);
    ScrollData[8] = speed;
    DPY_gu256x64ser_WriteArray2Display (ScrollData, sizeof (ScrollData));
}

static unsigned char BlinkData[] = { US, '(', 'a', '\x11', _p, _t1, _t2, _c };

extern void
DPY_gu256x64ser_Blink (unsigned char pattern,
                       unsigned char normalTime, unsigned char brankTime, unsigned char reptition)
{
    BlinkData[4] = pattern;
    BlinkData[5] = normalTime;
    BlinkData[6] = brankTime;
    BlinkData[7] = reptition;
    DPY_gu256x64ser_WriteArray2Display (BlinkData, sizeof (BlinkData));
}

static unsigned char CurtainData[] = { US, '(', 'a', '\x12', _v, _s, _p };

extern void
DPY_gu256x64ser_Curtain (unsigned char direction, unsigned char speed, unsigned char pattern)
{
    CurtainData[4] = direction;
    CurtainData[5] = speed;
    CurtainData[6] = pattern;
    DPY_gu256x64ser_WriteArray2Display (CurtainData, sizeof (CurtainData));
}

static unsigned char SpringData[] = { US, '(', 'a', '\x13', _v, _s, _pL, _pH };

extern void
DPY_gu256x64ser_Spring (unsigned char direction, unsigned char speed, unsigned short patternAddress)
{
    SpringData[4] = direction;
    SpringData[5] = speed;
    SpringData[6] = (unsigned char)patternAddress;
    SpringData[7] = (unsigned char)(patternAddress >> 8);
    DPY_gu256x64ser_WriteArray2Display (SpringData, sizeof (SpringData));
}

static unsigned char RandomData[] = { US, '(', 'a', '\x14', _s, _pL, _pH };

extern void
DPY_gu256x64ser_Random (unsigned char speed, unsigned short patternAddress)
{
    RandomData[4] = speed;
    RandomData[5] = (unsigned char)patternAddress;
    RandomData[6] = (unsigned char)(patternAddress >> 8);
    DPY_gu256x64ser_WriteArray2Display (RandomData, sizeof (RandomData));
}
