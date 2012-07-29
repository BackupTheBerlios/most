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

#define DEV_ERR                     (ACE_DEV_ERR_NR - 1)    /**< Not specified DEV error. */
#define DEV_ERR_MMC_BLOCK_SET       (ACE_DEV_ERR_NR - 10)   /**< MMC */
#define DEV_ERR_MMC_RESPONSE        (ACE_DEV_ERR_NR - 11)   /**< MMC response error*/
#define DEV_ERR_MMC_DATA_TOKEN      (ACE_DEV_ERR_NR - 12)   /**< MMC data token not received */
#define DEV_ERR_MMC_INIT            (ACE_DEV_ERR_NR - 13)   /**< MMC initialize failed*/
#define DEV_ERR_MMC_CRC             (ACE_DEV_ERR_NR - 14)   /**< MMC crc error */
#define DEV_ERR_MMC_WRITE           (ACE_DEV_ERR_NR - 15)   /**< MMC write error*/
#define DEV_ERR_MMC_OTHER           (ACE_DEV_ERR_NR - 16)   /**< MMC undefined error*/


/** @}
 */

/** @}
 */

#endif