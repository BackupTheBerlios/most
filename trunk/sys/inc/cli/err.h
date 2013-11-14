/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

#ifndef CLI_ERR_H
#define CLI_ERR_H

#include <ace/err.h>

/** @addtogroup cli
 * @{
 */

/** @addtogroup err
 *
 * Definitions for error constants. 
 *
 * @{
 */

#define CLI_ERR              (CLI_ERR_NR - 1)         /**< Not specified CLI error.  */
#define CLI_ERR_TOKEN        (CLI_ERR_NR - 2)         /**< CLI parser invalid token.  */
#define CLI_ERR_SYNTAX       (CLI_ERR_NR - 3)         /**< CLI parser syntax error. */
#define CLI_ERR_LINESIZE     (CLI_ERR_NR - 4)         /**< CLI command line exceeded max buffer size. */
#define CLI_ERR_LINEINVAL    (CLI_ERR_NR - 5)         /**< CLI command line is invalid. */
#define CLI_ERR_LINEEMPTY    (CLI_ERR_NR - 6)         /**< CLI command line is empty. */
#define CLI_ERR_NOT_FOUND    (CLI_ERR_NR - 7)         /**< descriptor not found */
#define CLI_ERR_NOT_REACHED  (CLI_ERR_NR - 8)         /**< will never happen :-) */
#define CLI_ERR_NO_PARENT    (CLI_ERR_NR - 9)         /**< no parent exists */ 

/** @}
 */

/** @}
 */

#endif
