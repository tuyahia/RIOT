/*
 * SPDX-FileCopyrightText: 2026 Technische Universität Hamburg
 * SPDX-License-Identifier: LGPL-2.1-only
 */

#pragma once

/**
 * @defgroup    drivers_gc9a01 GC9A01 display driver
 * @ingroup     drivers_display
 * @brief       Device driver implementation for the GC9A01 display controller
 *
 * # About
 * The GC9A01 is a single chip display driver for 240x240 TFT LCD displays.
 *
 * The driver communicates with the device either via an
 *
 * - SPI serial interface (if module `lcd_spi` enabled) or an
 * - MCU 8080 8-/16-bit parallel interface (if module `lcd_parallel` or
 *   module `lcd_parallel_16` is enabled).
 *
 * # Usage
 * After configuring the driver in a board definition via the parameters in
 * gc9a01_params.h (or accepting the default configuration), the driver can be used
 * with the generic LCD API (@ref drivers_lcd). See tests/drivers/gc9a01 for a usage example.
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
 * @name    GC9A01 reference voltage configuration
 * @{
 */
/**
 * @brief GC9A01 VREG1A level.
 *
 * Default VREG1A voltage of 5.34V. VREG1A is the highest positive grayscale reference voltage.
 * VREG1A should be between 4.8 V and 7.34 V .
 */
#ifndef CONFIG_GC9A01_VREG1A
#define CONFIG_GC9A01_VREG1A             (5340)
#endif

/**
 * @brief GC9A01 VREG2A level.
 *
 * Default VREG2A voltage of -3.98V. VREG1A is the lowest negative grayscale reference voltage.
 * VREG2A should be between -4.2 V and -1.66 V .
 */
#ifndef CONFIG_GC9A01_VREG2A
#define CONFIG_GC9A01_VREG2A            (-3980)
#endif
/** @} */

/**
 * @name    GC9A01 display rotation modes
 * @{
 */
#define GC9A01_ROTATION_VERT           LCD_MADCTL_MX           /**< Vertical mode */
#define GC9A01_ROTATION_VERT_FLIP      LCD_MADCTL_MY           /**< Flipped vertical */
#define GC9A01_ROTATION_HORZ           LCD_MADCTL_MV           /**< Horizontal mode */
#define GC9A01_ROTATION_HORZ_FLIP      LCD_MADCTL_MV | \
        LCD_MADCTL_MY | \
        LCD_MADCTL_MX                                           /**< Horizontal flipped */
/** @} */

/**
 * @brief   Device descriptor for the driver
 */
typedef struct {
    /** Device initialization parameters */
    lcd_t dev;
} gc9a01_t;

/**
 * @brief   LCD device operations table
 */
extern const lcd_driver_t lcd_gc9a01_driver;

#ifdef __cplusplus
}
#endif

/** @} */
