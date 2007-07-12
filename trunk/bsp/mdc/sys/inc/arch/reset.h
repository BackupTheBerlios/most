/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

#ifndef MDC_RESET_H
#define MDC_RESET_H

/** @addtogroup arch
 *
 * @{
 */

/** @defgroup reset reset.h
 *
 * Jump to application.
 * This function doesn't return
 * @{
 */

/**
 * Jump to Application
 */
void MDC_jump_app(void);

/**
 * Jump to Bootloader
 */
void MDC_jump_boot(void);

/** @}
 */

/** @}
 */

#endif
