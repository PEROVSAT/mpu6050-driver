#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

struct device;

/**
 * Perform a register read or write for this device instance.
 *
 * @param ctx  Opaque context (typically the Zephyr device pointer).
 * @param reg  Register address or protocol offset.
 * @param buf  Data buffer.
 * @param len  Length of @p buf.
 * @param read true for read, false for write.
 *
 * @return 0 on success, negative errno on failure.
 */
int __DRIVER_SLUG___transfer(void *ctx, uint8_t reg, uint8_t *buf, size_t len, bool read);


/**
 * Initialize transport for this device instance.
 *
 * Called once from driver init before lib_init. Use for bus readiness checks,
 * opening simulation sockets, or clearing mock register state.
 *
 * @param dev  Zephyr device instance.
 *
 * @return 0 on success, negative errno on failure.
 */
int __DRIVER_SLUG___transfer_init(const struct device *dev);
