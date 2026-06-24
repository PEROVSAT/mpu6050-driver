zephyr_library()
zephyr_library_include_directories(.)
zephyr_library_sources(__DRIVER_SLUG__.c)
zephyr_library_sources_ifdef(CONFIG___KCONFIG_SYM___BACKEND_HARDWARE hardware_transfer.c)
zephyr_library_sources_ifdef(CONFIG___KCONFIG_SYM___BACKEND_SIMULATION simulation_transfer.c)
zephyr_library_sources_ifdef(CONFIG___KCONFIG_SYM___BACKEND_LIBRARY_MOCK lib_mock_transfer.c)
set(__DRIVER_SLUG___LIB ${ZEPHYR_CURRENT_MODULE_DIR}/lib)
zephyr_library_sources_ifdef(CONFIG___KCONFIG_SYM___BACKEND_HARDWARE
			     ${__DRIVER_SLUG___LIB}/__DRIVER_SLUG___lib.c)
zephyr_library_sources_ifdef(CONFIG___KCONFIG_SYM___BACKEND_SIMULATION
			     ${__DRIVER_SLUG___LIB}/__DRIVER_SLUG___lib.c)
zephyr_library_sources_ifdef(CONFIG___KCONFIG_SYM___BACKEND_LIBRARY_MOCK
			     ${__DRIVER_SLUG___LIB}/__DRIVER_SLUG___lib.c)
zephyr_include_directories_ifdef(CONFIG___KCONFIG_SYM___BACKEND_HARDWARE     ${__DRIVER_SLUG___LIB})
zephyr_include_directories_ifdef(CONFIG___KCONFIG_SYM___BACKEND_SIMULATION   ${__DRIVER_SLUG___LIB})
zephyr_include_directories_ifdef(CONFIG___KCONFIG_SYM___BACKEND_LIBRARY_MOCK ${__DRIVER_SLUG___LIB})
