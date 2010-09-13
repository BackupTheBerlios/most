#ifndef ACE_VERSION_H
#define ACE_VERSION_H

/** This is the version for MOST.
 *  major.minor.bugfix-(d-counter|r)
 *  d-counter...marks a developer version:
 *  	on each commit the counter must be incremented starting with one
 *  r...marks a release version
 *  	on release version, commits are not allowed, set version to development before you commit
 *      each release version must have a CVS label with the version name
 *      if you go from d to r version major or minor or bugfix must be increased
 */

#define ACE_MOST_VERSION "V-0.1.0-d-1"

/* CVS Log:
 *
 * $Log: version.h,v $
 * Revision 1.1  2010/09/13 13:16:33  maik42
 * SYS: add version file, add stack check, improve doxygen header
 *
 *
 */

#endif
