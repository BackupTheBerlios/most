/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

#ifndef DEV_ERR_H
#define DEV_ERR_H

#include <ace/err.h>

/** @addtogroup dev
 * @{
 */

/** @addtogroup err err.h
 *
 * Definitions for error constants. 
 *
 * @{
 */

#define DEV_ERR                     (DEV_ERR_NR - 1)    /**< Not specified DEV error. */
#define DEV_ERR_ADDR                (DEV_ERR_NR - 16)   /**< invalid address */
#define DEV_ERR_LEN                 (DEV_ERR_NR - 16)   /**< invalid length */
#define DEV_ERR_TIMEOUT             (DEV_ERR_NR - 16)   /**< timeout */
#define DEV_ERR_MMC_BLOCK_SET       (DEV_ERR_NR - 10)   /**< MMC */
#define DEV_ERR_MMC_RESPONSE        (DEV_ERR_NR - 11)   /**< MMC response error*/
#define DEV_ERR_MMC_DATA_TOKEN      (DEV_ERR_NR - 12)   /**< MMC data token not received */
#define DEV_ERR_MMC_INIT            (DEV_ERR_NR - 13)   /**< MMC initialize failed*/
#define DEV_ERR_MMC_CRC             (DEV_ERR_NR - 14)   /**< MMC crc error */
#define DEV_ERR_MMC_WRITE           (DEV_ERR_NR - 15)   /**< MMC write error*/
#define DEV_ERR_MMC_OTHER           (DEV_ERR_NR - 16)   /**< MMC undefined error*/


/** @}
 */

/** @}
 */

#endif
