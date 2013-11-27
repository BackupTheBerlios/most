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
 * @{
 */


/**
 * Default error type.
 */
typedef int ACE_err_t;

/* Begin: Error number ranges ! ----------------------------- */

#define SYS_ERR_NR     0         /**< System error numbers start at -1 as 0 is OK, also -1 is EOF*/
#define BOARD_ERR_NR   -1000     /**< Board  error numbers */
#define USR_ERR_NR     -2000     /**< User   error numbers */
#define APP_ERR_NR     -10000    /**< Application error numbers */


/* SYS */
#define DEF_ERR_NR         (SYS_ERR_NR - 0)    /**< Default error numbers must be the first! */
#define ACE_ERR_NR         (SYS_ERR_NR - 100)  /**< ACE error numbers */
#define USO_ERR_NR         (SYS_ERR_NR - 200)  /**< USO error numbers */
#define DEV_ERR_NR         (SYS_ERR_NR - 300)  /**< DEV error numbers */
#define NET_ERR_NR         (SYS_ERR_NR - 400)  /**< NET error numbers */
#define NAP_ERR_NR         (SYS_ERR_NR - 500)  /**< NAP error numbers */
#define MFS_ERR_NR         (SYS_ERR_NR - 600)  /**< MFS error numbers */
#define CLI_ERR_NR         (SYS_ERR_NR - 700)  /**< CLI error numbers */

/* USR */
#define TST_ERR_NR         (USR_ERR_NR - 0)    /**< TST error numbers */
#define DPY_ERR_NR         (USR_ERR_NR - 100)  /**< DPY error numbers */
#define SEN_ERR_NR         (USR_ERR_NR - 200)  /**< SEN error numbers */
#define ACT_ERR_NR         (USR_ERR_NR - 300)  /**< ACT error numbers */

/* Board */
#define BSP_ERR_NR         (BOARD_ERR_NR - 0)    /**< BSP error numbers */
#define BOOT_ERR_NR        (BOARD_ERR_NR - 100)  /**< Bootloader error numbers */
#define KERNEL_ERR_NR      (BOARD_ERR_NR - 200)  /**< Kernel error numbers */

/* End: Error number ranges ! ----------------------------- */


/* Begin: Error numbers ! ----------------------------- */

/** Default error numbers */
#define ACE_OK                 (DEF_ERR_NR - 0)   /**< No error. */
#define DEF_ERR_SYS            (DEF_ERR_NR - 1)   /**< System failure. */
#define DEF_ERR_NULL           (DEF_ERR_NR - 2)   /**< Null pointer. */
#define DEF_ERR_MEM            (DEF_ERR_NR - 3)   /**< Out of memory. */
#define DEF_ERR_BUF            (DEF_ERR_NR - 4)   /**< Buffer overflow. */
#define DEF_ERR_VAL            (DEF_ERR_NR - 5)   /**< Value invalid. */
#define DEF_ERR_ARG            (DEF_ERR_NR - 6)   /**< Invalid argument. */
#define DEF_ERR_RANGE          (DEF_ERR_NR - 7)   /**< Out of range. */
#define DEF_ERR_SIZE           (DEF_ERR_NR - 8)   /**< Invalid size. */
#define DEF_ERR_TIMEOUT        (DEF_ERR_NR - 9)   /**< Timeout. */
#define DEF_ERR_BUSY           (DEF_ERR_NR - 10)   /**< Resource is busy. */
#define DEF_ERR_IN_USE         (DEF_ERR_NR - 11)   /**< Resource is in use. */
#define DEF_ERR_NOT_FOUND      (DEF_ERR_NR - 12)   /**< Not found. */
#define DEF_ERR_ROM            (DEF_ERR_NR - 13)    /**< Flash ROM error. */


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
