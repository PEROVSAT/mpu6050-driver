#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/*
 * Link-time bus. lib/ calls these by name; the build supplies the bodies.
 *
 * Linked from exactly one place:
 *   - src/hardware_transfer.c
 *   - src/lib_mock_transfer.c
 *   - tests/unit
 *
 * __DRIVER_SLUG___delay is defined in src/__DRIVER_SLUG__.c (k_msleep) or
 * by the unit test (no-op).
 *
 * The signature below is a register read/write. Change it here and in every
 * definition if the device is not a register map.
 */

int __DRIVER_SLUG___transfer(void *ctx, uint8_t reg, uint8_t *buf, size_t len, bool read);
void __DRIVER_SLUG___delay(uint32_t ms);
