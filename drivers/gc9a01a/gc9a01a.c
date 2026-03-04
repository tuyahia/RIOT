/*
 * Copyright (C) 2026 Technische Universität Hamburg
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @ingroup     drivers_gc9a01a
 * @{
 *
 * @file
 * @brief       Device driver implementation for the drivers_gc9a01a
 *
 * @author      Yahia Abdella <yahia.abdella@tuhh.de>
 *
 * @}
 */

#include "gc9a01a.h"
#include "gc9a01a_internal.h"

#include <assert.h>
#include <stdint.h>
#include <string.h>

#include "ztimer.h"

#include "lcd.h"
#include "lcd_internal.h"

#define ENABLE_DEBUG 0
#include "debug.h"

/* Datasheet page 168, formula from table (assumes vrh = 0x28)
 * vreg1a in 1mv increments: 4850 = 4.85V */
static uint8_t _gc9a01a_calc_pwrctl2(uint16_t vreg1a) {
    return (vreg1a - 4800) / 20;
}

/* Datasheet page 169, formula from table (assumes vrh = 0x28)
 * vreg1a in 1mv increments: -4850 = -4.85V */
static uint8_t _gc9a01a_calc_pwrctl3(int16_t vreg2a) {
    return (vreg2a + 4200) / 20;
}

static void _gc9a01a_send_cmds(lcd_t *dev, const uint8_t *cmds)
{
    uint8_t cmd;
    uint8_t numArgs;
    const uint8_t *addr = cmds;
    while ((cmd = *(addr++)) > 0) {
        numArgs = *(addr++);
        lcd_ll_write_cmd(dev, cmd, addr, numArgs);
        addr += numArgs;
    }
}


/* Initialize the display controller. 
 * Initialization sequence mostly adapted from the driver in Espressif's BSP
 * Many registers are undocumented by the manufacturer */
