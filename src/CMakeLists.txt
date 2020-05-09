
set(SOURCES
	kernel/kernel_loader.c
	kernel/kernel_loader.h
	kernel/kernel_request.c
	kernel/kernel_io.c
	kernel/kernel_io.h
	kernel/kernel_touch.c
	kernel/kernel_touch.h
	kernel/kernel_app.c
	kernel/kernel_app.h
	kernel/kernel_dac.c
	kernel/kernel_dac.h
	kernel/kernel_service.c
	kernel/kernel_service.h
	kernel/kernel_shared.c
	kernel/kernel_shared.h
	kernel/wifi_api.c
	kernel/wifi_api.h
	kernel/i2c_internal.c
	kernel/i2c_internal.h
	kernel/ft5446.c
	kernel/ft5446.h
	kernel/pcal6416a.c
	kernel/pcal6416a.h

	devfs/st7789h2.c
	devfs/st7789h2.h
	devfs/display_device.c
	devfs/display_device.h
	devfs/lcd.c
	devfs/lcd.h
	devfs/swd_device.c
	devfs/swd_device.h
	devfs/spi_device.c
	devfs/spi_device.h
	devfs/uart_device.c
	devfs/uart_device.h
	devfs/i2c_device.c
	devfs/i2c_device.h
	devfs/i2s_device.c
	devfs/i2s_device.h
	devfs/logic_device.c
	devfs/logic_device.h
	devfs/pio_device.c
	devfs/pio_device.h
	devfs/wifi_device.c
	devfs/wifi_device.h
	devfs/wifi_phy_device.c
	devfs/wifi_phy_device.h

	mcu/stm32_bsp.c
	mcu/stm32_bsp.h
	mcu/SystemClock_Config.c

	board_config.c
	board_config.h
	config.h
	devfs_config.c
	link_config.c
	link_config.h
	mcu_config.c
	sos_config.c
	board_symbols.c
	sl_config.h

	usb/toolbox_usb_class.c
	usb/toolbox_usb_class.h

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
