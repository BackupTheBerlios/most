/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

/*
 ** flash programming functions for AMD 29F040
 **
 ** NOTE: IRQs must de DISABLED before using any of the functions!
 ** 
 ** Der flash code darf nicht im rom stehen, da eine programmausf�hrung
 ** w�hrend das flash programmiert oder gel�scht wird nicht m�glich ist.
 ** Aus diesem grund m�ssen auch alle interrupts gesperrt sein.
 **
 */

#include <ace/stddef.h>
#include <uso/arch/cpu.h>

#include "arch/29F040.h"
#include "arch/cpu.h"


#define BASE_ADDR    ((volatile unsigned char*)(0x0))

#define ADI_MANUFACTURE   0x0
#define ADI_DEVICE        0x1
#define NOT_ADI           0x2

#define MANUFACTURE_CODE  0xC2
#define DEVICE_CODE       0xA4

#define UNPRODECTED       0x0
#define PROTECTED         0x1

#define HXXX         0x0
#define H555         0x555
#define H2AA         0x2AA

#define CMD_55H      0x55
#define CMD_AAH      0xAA

#define CMD_RESET    0xF0

#define CMD_INFO     0x90

#define CMD_PROGRAM  0xA0

#define CMD_ERASE    0x80
#define CMD_CHIP     0x10
#define CMD_UNLOCK   0x20
#define CMD_SECTOR   0x30

#define CMD_SUSPEND  0xB0

#define Q7           0x80
#define Q6           0x40
#define Q5           0x20


#define SECTOR_SIZE 0x10000
#define FLASH_SIZE (8 * SECTOR_SIZE)

static enum FLASH_29F040_err_code poll (volatile unsigned char *addr,
					unsigned char data)
      _SECTION_ (".ramcode");

extern unsigned long
FLASH_29F040_segment_size (void)
{
    return SECTOR_SIZE;
}

_INLINE_ static unsigned char
read (volatile unsigned char *addr)
{
    return *addr;
}

_INLINE_ static void
write (volatile unsigned char *addr, unsigned char data)
{
    *addr = data;
}

_INLINE_ static void
write_command (volatile unsigned char *addr, unsigned char cmd)
{
    write (BASE_ADDR + H555, CMD_AAH);
    write (BASE_ADDR + H2AA, CMD_55H);
    write (addr, cmd);
}

_INLINE_ static void
reset (void)
{
    write (BASE_ADDR + HXXX, CMD_RESET);
}

_INLINE_ static unsigned char
silicon_id (int adi)
{
    write_command (BASE_ADDR + H555, CMD_INFO);
    return read (BASE_ADDR + adi);
}

_INLINE_ static unsigned char
protect_verify (volatile unsigned char *sector_addr)
{
    write_command (BASE_ADDR + H555, CMD_INFO);
    return read (sector_addr + NOT_ADI);
}

_INLINE_ static void
program (volatile unsigned char *addr, unsigned char data)
{
    write_command (BASE_ADDR + H555, CMD_PROGRAM);
    write (addr, data);
}

_INLINE_ static void
chip_erase (void)
{
    write_command (BASE_ADDR + H555, CMD_ERASE);
    write_command (BASE_ADDR + H555, CMD_CHIP);
}

_INLINE_ static void
sector_erase (volatile unsigned char *sector_addr)
{
    write_command (BASE_ADDR + H555, CMD_ERASE);
    write_command (sector_addr, CMD_SECTOR);
}

_INLINE_ static void
suspend (void)
{
    write (BASE_ADDR + HXXX, CMD_SUSPEND);
}

_INLINE_ static void
resume (void)
{
    write (BASE_ADDR + HXXX, CMD_SECTOR);
}

_INLINE_ static void
un_lock (void)
{
    write_command (BASE_ADDR + H555, CMD_ERASE);
    write_command (BASE_ADDR + H555, CMD_UNLOCK);
}

static enum FLASH_29F040_err_code
poll (volatile unsigned char *addr, unsigned char data)
{
    unsigned char tmp;
    enum FLASH_29F040_err_code err = FLASH_29F040_ok;

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
            err = FLASH_29F040_timeout;
            break;
        }
        MDC_watchdog_trigger ();
    }
    return err;
}

extern enum FLASH_29F040_err_code
FLASH_29F040_sector_erase_s (volatile unsigned char *sector_addr)
{
    enum FLASH_29F040_err_code err;
    USO_cpu_status_t ps;
    ps = USO_disable ();
    err = FLASH_29F040_sector_erase_ns (sector_addr);
    USO_restore (ps);
    return err;
}

/*
 * extern enum FLASH_err_code FLASH_chip_erase(void) { enum FLASH_err_code 
 * err; USO_cpu_status_t ps = USO_disable(); chip_erase(); err =
 * poll(BASE_ADDR + HXXX, 0xFF); USO_restore(ps); return err; } 
 */

extern enum FLASH_29F040_err_code
FLASH_29F040_sector_erase_ns (volatile unsigned char *sector_addr)
{
    enum FLASH_29F040_err_code err;
    if (sector_addr < (BASE_ADDR + SECTOR_SIZE))
    {
        return FLASH_29F040_addr;
    }
    if ((unsigned long)sector_addr % SECTOR_SIZE)
    {
        return FLASH_29F040_addr;
    }
    if (sector_addr >= (BASE_ADDR + FLASH_SIZE))
    {
        return FLASH_29F040_addr;
    }
    sector_erase (sector_addr);
    err = poll (sector_addr, 0xFF);
    return err;
}

extern enum FLASH_29F040_err_code
FLASH_29F040_programm_s (volatile unsigned char *addr,
                unsigned char *data, unsigned long len)
{
    enum FLASH_29F040_err_code err;
    USO_cpu_status_t ps;
    ps = USO_disable ();
    err = FLASH_29F040_programm_ns (addr, data, len);
    USO_restore (ps);
    return err;
}

extern enum FLASH_29F040_err_code
FLASH_29F040_programm_ns (volatile unsigned char *addr,
                unsigned char *data, unsigned long len)
{
    enum FLASH_29F040_err_code err = FLASH_29F040_ok;
    if (addr < (BASE_ADDR + SECTOR_SIZE))
    {
        return FLASH_29F040_addr;
    }
    if ((addr + len) >= (BASE_ADDR + FLASH_SIZE))
    {
        return FLASH_29F040_addr;
    }
    for (; len--; ++addr, ++data)
    {
        program (addr, *data);
        err = poll (addr, *data);
        if (err != FLASH_29F040_ok)
            break;
    }
    return err;
}
