#include "__DRIVER_SLUG__.h"

#include <errno.h>

int __DRIVER_SLUG___init(__DRIVER_SLUG___t *dev, const __DRIVER_SLUG___config_t *cfg)
{
	if (dev == NULL || cfg == NULL) {
		return -EINVAL;
	}

	(void)cfg;

	/* FILL IN: probe the part, apply cfg, cache state on *dev.
	 * Call __DRIVER_SLUG___transfer / __DRIVER_SLUG___delay from
	 * lib/__DRIVER_SLUG___bus.h. Do not add API functions in src/.
	 */

	return 0;
}
