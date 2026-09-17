# mpu6050-driver

Device library for **MPU6050**, wrapped as a Zephyr module.

The bus is a named function (`mpu6050_transfer` in
`lib/mpu6050_bus.h`), defined at link time — not a pointer on the
device object. I2C, SPI, and UART belong in `src/hardware_transfer.c`. Change
the transfer signature if the device is not a register map.

## Where to write code

| File | Role |
|------|------|
| `include/mpu6050.h` | API — types and function prototypes |
| `lib/mpu6050.c` | Real implementations |
| `lib/mpu6050_bus.h` | Named bus functions (`transfer`, `delay`) |
| `lib/mpu6050_regs.h` | Protocol constants |
| `mock/mpu6050.c` | Same symbols, canned values (NDA-safe CI) |

**Adding a function:** declare it in the header, implement it in `lib/`, stub it in `mock/`. Do not edit `src/` for API work.

`src/` is Zephyr packaging (DT, bus bind, `from_dev`). Open it when bringing the chip up, not when adding API functions.

## Backends

Kconfig selects exactly one. Public mock links `mock/`. The other two link `lib/` and supply `mpu6050_transfer` at link time.

| Backend | Links | `mpu6050_transfer` comes from |
|---------|-------|--------------------------------------|
| Public mock (default) | `mock/mpu6050.c` | not used |
| Library mock | `lib/mpu6050.c` | `src/lib_mock_transfer.c` |
| Hardware | `lib/mpu6050.c` | `src/hardware_transfer.c` |

App code:

```c
#include <mpu6050.h>

mpu6050_t *dev = mpu6050_from_dev(DEVICE_DT_GET(DT_ALIAS(mpu6050)));
```

## Tests and REPL

From a west workspace that includes this module:

```bash
west twister -T tests/unit -p native_sim
west build -b native_sim mpu6050-driver/samples/repl
west build -t run
```

Unit tests compile `lib/` only. Uncomment the example in `tests/unit` and add
`mpu6050_transfer` there when tests need a bus. They do not enable the
Zephyr driver.

## App integration (perovsat-app)

1. Add this repo as a west project.
2. Snippet `.conf`: `CONFIG_PEROVSAT_MPU6050=y` and a backend.
3. Devicetree node with `compatible = "invensense,mpu6050"`. Put it on I2C/SPI/UART when filling in hardware.
