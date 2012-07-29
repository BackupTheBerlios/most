/*
 * err.h
 *
 *  Created on: 28.11.2010
 *      Author: maik
 */

#ifndef ACE_ERR_H_
#define ACE_ERR_H_

/** @addtogroup ace
 *
 * @{
 */

/** @defgroup err err.h
 *
 * Definitions for error constants.
 * Positive values mean no error,
 * Negative values mean error.
 *
 * For error numbers, always start with an offset of -1 and never start with an offset of 0 because when the range
 * is changed starting at 0 then an error value with an offset of 0 would mean ERR_OK.
 * @{
 */


/**
 * Default error type.
 */
typedef int ACE_err_t;

#define ACE_ERR_OK         0    /**< No error, everything OK. */


/* Begin: Error number ranges ! ----------------------------- */

#define ACE_SYS_ERR_NR     0         /**< System error numbers */
#define ACE_USR_ERR_NR     -2000     /**< User   error numbers */
#define ACE_BOARD_ERR_NR   -9000     /**< Board  error numbers */


/* SYS */
#define ACE_DEF_ERR_NR         (ACE_SYS_ERR_NR - 0)    /**< Default error numbers */
#define ACE_ACE_ERR_NR         (ACE_SYS_ERR_NR - 100)  /**< ACE error numbers */
#define ACE_USO_ERR_NR         (ACE_SYS_ERR_NR - 200)  /**< USO error numbers */
#define ACE_DEV_ERR_NR         (ACE_SYS_ERR_NR - 300)  /**< DEV error numbers */
#define ACE_NET_ERR_NR         (ACE_SYS_ERR_NR - 400)  /**< NET error numbers */
#define ACE_NAP_ERR_NR         (ACE_SYS_ERR_NR - 500)  /**< NAP error numbers */
#define ACE_MFS_ERR_NR         (ACE_SYS_ERR_NR - 600)  /**< MFS error numbers */
#define ACE_CLI_ERR_NR         (ACE_SYS_ERR_NR - 700)  /**< CLI error numbers */

/* USR */
#define ACE_DPY_ERR_NR         (ACE_USR_ERR_NR - 0)    /**< DPY error numbers */
#define ACE_TST_ERR_NR         (ACE_USR_ERR_NR - 100)  /**< TST error numbers */

/* Board */
#define ACE_BSP_ERR_NR         (ACE_BOARD_ERR_NR - 0)    /**< BSP error numbers */
#define ACE_BOOT_ERR_NR        (ACE_BOARD_ERR_NR - 900)  /**< Bootloader error numbers */
#define ACE_APP_ERR_NR         (ACE_BOARD_ERR_NR - 1000)  /**< Application error numbers */

/* End: Error number ranges ! ----------------------------- */


/* Begin: Error numbers ! ----------------------------- */

/** Default error numbers */
#define ACE_DEF_ERR                (ACE_DEF_ERR_NR - 1)   /**< Not specified error. */
#define ACE_DEF_ERR_MEM            (ACE_DEF_ERR_NR - 2)   /**< Out of memory. */
#define ACE_DEF_ERR_BUF            (ACE_DEF_ERR_NR - 3)   /**< Buffer overflow. */
#define ACE_DEF_ERR_VAL            (ACE_DEF_ERR_NR - 4)   /**< Value invalid. */
#define ACE_DEF_ERR_ARG            (ACE_DEF_ERR_NR - 5)   /**< Invalid argument. */
#define ACE_DEF_ERR_TIMEOUT        (ACE_DEF_ERR_NR - 6)   /**< Timeout. */


/** ACE specific error numbers */
#define ACE_ERR                (ACE_ACE_ERR_NR - 1)   /**< Not specified ACE error. */
#define ACE_ERR_YEAR           (ACE_ACE_ERR_NR - 2)   /**< Invalid year in time struct. */
#define ACE_ERR_MONTH          (ACE_ACE_ERR_NR - 3)   /**< Invalid month in time struct. */
#define ACE_ERR_DAY            (ACE_ACE_ERR_NR - 4)   /**< Invalid day in time struct. */
#define ACE_ERR_HOUR           (ACE_ACE_ERR_NR - 5)   /**< Invalid hour in time struct. */
#define ACE_ERR_MIN            (ACE_ACE_ERR_NR - 6)   /**< Invalid min in time struct. */
#define ACE_ERR_SEC            (ACE_ACE_ERR_NR - 7)   /**< Invalid sec in time struct. */

/* End: Error numbers ! ----------------------------- */

/** @}
 */

/** @}
 */

#endif /* ACE_ERR_H_ */
