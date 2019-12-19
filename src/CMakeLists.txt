
set(SOURCES
	kernel_loader.c
	kernel_loader.h
	board_config.c
	board_config.h
	config.h
	display_device.c
	display_device.h
	st7789h2.c
	st7789h2.h
	lcd.c
	lcd.h
	devfs_config.c
	pio_ext_device.c
	pio_ext_device.h
	link_config.c
	link_config.h
	mcu_config.c
	sos_config.c
	board_symbols.c
	stm32_bsp.c
	stm32_bsp.h
	sl_config.h
	kernel_request.c
	SystemClock_Config.c
	wifi_api.c
	wifi_api.h
	wifi_device.c
	wifi_device.h
	wifi_phy_device.c
	wifi_phy_device.h
	atwinc1500/driver/include/ecc_types.h
	atwinc1500/driver/include/m2m_ssl.h
	atwinc1500/driver/include/m2m_crypto.h
	atwinc1500/driver/include/m2m_ate_mode.h
	atwinc1500/driver/include/m2m_ota.h
	atwinc1500/driver/include/m2m_types.h
	atwinc1500/driver/include/m2m_wifi.h
	atwinc1500/driver/include/m2m_periph.h

	atwinc1500/driver/source/m2m_hif.h
	atwinc1500/driver/source/nmasic.c
	atwinc1500/driver/source/nmi2c.c
	atwinc1500/driver/source/m2m_ate_mode.c
	atwinc1500/driver/source/m2m_ota.c
	atwinc1500/driver/source/nmasic.h
	atwinc1500/driver/source/nmi2c.h
	atwinc1500/driver/source/m2m_wifi.c
	atwinc1500/driver/source/nmbus.c
	atwinc1500/driver/source/nmspi.c
	atwinc1500/driver/source/m2m_crypto.c
	atwinc1500/driver/source/m2m_periph.c
	atwinc1500/driver/source/nmbus.h
	atwinc1500/driver/source/nmspi.h
	atwinc1500/driver/source/nmdrv.c
	atwinc1500/driver/source/nmuart.c
	atwinc1500/driver/source/m2m_hif.c
	atwinc1500/driver/source/m2m_ssl.c
	atwinc1500/driver/source/nmdrv.h
	atwinc1500/driver/source/nmuart.h

	atwinc1500/common/include/nm_common.h
	atwinc1500/common/include/nm_debug.h
	atwinc1500/common/source/nm_common.c

	atwinc1500/bsp/include/nm_bsp.h
	atwinc1500/bsp/include/nm_bsp_toolbox.h
	atwinc1500/bsp/include/nm_bsp_internal.h
	atwinc1500/bsp/source/nm_bsp_toolbox.c

	atwinc1500/bus_wrapper/source/nm_bus_wrapper.c
	atwinc1500/bus_wrapper/include/nm_bus_wrapper.h

	atwinc1500/spi_flash/source/spi_flash.c
	atwinc1500/spi_flash/include/spi_flash_map.h
	atwinc1500/spi_flash/include/spi_flash.h

	atwinc1500/socket/source/m2m_socket.c
	atwinc1500/socket/source/socket_internal.h
	atwinc1500/socket/include/m2m_socket.h
	atwinc1500/socket/include/m2m_socket_host_if.h


	PARENT_SCOPE)
