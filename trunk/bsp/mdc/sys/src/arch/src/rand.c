/*
 * rand.c
 *
 *  Created on: 31.08.2013
 *      Author: maik
 */

#include <dev/cpu.h>
#include <arch/rand.h>
#include <arch/digio.h>

//                    R1      R2                          C3           R4              R5
//      +12V o-------mmm------mmm-------------------------||--------mmm-------------mmm---------------------o +5V
//                         |           R3   |       |           |            |
//                         |      ----mmm----       |           |            |----------       --------
//                         |      |              | /            |         | /          |       |    _ | 
//                         |      |---|<---------|<   T1        ----------|<   T2      --------| _//  |-----o digital random signal -> mcu pin
//                     C1  =      |      D1      | >                      | >                  |      |
//                         |      = C2              |                        |                 --------
//                         |      |                 |                        |                   IC1
//                         |      |                 |                        |
//      GND  o-----------------------------------------------------------------------------------------------o GND


// R1 = 2k2
// R2 = 4k7
// R3 = 47k
// R4 = 100k
// R5 = 2k2
// C1 = 100uF
// C2 = 100nF
// C3 = 100nF
// D1 = 8V2 BZX83
// T1 = BC547B
// T2 = BC547B
// IC1 =74HC14 Schmitt trigger

/*
 * Die Schaltung wurde von "http://www.jtxp.org/tech/xr232web.htm" übernommen, das Zufallssignal wird jedoch
 * einfach an einen Port Pin eingelesen.
 * Meine Dioden und Transistor Werte unterscheiden sich etwas vom Original, die Widerstandswerte sollten aber eingehalten
 * werden, da dadurch anscheinend die Schaltung ausbalanziert wird.
 * Das Signal am Kollektor von T1 wird durch C3 anscheinend verzerrt. Der AP von T2 liegt im unteren Bereich
 * und T2 scheint stark zu uebersteuern. Das digitale Signal nach dem Schmitt Trigger ist aber recht gut ausbalanziert,
 * der Mittelwert des Signals liegt bei mir bei 2,6V. 
 *
 * Das Rauschsignal soll eine Bandbreite von mehreren 100KHz aufweisen.
 * Somit soll die Zeit zwischen  zwei Sample mindestens 10uSec (<100 kBits/sec) betragen.
 */



extern ACE_u32_t
MDC_rand(int bits, int delay_us)
{
    ACE_u32_t r = 0;
    if (bits < 0 || bits >32){
        bits = 32;
    }
    for (int i = 0; i < bits; ++i){
        r = r << 1;
        // sampling the input needs ca 20 us , this seems to be right as
        // the HW can just do 175 cpudelay loops  per ms;
        // So we have a maximum sampling rate at 50 kBits/sec if delay = 0
        r |= DEV_digin_isset(&MDC_random_signal);
        if (delay_us){
            DEV_cpudelay(DEV_USEC_2_LOOPS(delay_us));
        }
    }
    return r;
}

