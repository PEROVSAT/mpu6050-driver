zephyr_library()
zephyr_library_include_directories(.)
zephyr_library_include_directories(${ZEPHYR_CURRENT_MODULE_DIR}/include)
zephyr_include_directories(${ZEPHYR_CURRENT_MODULE_DIR}/include)

# Packaging. Do not add device functions to these files.
zephyr_library_sources(mpu6050.c)

zephyr_library_sources_ifdef(CONFIG_PEROVSAT_MPU6050_BACKEND_HARDWARE hardware_transfer.c)
zephyr_library_sources_ifdef(CONFIG_PEROVSAT_MPU6050_BACKEND_LIBRARY_MOCK lib_mock_transfer.c)

# Device API: mock xor library, same symbols (mpu6050_init, …).
zephyr_library_sources_ifdef(CONFIG_PEROVSAT_MPU6050_BACKEND_PUBLIC_MOCK
			     ${ZEPHYR_CURRENT_MODULE_DIR}/mock/mpu6050.c)

set(mpu6050_LIB ${ZEPHYR_CURRENT_MODULE_DIR}/lib)

if(CONFIG_PEROVSAT_MPU6050_BACKEND_HARDWARE OR CONFIG_PEROVSAT_MPU6050_BACKEND_LIBRARY_MOCK)
	zephyr_library_include_directories(${mpu6050_LIB})
	zephyr_library_sources(${mpu6050_LIB}/mpu6050.c)
endif()