static int _init(lcd_t *dev, const lcd_params_t *params)
{
    (void)params;
    assert(params->lines == 240);
    assert(params->rgb_channels == 240);

    uint8_t command_params[1] = { 0 };

    /* Acquire once and release at the end */
    lcd_ll_acquire(dev);

    /* Soft Reset (requires 120 ms if in Sleep In mode), default display off */
    lcd_ll_write_cmd(dev, LCD_CMD_SWRESET, NULL, 0);
    ztimer_sleep(ZTIMER_MSEC, 120);

    /* Exit sleep */
    lcd_ll_write_cmd(dev, LCD_CMD_SLPOUT, NULL, 0);
    ztimer_sleep(ZTIMER_MSEC, 120);

    /* COLMOD: Interface Pixel Format */
    command_params[0] = 0x55; /* 16 bit mode RGB & Control */
    lcd_ll_write_cmd(dev, LCD_CMD_COLMOD, command_params, 1);

    /* Set Inter_command to high */
    lcd_ll_write_cmd(dev, GC9A01A_INREGEN1, NULL, 0);
    lcd_ll_write_cmd(dev, GC9A01A_INREGEN2, NULL, 0);
    
    {
        uint8_t commands[] = {
            0xEB, 1, 0x14,
            0x84, 1, 0x40,
            0x85, 1, 0xFF,
            0x86, 1, 0xFF,
            0x87, 1, 0xFF,
            0x88, 1, 0x0A,
            0x89, 1, 0x21,
            0x8A, 1, 0x00,
            0x8B, 1, 0x80,
            0x8C, 1, 0x01,
            0x8D, 1, 0x01,
            0x8E, 1, 0xFF,
            0x8F, 1, 0xFF,
            0x90, 4, 0x08, 0x08, 0x08, 0x08,
            0xBD, 1, 0x06,
            0xBC, 1, 0x00,
            0xFF, 3, 0x60, 0x01, 0x04,
            0x0  // End of list
        };

        _gc9a01a_send_cmds(dev, commands);
    }

    /* Power Control 2/3/4 */
    command_params[0] = _gc9a01a_calc_pwrctl2(CONFIG_GC9A01A_VREG1A);
    lcd_ll_write_cmd(dev, GC9A01A_POWER2, command_params, 1);
    command_params[0] = _gc9a01a_calc_pwrctl3(CONFIG_GC9A01A_VREG2A);
    lcd_ll_write_cmd(dev, GC9A01A_POWER3, command_params, 1);
    command_params[0] = GC9A01A_VRH_DEFAULT;
    lcd_ll_write_cmd(dev, GC9A01A_POWER4, command_params, 1);

    {
        uint8_t commands[] = {
            0xBE, 1, 0x11,
            0xE1, 2, 0x10, 0x0E,
            0xDF, 3, 0x21, 0x0c, 0x02,

            /* Gamma correction */
            GC9A01A_GAMMA1, 6, 0x45, 0x09, 0x08, 0x08, 0x26, 0x2A,
            GC9A01A_GAMMA2, 6, 0x43, 0x70, 0x72, 0x36, 0x37, 0x6F,
            GC9A01A_GAMMA3, 6, 0x45, 0x09, 0x08, 0x08, 0x26, 0x2A,
            GC9A01A_GAMMA4, 6, 0x43, 0x70, 0x72, 0x36, 0x37, 0x6F,

            0xED, 2, 0x1B, 0x0B,
            0xAE, 1, 0x77,
            0xCD, 1, 0x63,
            // 0x70, 9, 0x07, 0x07, 0x04, 0x0E, 0x0F, 0x09, 0x07, 0x08, 0x03,

            /* 4 dot inversion */
            GC9A01A_FRAMERATE, 1, GC9A01A_FRAMERATE_4DOT_INVERSION,

            0x62, 12, 0x18, 0x0D, 0x71, 0xED, 0x70, 0x70,
                                0x18, 0x0F, 0x71, 0xEF, 0x70, 0x70,
            0x63, 12, 0x18, 0x11, 0x71, 0xF1, 0x70, 0x70,
                                0x18, 0x13, 0x71, 0xF3, 0x70, 0x70,
            0x64, 7, 0x28, 0x29, 0xF1, 0x01, 0xF1, 0x00, 0x07,
            0x66, 10, 0x3C, 0x00, 0xCD, 0x67, 0x45, 0x45, 0x10, 0x00, 0x00, 0x00,
            0x67, 10, 0x00, 0x3C, 0x00, 0x00, 0x00, 0x01, 0x54, 0x10, 0x32, 0x98,
            0x74, 7, 0x10, 0x85, 0x80, 0x00, 0x00, 0x4E, 0x00,
            0x98, 2, 0x3e, 0x07,
            // 0x99, 2, 0x3e, 0x07,
            0x0  // End of list
        };

        _gc9a01a_send_cmds(dev, commands);
    }

    /* MADCTL: Memory Data Access Control */
    command_params[0] = dev->params->rotation;
    command_params[0] |= dev->params->rgb ? 0 : LCD_MADCTL_BGR;
    lcd_ll_write_cmd(dev, LCD_CMD_MADCTL, command_params, 1);

    /* enable Inversion if configured, reset default is off */
    if (dev->params->inverted) {
        /* INVON: Display Inversion On */
        lcd_ll_write_cmd(dev, LCD_CMD_DINVON, NULL, 0);
    }

    lcd_ll_write_cmd(dev, LCD_CMD_TEON, NULL, 0);

    /* Display on */
    lcd_ll_write_cmd(dev, LCD_CMD_DISPON, NULL, 0);
    ztimer_sleep(ZTIMER_MSEC, 120);

    /* Finally release the device */
    lcd_ll_release(dev);

    return 0;
}

const lcd_driver_t lcd_gc9a01a_driver = {
    .init = _init,
    .set_area = NULL, /* default implementation is used */
};
