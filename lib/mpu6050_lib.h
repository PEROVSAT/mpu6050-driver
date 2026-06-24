#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/**
 * Bus-agnostic byte transfer callback.
 *
 * @param ctx  Opaque context (typically the Zephyr device pointer).
 * @param reg  Register address or protocol offset.
 * @param buf  Data buffer.
 * @param len  Length of @p buf.
 * @param read true for read, false for write.
 *
 * @return 0 on success, negative errno on failure.
 */
typedef int (*mpu6050_transfer_fn)(void *ctx, uint8_t reg, uint8_t *buf, size_t len, bool read);

/**
 * Probe the device and apply initial configuration.
 *
 * Use @p transfer and @p ctx to communicate with the device (e.g. read WHO_AM_I,
 * write config registers). Call once at startup.
 *
 * @param transfer  Bus transfer callback.
 * @param ctx       Opaque context passed to @p transfer.
 *
 * @return 0 on success, negative errno on failure.
 */
int mpu6050_lib_init(mpu6050_transfer_fn transfer, void *ctx);

/* FILL IN: declare protocol functions, e.g.:
 *
 * int mpu6050_lib_read_reg(mpu6050_transfer_fn transfer, void *ctx,
 *                                  uint8_t reg, uint8_t *val);
 */
