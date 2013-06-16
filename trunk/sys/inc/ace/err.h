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

#define ACE_OK         0    /**< No error, everything OK. */


/* Begin: Error number ranges ! ----------------------------- */

#define SYS_ERR_NR     0         /**< System error numbers */
#define USR_ERR_NR     -2000     /**< User   error numbers */
#define BOARD_ERR_NR   -9000     /**< Board  error numbers */


/* SYS */
#define DEF_ERR_NR         (SYS_ERR_NR - 0)    /**< Default error numbers */
#define ACE_ERR_NR         (SYS_ERR_NR - 100)  /**< ACE error numbers */
#define USO_ERR_NR         (SYS_ERR_NR - 200)  /**< USO error numbers */
#define DEV_ERR_NR         (SYS_ERR_NR - 300)  /**< DEV error numbers */
#define NET_ERR_NR         (SYS_ERR_NR - 400)  /**< NET error numbers */
#define NAP_ERR_NR         (SYS_ERR_NR - 500)  /**< NAP error numbers */
#define MFS_ERR_NR         (SYS_ERR_NR - 600)  /**< MFS error numbers */
#define CLI_ERR_NR         (SYS_ERR_NR - 700)  /**< CLI error numbers */

/* USR */
#define DPY_ERR_NR         (USR_ERR_NR - 0)    /**< DPY error numbers */
#define TST_ERR_NR         (USR_ERR_NR - 100)  /**< TST error numbers */

/* Board */
#define BSP_ERR_NR         (BOARD_ERR_NR - 0)    /**< BSP error numbers */
#define BOOT_ERR_NR        (BOARD_ERR_NR - 100)  /**< Bootloader error numbers */
#define KERNEL_ERR_NR      (BOARD_ERR_NR - 200)  /**< Kernel error numbers */
#define APP_ERR_NR         (BOARD_ERR_NR - 300)  /**< Application error numbers */

/* End: Error number ranges ! ----------------------------- */


/* Begin: Error numbers ! ----------------------------- */

/** Default error numbers */
#define DEF_ERR_EOF            (DEF_ERR_NR - 1)   /**< End of File. */
#define DEF_ERR_MEM            (DEF_ERR_NR - 2)   /**< Out of memory. */
#define DEF_ERR_BUF            (DEF_ERR_NR - 3)   /**< Buffer overflow. */
#define DEF_ERR_VAL            (DEF_ERR_NR - 4)   /**< Value invalid. */
#define DEF_ERR_ARG            (DEF_ERR_NR - 5)   /**< Invalid argument. */
#define DEF_ERR_TIMEOUT        (DEF_ERR_NR - 6)   /**< Timeout. */
#define DEF_ERR_BUSY           (DEF_ERR_NR - 7)   /**< Resource is busy. */
#define DEF_ERR_ROM            (DEF_ERR_NR - 8)   /**< Flash ROM error. */
#define DEF_ERR_RANGE          (DEF_ERR_NR - 9)   /**< Out of range. */
#define DEF_ERR_SIZE           (DEF_ERR_NR - 10)   /**< Invalid size. */
#define DEF_ERR_NULL           (DEF_ERR_NR - 11)   /**< Null pointer. */


/** ACE specific error numbers */
#define ACE_ERR                (ACE_ERR_NR - 1)   /**< Not specified ACE error. */
#define ACE_ERR_YEAR           (ACE_ERR_NR - 2)   /**< Invalid year in time struct. */
#define ACE_ERR_MONTH          (ACE_ERR_NR - 3)   /**< Invalid month in time struct. */
#define ACE_ERR_DAY            (ACE_ERR_NR - 4)   /**< Invalid day in time struct. */
#define ACE_ERR_HOUR           (ACE_ERR_NR - 5)   /**< Invalid hour in time struct. */
#define ACE_ERR_MIN            (ACE_ERR_NR - 6)   /**< Invalid min in time struct. */
#define ACE_ERR_SEC            (ACE_ERR_NR - 7)   /**< Invalid sec in time struct. */

/* End: Error numbers ! ----------------------------- */

/** @}
 */

/** @}
 */

#endif /* ACE_ERR_H_ */
