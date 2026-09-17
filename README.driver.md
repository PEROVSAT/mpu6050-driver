# __MODULE_NAME__

Device library for **__DRIVER_UPPER__**, wrapped as a Zephyr module.

The bus is a named function (`__DRIVER_SLUG___transfer` in
`lib/__DRIVER_SLUG___bus.h`), defined at link time — not a pointer on the
device object. I2C, SPI, and UART belong in `src/hardware_transfer.c`. Change
the transfer signature if the device is not a register map.

## Where to write code

| File | Role |
|------|------|
| `include/__DRIVER_SLUG__.h` | API — types and function prototypes |
| `lib/__DRIVER_SLUG__.c` | Real implementations |
| `lib/__DRIVER_SLUG___bus.h` | Named bus functions (`transfer`, `delay`) |
| `lib/__DRIVER_SLUG___regs.h` | Protocol constants |
| `mock/__DRIVER_SLUG__.c` | Same symbols, canned values (NDA-safe CI) |

**Adding a function:** declare it in the header, implement it in `lib/`, stub it in `mock/`. Do not edit `src/` for API work.

`src/` is Zephyr packaging (DT, bus bind, `from_dev`). Open it when bringing the chip up, not when adding API functions.

## Backends

Kconfig selects exactly one. Public mock links `mock/`. The other two link `lib/` and supply `__DRIVER_SLUG___transfer` at link time.

| Backend | Links | `__DRIVER_SLUG___transfer` comes from |
|---------|-------|--------------------------------------|
| Public mock (default) | `mock/__DRIVER_SLUG__.c` | not used |
| Library mock | `lib/__DRIVER_SLUG__.c` | `src/lib_mock_transfer.c` |
| Hardware | `lib/__DRIVER_SLUG__.c` | `src/hardware_transfer.c` |

App code:

```c
#include <__DRIVER_SLUG__.h>

__DRIVER_SLUG___t *dev = __DRIVER_SLUG___from_dev(DEVICE_DT_GET(DT_ALIAS(__DRIVER_SLUG__)));
```

## Tests and REPL

From a west workspace that includes this module:

```bash
west twister -T tests/unit -p native_sim
west build -b native_sim __MODULE_NAME__/samples/repl
west build -t run
```

Unit tests compile `lib/` only. Uncomment the example in `tests/unit` and add
`__DRIVER_SLUG___transfer` there when tests need a bus. They do not enable the
Zephyr driver.

## App integration (perovsat-app)

1. Add this repo as a west project.
2. Snippet `.conf`: `CONFIG___KCONFIG_SYM__=y` and a backend.
3. Devicetree node with `compatible = "__COMPAT__"`. Put it on I2C/SPI/UART when filling in hardware.
