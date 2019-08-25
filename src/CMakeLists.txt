
set(SOURCES
  ${SOURCES_PREFIX}/board_config.c
  ${SOURCES_PREFIX}/board_config.h
  ${SOURCES_PREFIX}/config.h
	${SOURCES_PREFIX}/display_device.c
	${SOURCES_PREFIX}/display_device.h
	#${SOURCES_PREFIX}/st7789h2.c
	#${SOURCES_PREFIX}/st7789h2.h
	#${SOURCES_PREFIX}/lcd.h
	${SOURCES_PREFIX}/devfs_config.c
	${SOURCES_PREFIX}/link_config.c
  ${SOURCES_PREFIX}/link_config.h
  ${SOURCES_PREFIX}/mcu_config.c
  ${SOURCES_PREFIX}/sos_config.c
	${SOURCES_PREFIX}/board_symbols.c
	${SOURCES_PREFIX}/ram_drive.c
	${SOURCES_PREFIX}/ram_drive.h
	${SOURCES_PREFIX}/stm32_bsp.c
	${SOURCES_PREFIX}/stm32_bsp.h
	${SOURCES_PREFIX}/sl_config.h
	${SOURCES_PREFIX}/kernel_request.c
	${SOURCES_PREFIX}/SystemClock_Config.c
	${SOURCES_PREFIX}/wifi_api.c
	${SOURCES_PREFIX}/wifi_api.h

	${SOURCES_PREFIX}/atwinc1500/driver/include/ecc_types.h
	${SOURCES_PREFIX}/atwinc1500/driver/include/m2m_ssl.h
	${SOURCES_PREFIX}/atwinc1500/driver/include/m2m_crypto.h
	${SOURCES_PREFIX}/atwinc1500/driver/include/m2m_ate_mode.h
	${SOURCES_PREFIX}/atwinc1500/driver/include/m2m_ota.h
	${SOURCES_PREFIX}/atwinc1500/driver/include/m2m_types.h
	${SOURCES_PREFIX}/atwinc1500/driver/include/m2m_wifi.h
	${SOURCES_PREFIX}/atwinc1500/driver/include/m2m_periph.h

	${SOURCES_PREFIX}/atwinc1500/driver/source/m2m_hif.h
	${SOURCES_PREFIX}/atwinc1500/driver/source/nmasic.c
	${SOURCES_PREFIX}/atwinc1500/driver/source/nmi2c.c
	${SOURCES_PREFIX}/atwinc1500/driver/source/m2m_ate_mode.c
	${SOURCES_PREFIX}/atwinc1500/driver/source/m2m_ota.c
	${SOURCES_PREFIX}/atwinc1500/driver/source/nmasic.h
	${SOURCES_PREFIX}/atwinc1500/driver/source/nmi2c.h
	${SOURCES_PREFIX}/atwinc1500/driver/source/m2m_wifi.c
	${SOURCES_PREFIX}/atwinc1500/driver/source/nmbus.c
	${SOURCES_PREFIX}/atwinc1500/driver/source/nmspi.c
	${SOURCES_PREFIX}/atwinc1500/driver/source/m2m_crypto.c
	${SOURCES_PREFIX}/atwinc1500/driver/source/m2m_periph.c
	${SOURCES_PREFIX}/atwinc1500/driver/source/nmbus.h
	${SOURCES_PREFIX}/atwinc1500/driver/source/nmspi.h
	${SOURCES_PREFIX}/atwinc1500/driver/source/nmdrv.c
	${SOURCES_PREFIX}/atwinc1500/driver/source/nmuart.c
	${SOURCES_PREFIX}/atwinc1500/driver/source/m2m_hif.c
	${SOURCES_PREFIX}/atwinc1500/driver/source/m2m_ssl.c
	${SOURCES_PREFIX}/atwinc1500/driver/source/nmdrv.h
	${SOURCES_PREFIX}/atwinc1500/driver/source/nmuart.h

	${SOURCES_PREFIX}/atwinc1500/common/include/nm_common.h
	${SOURCES_PREFIX}/atwinc1500/common/include/nm_debug.h
	${SOURCES_PREFIX}/atwinc1500/common/source/nm_common.c

	${SOURCES_PREFIX}/atwinc1500/bsp/include/nm_bsp.h
	${SOURCES_PREFIX}/atwinc1500/bsp/include/nm_bsp_toolbox.h
	${SOURCES_PREFIX}/atwinc1500/bsp/include/nm_bsp_internal.h
	${SOURCES_PREFIX}/atwinc1500/bsp/source/nm_bsp_toolbox.c

	${SOURCES_PREFIX}/atwinc1500/bus_wrapper/source/nm_bus_wrapper.c
	${SOURCES_PREFIX}/atwinc1500/bus_wrapper/include/nm_bus_wrapper.h

	${SOURCES_PREFIX}/atwinc1500/spi_flash/source/spi_flash.c
	${SOURCES_PREFIX}/atwinc1500/spi_flash/include/spi_flash_map.h
	${SOURCES_PREFIX}/atwinc1500/spi_flash/include/spi_flash.h

	${SOURCES_PREFIX}/atwinc1500/socket/source/m2m_socket.c
	${SOURCES_PREFIX}/atwinc1500/socket/source/socket_internal.h
	${SOURCES_PREFIX}/atwinc1500/socket/include/m2m_socket.h
	${SOURCES_PREFIX}/atwinc1500/socket/include/m2m_socket_host_if.h


	PARENT_SCOPE)
