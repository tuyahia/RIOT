/*
 * Copyright (C) 2026 Technische Universität Hamburg
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

#pragma once

/**
 * @ingroup     drivers_gc9a01a
 * @{
 *
 * @file
 * @brief       Internal addresses, registers and constants
 *
 * @author      Yahia Abdella <yahia.abdella@tuhh.de>
 */

#include "lcd_internal.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @name LCD GC9A01A commands
 *
 * LCD commands extension available for GC9A01A LCD controllers
 * @{
 */
#define GC9A01A_POWER7 0xA7    ///< Power Control 7
#define GC9A01A_POWER1 0xC1    ///< Power Control 1
#define GC9A01A_POWER2 0xC3    ///< Power Control 2
#define GC9A01A_POWER3 0xC4    ///< Power Control 3
#define GC9A01A_POWER4 0xC9    ///< Power Control 4
#define GC9A01A_FRAMERATE 0xE8 ///< Frame rate control
#define GC9A01A_INREGEN1 0xFE  ///< Inter register enable 1
#define GC9A01A_INREGEN2 0xEF  ///< Inter register enable 2
#define GC9A01A_GAMMA1 0xF0    ///< Set gamma 1
#define GC9A01A_GAMMA2 0xF1    ///< Set gamma 2
#define GC9A01A_GAMMA3 0xF2    ///< Set gamma 3
#define GC9A01A_GAMMA4 0xF3    ///< Set gamma 4
/** @} */

/**
 * @name LCD GC9A01A values
 *
 * Default values for registers of the GC9A01A LCD controller
 * @{
 */
#define GC9A01A_VRH_DEFAULT 0x28
#define GC9A01A_FRAMERATE_4DOT_INVERSION 0x34
/** @} */

/**
 * @brief   GC9A01A controller specific initialization part
 */
int gc9a01a_init(lcd_t *dev, const lcd_params_t *params);

#ifdef __cplusplus
}
#endif

/** @} */
