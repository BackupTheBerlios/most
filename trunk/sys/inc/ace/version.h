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
 *
 *  major.minor.bugfix-(d-counter|t-counter|r)
 *
 *  for each commit increment a counter or version and make a tag with the version!
 *
 *  d-counter...marks a developer version: (debugging allowed)
 *      for each commit the counter must be incremented starting with one,
 *      best you increment the counter immediate after the commit.
 *  t-counter...marks a testing version: (no debugging)
 *      a testing version is created by replacing d with t and set counter to 1
 *      for each commit the counter must be incremented,
 *      this shall be the beginning of the test phase
 *  r...marks a release version: (no debugging)
 *      this means a freeze and only bugfixes are allowed (we also need the bugfixes on the trunk)
 *      only testing versions are allowed to become a release version.
 *      each release version must have a CVS label with the version name
 *      for building the release check out the testing (release) version, do not build the release from
 *      your development directory
 *      for each bugfix commit increment the bugfix counter
 *
 *  d-counter ... after installing the release change back to developer version:
 *      after creating a release the version minor number must be incremented by 1
 *      and change r to d-1.
 *
 *  r...if you have to make a bugfix on a release which has turned back to development
 *      you have to branch the release with the latest bugfix
 *      and increment the bugfix number
 *      (i will not do that here on berlios)
 *
 *
 *   VERSION V-0-1-0-d-x   pre alpha development
 *   VERSION V-0-1-0-t-x   pre alpha testing
 *   VERSION V-0-1-0-r     alpha version
 *   VERSION V-0-1-x-r     alpha version bugfix
 *   VERSION V-0-2-0-d-x   pre beta development
 *   VERSION V-0-2-0-t-x   pre beta testing
 *   VERSION V-0-2-0-r     beta version
 *   VERSION V-0-2-x-r     beta version bugfix
 *   VERSION V-0-3-0-d-x   beta version continued
               ..........
 *   VERSION V-1-0-0-r     first release ....
 *   VERSION V-1-0-x-r     first release bugfix
 *   VERSION V-1-1-0-d-x   release development
 *           .................
 *
 *
 */

#define ACE_MOST "freoMOST"

#define ACE_MOST_VERSION "V-0-1-0-d-11"

#if defined MOST_BUILD_DEBUG
#define ACE_MOST_BUILD    "debug"
#elif defined MOST_BUILD_RUN
#define ACE_MOST_BUILD    "run"
#else
#error "BUILD not defined!"
#endif

#define ACE_MOST_VERSION_STRING   ACE_MOST" "ACE_MOST_VERSION" [ "__DATE__" - "__TIME__" ] build: "ACE_MOST_BUILD

/* CVS Log:
 *
 * $Log: version.h,v $
 * Revision 1.11  2014/01/13 10:39:34  maik42
 * Version V-0-1-0-d-11  see CHANGELOG, fexible OCD configuration, add lalinea, ...
 *
 * Revision 1.10  2013-12-21 11:10:39  maik42
 * V-0-1-0-d-10 see CHANGELOG, just sync development
 *
 * Revision 1.9  2013-11-27 21:27:41  maik42
 * V-0-1-0-d-9 see doc/CHANGELOG, bugfixis, up to 4 virtual terminals, vt100 esc subset (very limited)
 *
 * Revision 1.8  2013-11-14 17:46:29  maik42
 * V-0-1-0-d-8, see CHANGELOG
 *
 * Revision 1.7  2013-09-07 09:15:13  maik42
 * Version V-0-1-0-d-7:
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
