#ifndef WIFI_PHY_DEVICE_H
#define WIFI_PHY_DEVICE_H

#include <mcu/arch.h>
#include <sos/fs/devfs.h>
#include <mcu/spi.h>

typedef struct {
	stm32_spi_dma_config_t spi_dma_config;
	u8 reset_pin_number;
	u8 chip_select_pin_number;
	u8 chip_enable_pin_number;
	devfs_handle_t chip_select_handle;
	devfs_handle_t reset_handle;
	devfs_handle_t chip_enable_handle;
} wifi_phy_config_t;

typedef struct {
	mcu_event_handler_t transfer_event_handler;
	int is_initialized;
} wifi_state_t;


#define WIFI_PHY_IDENT_CHAR 'p'
#define I_WIFIPHY_GETVERSION _IOCTL(WIFI_PHY_IDENT_CHAR, I_MCU_GETVERSION)
#define I_WIFIPHY_GETINFO _IOCTLR(WIFI_PHY_IDENT_CHAR, I_MCU_GETINFO, u32)
#define I_WIFIPHY_SETATTR _IOCTLW(WIFI_PHY_IDENT_CHAR, I_MCU_SETATTR, u32)
#define I_WIFIPHY_SETACTION _IOCTLW(WIFI_PHY_IDENT_CHAR, I_MCU_SETACTION, mcu_action_t)
#define I_WIFIPHY_ASSERT_RESET _IOCTL(WIFI_PHY_IDENT_CHAR, I_MCU_TOTAL+1)
#define I_WIFIPHY_DEASSERT_RESET _IOCTL(WIFI_PHY_IDENT_CHAR, I_MCU_TOTAL+2)
#define I_WIFIPHY_SET_FULL_DUPLEX _IOCTL(WIFI_PHY_IDENT_CHAR, I_MCU_TOTAL+3)
#define I_WIFIPHY_SET_HALF_DUPLEX _IOCTL(WIFI_PHY_IDENT_CHAR, I_MCU_TOTAL+4)


int wifi_phy_device_open(const devfs_handle_t * handle);
int wifi_phy_device_close(const devfs_handle_t * handle);
int wifi_phy_device_ioctl(const devfs_handle_t * handle, int request, void * ctl);
int wifi_phy_device_read(const devfs_handle_t * handle, devfs_async_t * async);
int wifi_phy_device_write(const devfs_handle_t * handle, devfs_async_t * async);


#endif // WIFI_PHY_DEVICE_H
