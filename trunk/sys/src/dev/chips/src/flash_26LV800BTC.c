/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

/*
 ** flash programming functions for AMD 29F040
 **
 ** NOTE: IRQs must de DISABLED before using any of the functions!
 ** 
 ** Der flash code darf nicht im rom stehen, da eine programmausführung
 ** während das flash programmiert oder gelöscht wird nicht möglich ist.
 ** Aus diesem grund müssen auch alle interrupts gesperrt sein.
 **
 */

#include <ace/stddef.h>
#include <ace/string.h>
#include <ace/stdlib.h>
#include <uso/cpu.h>
#include <uso/debug.h>
#include <dev/chips/flash_26LV800BTC.h>


#define BASE_ADDR    ((volatile unsigned short*)(0x40000000))

#define ADI_MANUFACTURE   0x0
#define ADI_DEVICE        0x1

#define MANUFACTURE_CODE  0xC2
#define DEVICE_CODE_TOP       0x22DA
#define DEVICE_CODE_BOTTOM    0x225B

#define HXXX         (0x0)
#define H555         (0x555)
#define H2AA         (0x2AA)

#define CMD_55H      0x55
#define CMD_AAH      0xAA

#define CMD_RESET    0xF0

#define CMD_INFO     0x90

#define CMD_PROGRAM  0xA0

#define CMD_ERASE    0x80
#define CMD_CHIP     0x10
#define CMD_SECTOR   0x30

#define Q7           0x80
#define Q6           0x40
#define Q5           0x20


#define SECTOR_SIZE 0x10000
#define FLASH_SIZE (16 * SECTOR_SIZE)

#define WATCHDOG()  if (watchdog != NULL) watchdog()

static void (*watchdog) (void) = NULL;

extern void FLASH_26LV800BTC_set_wtd_trigger (void (*watchdog_trigger) (void))
{
    watchdog = watchdog_trigger;
}

extern unsigned long
FLASH_26LV800BTC_segment_size (void)
{
    return SECTOR_SIZE;
}


static unsigned short
read (volatile unsigned short *addr)
ACE_SECTION_(".ramcode");

ACE_INLINE_  static unsigned short
read (volatile unsigned short *addr)
{
    return *addr;
}

static void
write (volatile unsigned short *addr, unsigned short data)
ACE_SECTION_(".ramcode");

ACE_INLINE_ static void
write (volatile unsigned short *addr, unsigned short data)
{
    *addr = data;
}

static void
write_command (volatile unsigned short *addr, unsigned short cmd)
ACE_SECTION_(".ramcode");

ACE_INLINE_  static void
write_command (volatile unsigned short *addr, unsigned short cmd)
{
    write (BASE_ADDR + H555, CMD_AAH);
    write (BASE_ADDR + H2AA, CMD_55H);
    write (addr, cmd);
}

static void
reset (void)
ACE_SECTION_(".ramcode");

ACE_INLINE_ static void
reset (void)
{
    write (BASE_ADDR + HXXX, CMD_RESET);
}

static unsigned short
silicon_id (int adi)
ACE_SECTION_(".ramcode");

ACE_INLINE_ static unsigned short
silicon_id (int adi)
{
    write_command (BASE_ADDR + H555, CMD_INFO);
    return read (BASE_ADDR + adi);
}

#if 0
static void
chip_erase (void)
ACE_SECTION_(".ramcode");

ACE_INLINE_ static void
chip_erase (void)
{
    write_command (BASE_ADDR + H555, CMD_ERASE);
    write_command (BASE_ADDR + H555, CMD_CHIP);
}
#endif

static void
sector_erase (volatile unsigned short *sector_addr)
ACE_SECTION_(".ramcode");

ACE_INLINE_ static void
sector_erase (volatile unsigned short *sector_addr)
{
    write_command (BASE_ADDR + H555, CMD_ERASE);
    write_command (sector_addr, CMD_SECTOR);
}

static void
program (volatile unsigned short *addr, unsigned short data)
ACE_SECTION_(".ramcode");

ACE_INLINE_ static void
program (volatile unsigned short *addr, unsigned short data)
{
    write_command (BASE_ADDR + H555, CMD_PROGRAM);
    write (addr, data);
}

static ACE_err_t
poll (volatile unsigned short *addr, unsigned short data)
ACE_SECTION_(".ramcode");

