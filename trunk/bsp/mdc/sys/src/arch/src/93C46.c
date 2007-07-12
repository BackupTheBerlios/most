/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

/*
 ** low level support for 93c46b 1 kbit serial eeprom
 * 128 Bytes
 * 64 words *
 */

#include <uso/sleep.h>
#include <uso/semaphore.h>
#include <dev/arch/h8300/io_ports.h>
#include <dev/arch/h8300/bitops.h>

#include "arch/pins.h"
#include "arch/93C46.h"

#define CS_PORT   h8_P6DR
#define CS_PIN    H8_P60_CS

#define CLK_PORT  h8_P6DR
#define CLK_PIN   H8_P62_CLK

#define DI_PORT   h8_P7DR
#define DI_PIN    H8_P77_DATA

#define DO_PORT   h8_P6DR
#define DO_PIN    H8_P61_DATA


/*
 * MICROWIRE COMMANDS 
 */
#define _MW_EWDS  (0x0)
#define _MW_WRAL  (0x1)
#define _MW_ERAL  (0x2)
#define _MW_EWEN  (0x3)
#define _MW_WRITE (0x4)
#define _MW_ERASE (0x6)
#define _MW_READ  (0x8)

#define _MW_93C46(c) ((c)<<4)

#define CLK_HIGH()      H8_BITSET(CLK_PIN,CLK_PORT)
#define CLK_LOW()       H8_BITCLEAR(CLK_PIN,CLK_PORT)
#define CLK_OUT()       CLK_LOW(); CLK_HIGH()   // !
#define CHIP_STARTBIT() H8_BITSET(DO_PIN,DO_PORT); CLK_OUT()       // !
#define CHIP_SELECT()   CLK_LOW(); H8_BITSET(CS_PIN,CS_PORT)       // !
#define CHIP_RELEASE()  CLK_LOW(); H8_BITCLEAR(CS_PIN,CS_PORT); H8_BITCLEAR(DO_PIN,DO_PORT)   // !
#define EE_BUSY()       ((DI_PORT & DI_PIN) == 0)


static USO_semaphore_t mutex;

static void
CHIP_READY (void)
{
    /*
     * do not use polling! missing pull up! 
     */
    USO_sleep (ACE_MSEC_2_TICKS(20));
}


static void
EE93c46_command (unsigned int command)
{
    unsigned int i;

    H8_BITSET (DO_PIN, DO_PORT);   /* startbit */
    CLK_OUT ();

    for (i = 0x80; i; i >>= 1)
    {
        if (command & i)
        {
            H8_BITSET (DO_PIN, DO_PORT);
        }
        else
        {
            H8_BITCLEAR (DO_PIN, DO_PORT);
        }
        CLK_OUT ();
    }
}

/*
 ** Enable Write Mode
 */
static void
EE93c46_ewen (void)
{
    CHIP_SELECT ();

    EE93c46_command (_MW_93C46 (_MW_EWEN));

    CHIP_RELEASE ();
}

/*
 ** Disable Write Mode
 */
static void
EE93c46_ewds (void)
{
    CHIP_SELECT ();

    EE93c46_command (_MW_93C46 (_MW_EWDS));

    CHIP_RELEASE ();
}

static void
EE93c46_write (unsigned int addr, unsigned short data)
{
    char i;

    CHIP_SELECT ();

    EE93c46_command ((addr & 0x3f) | _MW_93C46 (_MW_WRITE));

    for (i = 0; i < 16; i++, data <<= 1)
    {
        if (data & 0x8000)
            H8_BITSET (DO_PIN, DO_PORT);
        else
            H8_BITCLEAR (DO_PIN, DO_PORT);
        CLK_OUT ();
    }

    CHIP_RELEASE ();

    CHIP_READY ();
}

static unsigned short
EE93c46_read (unsigned int addr)
{
    char i;
    unsigned short data = 0;

    CHIP_SELECT ();

    EE93c46_command ((addr & 0x3f) | _MW_93C46 (_MW_READ));

    CLK_LOW ();

    for (i = 0; i < 16; i++)
    {
        data <<= 1;
        CLK_HIGH ();
        data |= (DI_PORT & DI_PIN) ? 1 : 0;
        CLK_LOW ();
    }
    CHIP_RELEASE ();

    return (data);
}

// Address MUST BE word aligned!!!!
static unsigned int
EE93c46_multipleread (unsigned short *rp, unsigned int addr, unsigned int len)
{
    unsigned int i;

    for (i = len; i; i--, rp++, addr++)
    {
        *rp = EE93c46_read (addr);
    }

    return (len);
}

// Address MUST BE word aligned!!!!
static void
EE93c46_multiplewrite (unsigned short *wp, unsigned int addr, unsigned int len)
{
    unsigned int i;

    EE93c46_ewen ();
    for (i = len; i; i--, wp++, addr++)
    {
        EE93c46_write (addr, *wp);
    }
    EE93c46_ewds ();
}



extern void
EE_93C46_init (void)
{
    USO_semaphore_init (&mutex, 1);
}

extern void
EE_93C46_eraseall (void)
{
    USO_wait (&mutex);

    EE93c46_ewen ();

    CHIP_SELECT ();

    EE93c46_command (_MW_93C46 (_MW_ERAL));

    CHIP_RELEASE ();

    CHIP_READY ();

    EE93c46_ewds ();

    USO_signal (&mutex);
}

extern void
EE_93C46_write (unsigned int addr, unsigned int len, void *data)
{
    USO_wait (&mutex);

    EE93c46_multiplewrite ((unsigned short *)data, addr / 2, len / 2);

    USO_signal (&mutex);
}

extern unsigned int
EE_93C46_read (unsigned int addr, unsigned int len, void *data)
{
    unsigned int l;

    USO_wait (&mutex);

    l = EE93c46_multipleread ((unsigned short *)data, addr / 2, len / 2);

    USO_signal (&mutex);

    return l;
}
