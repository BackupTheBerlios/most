/*
 * bsp_commands.h
 *
 *  Created on: 11.09.2010
 *      Author: maik
 */

#ifndef BSP_COMMANDS_H_
#define BSP_COMMANDS_H_

#include <mfs/vfs.h>

/** @addtogroup init
 *
 * @{
 */

/** @defgroup bsp_commands bsp_commands.h
 *
 * Configuration
 *
 * @{
 */

/**
 *
 */
extern void
LA2_start_boot (void);

/**
 *
 */
extern void
LA2_start_app (void);

/**
 *
 */
extern void LA2_bsp_commands_install (MFS_descriptor_t * bsp);

/** @}
 */

/** @}
 */

#endif /* BSP_COMMANDS_H_ */
