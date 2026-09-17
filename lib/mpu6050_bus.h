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
 * mpu6050_delay is defined in src/mpu6050.c (k_msleep) or
 * by the unit test (no-op).
 *
 * The signature below is a register read/write. Change it here and in every
 * definition if the device is not a register map.
 */

int mpu6050_transfer(void *ctx, uint8_t reg, uint8_t *buf, size_t len, bool read);
void mpu6050_delay(uint32_t ms);
