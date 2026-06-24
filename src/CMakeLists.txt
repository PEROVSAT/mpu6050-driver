zephyr_library()
zephyr_library_include_directories(.)
zephyr_library_sources(mpu6050.c)
zephyr_library_sources_ifdef(CONFIG_PEROVSAT_MPU6050_BACKEND_HARDWARE hardware_transfer.c)
zephyr_library_sources_ifdef(CONFIG_PEROVSAT_MPU6050_BACKEND_SIMULATION simulation_transfer.c)
zephyr_library_sources_ifdef(CONFIG_PEROVSAT_MPU6050_BACKEND_LIBRARY_MOCK lib_mock_transfer.c)
set(mpu6050_LIB ${ZEPHYR_CURRENT_MODULE_DIR}/lib)
zephyr_library_sources_ifdef(CONFIG_PEROVSAT_MPU6050_BACKEND_HARDWARE
			     ${mpu6050_LIB}/mpu6050_lib.c)
zephyr_library_sources_ifdef(CONFIG_PEROVSAT_MPU6050_BACKEND_SIMULATION
			     ${mpu6050_LIB}/mpu6050_lib.c)
zephyr_library_sources_ifdef(CONFIG_PEROVSAT_MPU6050_BACKEND_LIBRARY_MOCK
			     ${mpu6050_LIB}/mpu6050_lib.c)
zephyr_include_directories_ifdef(CONFIG_PEROVSAT_MPU6050_BACKEND_HARDWARE     ${mpu6050_LIB})
zephyr_include_directories_ifdef(CONFIG_PEROVSAT_MPU6050_BACKEND_SIMULATION   ${mpu6050_LIB})
zephyr_include_directories_ifdef(CONFIG_PEROVSAT_MPU6050_BACKEND_LIBRARY_MOCK ${mpu6050_LIB})
