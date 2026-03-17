/*
 * Copyright (C) 2026 Technische Universität Hamburg
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

#pragma once

/**
 * @defgroup    drivers_gc9a01a GC9A01A display driver
 * @ingroup     drivers_display
 * @brief       Device driver implementation for the GC9A01A display controller
 *
 * @{
 *
 * @file
 *
 * @author      Yahia Abdella <yahia.abdella@tuhh.de>
 */

#include "lcd.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief GC9A01A VREG1A level.
 *
 * Default VREG1A voltage of 5.34V. VREG1A is the highest positive grayscale reference voltage.
 * VREG1A should be between 4.8 V and 7.34 V .
 */
#ifndef CONFIG_GC9A01A_VREG1A
#define CONFIG_GC9A01A_VREG1A             5340
#endif

/**
 * @brief GC9A01A VREG2A level.
 *
 * Default VREG2A voltage of -3.98V. VREG1A is the lowest negative grayscale reference voltage.
 * VREG1A should be between -4.2 V and -1.66 V .
 */
#ifndef CONFIG_GC9A01A_VREG2A
#define CONFIG_GC9A01A_VREG2A            (-3980)
#endif

/**
 * @name    GC9A01A display rotation modes
 * @{
 */
#define GC9A01A_ROTATION_VERT           LCD_MADCTL_MX           /**< Vertical mode */
#define GC9A01A_ROTATION_VERT_FLIP      LCD_MADCTL_MY           /**< Flipped vertical */
#define GC9A01A_ROTATION_HORZ           LCD_MADCTL_MV           /**< Horizontal mode */
#define GC9A01A_ROTATION_HORZ_FLIP      LCD_MADCTL_MV | \
        LCD_MADCTL_MY | \
        LCD_MADCTL_MX                                           /**< Horizontal flipped */
/** @} */

/**
 * @brief   Device descriptor for the driver
 */
typedef struct {
    /** Device initialization parameters */
    lcd_t dev;
} gc9a01a_t;

/**
 * @brief   LCD device operations table
 */
extern const lcd_driver_t lcd_gc9a01a_driver;

#ifdef __cplusplus
}
#endif

/** @} */
