/*
 * assert.h
 *
 *  Created on: 30.11.2010
 *      Author: maik
 */

#ifndef ACE_ASSERT_H_
#define ACE_ASSERT_H_

#include "ace/stdlib.h"

/** @addtogroup ace
 *
 * @{
 */

/** @defgroup assert assert.h
 *
 * @{
 */

/** Turn off assertion and sanity checks.
 */
#define ACE_NDEBUG 0

#if ACE_NDEBUG

/** No assert */
#define ACE_ASSERT(condition, msg)

/** No sanity check */
#define ACE_SANITY_CHECK(condition)

#else

/** If condition is not valid do abort and print msg */
#define ACE_ASSERT(condition, msg)  { \
            if (!(condition)) { \
                ACE_ABORT(msg); \
            } \
        }

/** Check for NULL pointer */
#define ACE_SANITY_CHECK(pointer)  ACE_ASSERT(pointer, "sanity error (null pointer)")

#endif


#endif /* ACE_ASSERT_H_ */

/** @}
 */

/** @}
 */
