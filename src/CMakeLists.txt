zephyr_library()
zephyr_library_include_directories(.)
zephyr_library_include_directories(${ZEPHYR_CURRENT_MODULE_DIR}/include)
zephyr_include_directories(${ZEPHYR_CURRENT_MODULE_DIR}/include)

# Packaging. Do not add device functions to these files.
zephyr_library_sources(__DRIVER_SLUG__.c)

zephyr_library_sources_ifdef(CONFIG___KCONFIG_SYM___BACKEND_HARDWARE hardware_transfer.c)
zephyr_library_sources_ifdef(CONFIG___KCONFIG_SYM___BACKEND_LIBRARY_MOCK lib_mock_transfer.c)

# Device API: mock xor library, same symbols (__DRIVER_SLUG___init, …).
zephyr_library_sources_ifdef(CONFIG___KCONFIG_SYM___BACKEND_PUBLIC_MOCK
			     ${ZEPHYR_CURRENT_MODULE_DIR}/mock/__DRIVER_SLUG__.c)

set(__DRIVER_SLUG___LIB ${ZEPHYR_CURRENT_MODULE_DIR}/lib)

if(CONFIG___KCONFIG_SYM___BACKEND_HARDWARE OR CONFIG___KCONFIG_SYM___BACKEND_LIBRARY_MOCK)
	zephyr_library_include_directories(${__DRIVER_SLUG___LIB})
	zephyr_library_sources(${__DRIVER_SLUG___LIB}/__DRIVER_SLUG__.c)
endif()
