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
 *
 * @{
 * @file
 * @brief       Default configuration
 *
 * @author      Yahia Abdella <yahia.abdella@tuhh.de>
 */

#include "board.h"
#include "gc9a01a.h"
#include "gc9a01a_constants.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @name    Set default configuration parameters
 * @{
 */
#ifndef GC9A01A_PARAM_PARAM1
#define GC9A01A_PARAM_PARAM1
#endif

#ifndef GC9A01A_PARAMS
#define GC9A01A_PARAMS
#endif
/**@}*/

/**
 * @brief   Configuration struct
 */
static const gc9a01a_params_t gc9a01a_params[] =
{
    GC9A01A_PARAMS
};

#ifdef __cplusplus
}
#endif

/** @} */
