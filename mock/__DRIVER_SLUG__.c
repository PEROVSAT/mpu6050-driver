/*
 * Public fake of the device API. Same symbols as lib/__DRIVER_SLUG__.c;
 * CMake links exactly one of the two.
 */

#include "__DRIVER_SLUG__.h"

int __DRIVER_SLUG___init(__DRIVER_SLUG___t *dev, const __DRIVER_SLUG___config_t *cfg)
{
	(void)dev;
	(void)cfg;

	return 0;
}
