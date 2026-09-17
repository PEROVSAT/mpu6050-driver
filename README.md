# driver-template

Cookiecutter for a PerovSat device library packaged as a Zephyr module.

```bash
source .venv/bin/activate
python setup.py
```

You will be asked for a device-model slug (e.g. `mpu6050`) and a devicetree vendor prefix (e.g. `invensense`). `setup.py` substitutes tokens, renames files, and replaces this README with the generated driver's README.

After setup, the API lives in `include/<slug>.h`. Implement functions in `lib/` and `mock/`. The bus is a named function in `lib/<slug>_bus.h`, defined at link time — do not add function pointers on the device object. Do not add API methods in `src/`.