static ACE_err_t
poll (volatile unsigned short *addr, unsigned short data)
{
    unsigned short tmp;
    ACE_err_t err = ACE_OK;

    for (;;)
    {
        tmp = read (addr);
        if ((data & Q7) == (tmp & Q7))
        {
            break;
        }
        if (tmp & Q5)
        {
            reset ();
            err = DEV_ERR_TIMEOUT;
            break;
        }
        WATCHDOG ();
    }
    return err;
}

/* this function must be extern to be in the ramcode section (run build)! */
extern ACE_err_t
ACE_LONG_CALL_ FLASH_26LV800BTC_sector_erase_ns (volatile unsigned short *sector_addr)
ACE_SECTION_(".ramcode");

extern ACE_err_t
ACE_LONG_CALL_ FLASH_26LV800BTC_sector_erase_ns (volatile unsigned short *sector_addr)
{
    ACE_err_t err = DEV_ERR_ADDR;
    if (((unsigned long)sector_addr)%2)
    {
        return err;
    }
    if (sector_addr < (BASE_ADDR))
    {
        return err;
    }
    if (sector_addr >= (BASE_ADDR + FLASH_SIZE/2))
    {
        return DEV_ERR_LEN;
    }
    sector_erase (sector_addr);
    err = poll (sector_addr, 0xFFFF);
    return err;
}

extern ACE_err_t
FLASH_26LV800BTC_sector_erase_is (volatile unsigned short *sector_addr)
{
    ACE_err_t err;
    USO_cpu_status_t ps;
    ps = USO_disable ();
    err = FLASH_26LV800BTC_sector_erase_ns (sector_addr);
    USO_restore (ps);
    return err;
}

static void *
FLASH_26LV800BTC_memcpy (void *s, const void *ct, ACE_size_t n)
ACE_SECTION_(".ramcode");

ACE_INLINE_ static void *
FLASH_26LV800BTC_memcpy (void *s, const void *ct, ACE_size_t n)
{
    char *p1 = (char *)s, *p2 = (char *)ct;

    while (n--)
        *p1++ = *p2++;
    return s;
}

/* this function must be extern to be in the ramcode section (run build)! */
extern ACE_err_t
ACE_LONG_CALL_ FLASH_26LV800BTC_programm_ns (volatile unsigned short *addr, const char *data, ACE_size_t len)
ACE_SECTION_(".ramcode");

extern ACE_err_t
ACE_LONG_CALL_ FLASH_26LV800BTC_programm_ns (volatile unsigned short *addr, const char *data, ACE_size_t len)
{
    ACE_err_t err = DEV_ERR_ADDR;
    if (((unsigned long)addr)%2)
    {
        return err;
    }
    if (len%2)
    {
        return err;
    }
    if (addr < (BASE_ADDR))
    {
        return err;
    }
    if ((addr + len) >= (BASE_ADDR + FLASH_SIZE/2))
    {
        return DEV_ERR_LEN;
    }
    for (; len >= 2; ++addr)
    {
        /** todo for big endian, no conversion is needed */
        /* can not use memcpy() because it is not in ramcode */
        unsigned short s;
        FLASH_26LV800BTC_memcpy (&s, data, sizeof(s));
        len -= sizeof(s);
        data += sizeof(s);
        program (addr, s);
        err = poll (addr, s);
        if (err != ACE_OK)
            break;
    }
    return err;
}

extern ACE_err_t
FLASH_26LV800BTC_programm_is (volatile unsigned short *addr, const char *data, ACE_size_t len)
{
    ACE_err_t err;
    USO_cpu_status_t ps;
    ps = USO_disable ();
    err = FLASH_26LV800BTC_programm_ns (addr, data, len);
    USO_restore (ps);
    return err;
}

/* this function must be extern to be in the ramcode section (run build)! */
extern ACE_err_t
ACE_LONG_CALL_ FLASH_26LV800BTC_get_id_ns (volatile unsigned short *mf, volatile unsigned short *device)
ACE_SECTION_(".ramcode");

extern ACE_err_t
ACE_LONG_CALL_ FLASH_26LV800BTC_get_id_ns (volatile unsigned short *mf, volatile unsigned short *device)
{
    ACE_err_t err = ACE_OK;
    *mf = silicon_id (ADI_MANUFACTURE);
    *device = silicon_id (ADI_DEVICE);
    reset ();
    return err;
}

extern ACE_err_t
FLASH_26LV800BTC_get_id_is (volatile unsigned short *mf, volatile unsigned short *device)
{
    ACE_err_t err;
    USO_cpu_status_t ps;
    ps = USO_disable ();
    err = FLASH_26LV800BTC_get_id_ns(mf, device);
    USO_restore (ps);
    return err;
}
