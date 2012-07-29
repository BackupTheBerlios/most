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
 *  d-counter...marks a developer version:
 *  	for each commit the counter must be incremented starting with one,
 *      best you increment the counter immediate after the commit.
 *  t-counter...marks a testing version:
 *      a testing version is created by branching a developer version replacing d with t
 *      for each commit the counter must be incremented,
 *      after creating the branch the developer version minor number on the main line must be incremented by 1
 *      and d reset to 1.
 *  r...marks a release version
 *      only testing versions are allowed to became a release version.
 *      only bugfix commits are allowed incrementing the bugfix number.
 *      each release version must have a CVS label with the version name
 *      for building the release check out the testing version, do not build the release from
 *      your development directory
 */

#define ACE_MOST_VERSION "V-0.1.0-d-3"


#define ACE_MOST_BUILD   ACE_MOST_VERSION" [ "__DATE__" - "__TIME__" ] "

/* CVS Log:
 *
 * $Log: version.h,v $
 * Revision 1.3  2012/07/29 15:58:38  maik42
 * see CHANGELOG
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
