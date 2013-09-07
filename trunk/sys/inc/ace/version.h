#ifndef ACE_VERSION_H
#define ACE_VERSION_H

/** @addtogroup ace
 *
 * @{
 */


/** @defgroup version version.h
 *
 * @{
 */

/** This is the version for MOST.
 *  major.minor.bugfix-(d-counter|t-counter,r)
 *  d-counter...marks a developer version: (debugging allowed)
 *  	for each commit the counter must be incremented starting with one,
 *      best you increment the counter immediate after the commit.
 *  t-counter...marks a testing version: (no debugging)
 *      a testing version is created by branching a developer version replacing d with t
 *      for each commit the counter must be incremented,
 *      after creating the branch the developer version minor number on the main line must be incremented by 1
 *      and counter on main line reset to 1.
 *      (for compile with debug info, replace t with d)
 *  r...marks a release version: (no debugging)
 *      only testing versions are allowed to became a release version.
 *      only bugfix commits are allowed incrementing the bugfix number.
 *      each release version must have a CVS label with the version name
 *      for building the release check out the testing version, do not build the release from
 *      your development directory
 */

#define ACE_MOST "freoMost"

#define ACE_MOST_VERSION "V-0-1-0-d-7"


#define ACE_MOST_BUILD   ACE_MOST" "ACE_MOST_VERSION" [ "__DATE__" - "__TIME__" ] "

/* CVS Log:
 *
 * $Log: version.h,v $
 * Revision 1.7  2013/09/07 09:15:13  maik42
 * bsp/la2: bootloader (y-modem is working, eth still missing),
 *              add frame and test app.
 * build: check bin file size
 * add user libraries for sensors and actors
 * add lcd character display
 * for detailed info see doc/CHANGELOG
 *
 * Revision 1.6  2013-06-16 18:10:43  maik42
 * Version V-0-1-0-d-6, add support for new targets PC(Qemu) and Olimex SAM7-LA2 (AT91SAM7-A2)
 *
 * Revision 1.5  2013-04-14 06:11:36  maik42
 * -flash is changed from block to stream as the low level interface for flash ops already provides more a stream interface than a block interface,
 * -some cleanup work
 *
 * Revision 1.4  2013-01-15 15:50:30  maik42
 * Version V-0-1-0-d-4, for more information see "doc/CHANGELOG"
 *
 * Revision 1.3  2012-07-29 15:58:38  maik42
 * V-0-1-0-d-3: see CHANGELOG
 *
 * Revision 1.2  2010-09-14 17:28:38  maik42
 * V-0.1.0-d-2: bsp/mdc: compileable and running,
 *
 * Revision 1.1  2010-09-13 13:16:33  maik42
 * SYS: add version file, add stack check, improve doxygen header
 *
 *
 */

/** @}
 */

/** @}
 */

#endif
