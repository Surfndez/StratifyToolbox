
#include <device/sys.h>
#include <mcu/debug.h>
#include <sapi/sg.h>
#include <cortexm/mpu.h>
#include <mcu/arch.h>
#include <mcu/arch/stm32/stm32h7xx/stm32h7xx_hal.h>
#include "display_device.h"
#include "st7789h2.h"

static int m_is_initialized MCU_SYS_MEM;

#define DISPLAY_PALETTE_BITS_PER_PIXEL 4
#define DISPLAY_PALETTE_COLOR_COUNT (1<<DISPLAY_PALETTE_BITS_PER_PIXEL)
static display_palette_t m_display_palette MCU_SYS_MEM;
static u16 m_display_colors[DISPLAY_PALETTE_COLOR_COUNT];
static u32 m_o_flags MCU_SYS_MEM;
static sg_region_t m_window;

static int display_device_getinfo(const devfs_handle_t * handle, void * ctl);
static int display_device_clear(const devfs_handle_t * handle, void * ctl);
static int display_device_enable(const devfs_handle_t * handle, void * ctl);
static int display_device_disable(const devfs_handle_t * handle, void * ctl);
static int display_device_isbusy(const devfs_handle_t * handle, void * ctl);

int display_device_open(const devfs_handle_t * handle){
	return SYSFS_RETURN_SUCCESS;
}

int display_device_close(const devfs_handle_t * handle){
	return SYSFS_RETURN_SUCCESS;
}

int display_device_ioctl(const devfs_handle_t * handle, int request, void * ctl){
	display_attr_t * attr = ctl;
	int result;

	switch(request){
		case I_DISPLAY_GETVERSION: return DISPLAY_VERSION;
		case I_DISPLAY_GETINFO: return display_device_getinfo(handle, ctl);

		case I_DISPLAY_SETATTR:

			if( attr->o_flags & DISPLAY_FLAG_INIT ){
				result = display_device_init(handle, ctl);
				if( result < 0 ){ return result; }
			}

			if( attr->o_flags & DISPLAY_FLAG_ENABLE ){
				result = display_device_enable(handle, ctl);
				if( result < 0 ){ return result; }
			}

			if( attr->o_flags & DISPLAY_FLAG_DISABLE ){
				result = display_device_disable(handle, ctl);
				if( result < 0 ){ return result; }
			}

			if( attr->o_flags & DISPLAY_FLAG_CLEAR ){
				result = display_device_clear(handle, ctl);
				if( result < 0 ){ return result; }
			}

			if( attr->o_flags & DISPLAY_FLAG_SET_MODE ){
				m_o_flags = attr->o_flags;
			}

			if( attr->o_flags & DISPLAY_FLAG_SET_WINDOW ){
				m_window.point.x = attr->window_x;
				m_window.point.y = attr->window_y;
				m_window.area.width = attr->window_width;
				m_window.area.height = attr->window_height;
			}
			return 0;

		case I_DISPLAY_ISBUSY: return display_device_isbusy(handle, ctl);
		case I_DISPLAY_GETPALETTE:
			memcpy(ctl, &m_display_palette, sizeof(m_display_palette));
			return SYSFS_RETURN_SUCCESS;
	}

	return SYSFS_SET_RETURN(EINVAL);
}

int display_device_read(const devfs_handle_t * handle, devfs_async_t * async){
	return SYSFS_SET_RETURN(ENOTSUP);
}

int display_device_write(const devfs_handle_t * handle, devfs_async_t * async){

	return async->nbyte;
}

int display_device_getinfo(const devfs_handle_t * handle, void * ctl){
	MCU_UNUSED_ARGUMENT(handle);
	display_info_t * info = ctl;
	//these are the flags that are supported by the driver
	info->o_flags = DISPLAY_FLAG_CLEAR |
			DISPLAY_FLAG_DISABLE |
			DISPLAY_FLAG_ENABLE |
			DISPLAY_FLAG_INIT |
			DISPLAY_FLAG_SET_MODE |
			DISPLAY_FLAG_SET_WINDOW |
			DISPLAY_FLAG_IS_MODE_PALETTE |
			DISPLAY_FLAG_IS_MODE_RAW;

	info->width = DISPLAY_WIDTH;
	info->height = DISPLAY_HEIGHT;
	info->cols = DISPLAY_WIDTH;
	info->rows = DISPLAY_HEIGHT;
	info->freq = 30;
	info->bits_per_pixel = DISPLAY_PALETTE_BITS_PER_PIXEL;
	info->margin_left = 8;
	info->margin_right = 8;
	info->margin_top = 0;
	info->margin_bottom = 0;
	return SYSFS_RETURN_SUCCESS;
}

int display_device_clear(const devfs_handle_t * handle, void * ctl){
	MCU_UNUSED_ARGUMENT(handle);
	MCU_UNUSED_ARGUMENT(ctl);

	for(u32 i=0; i < DISPLAY_HEIGHT; i++){
		ST7789H2_DrawHLine(m_display_colors[0], 0, i, ST7789H2_LCD_PIXEL_WIDTH);
	}

	return SYSFS_RETURN_SUCCESS;
}

int display_device_init(const devfs_handle_t * handle, void * ctl){
	//initialize and clear the display

	if( m_is_initialized == 0 ){
		mcu_debug_printf("initialize LCD\n");
		ST7789H2_Init();
		m_is_initialized = 1;
	}

	m_o_flags = DISPLAY_FLAG_IS_MODE_PALETTE;

	m_window.point.point = 0;
	m_window.area.width = DISPLAY_WIDTH;
	m_window.area.height = DISPLAY_HEIGHT;

	m_display_palette.pixel_format = DISPLAY_PALETTE_PIXEL_FORMAT_RGB565;
	m_display_palette.count = DISPLAY_PALETTE_COLOR_COUNT;
	m_display_palette.colors = m_display_colors;

	//set the default palette
	m_display_colors[0] = 0x0004;
	m_display_colors[1] = 0x0008;
	m_display_colors[2] = 0x0010;
	//m_display_colors[3] = LCD_COLOR_BLUE; //0x001F

	m_display_colors[4] = 0x0040;
	m_display_colors[5] = 0x00E0;
	m_display_colors[6] = 0x03E0;
	//m_display_colors[7] = LCD_COLOR_GREEN;

	m_display_colors[8] = 0x0400;
	m_display_colors[9] = 0x0800;
	m_display_colors[10] = 0x7800;
	//m_display_colors[11] = LCD_COLOR_RED;

	m_display_colors[12] = 0x0000;
	m_display_colors[13] = 0x6666;
	m_display_colors[14] = 0xCCCC;
	//m_display_colors[15] = LCD_COLOR_WHITE;


	for(u32 i=0; i < DISPLAY_HEIGHT; i++){
		//ST7789H2_DrawHLine(LCD_COLOR_WHITE, 0, i, ST7789H2_LCD_PIXEL_WIDTH);
	}
	return 0;
}

int display_device_enable(const devfs_handle_t * handle, void * ctl){
	//ST7789H2_WriteReg(ST7789H2_DISPLAY_ON, (uint8_t*)NULL, 0);
	return 0;
}

int display_device_disable(const devfs_handle_t * handle, void * ctl){
	//ST7789H2_WriteReg(ST7789H2_DISPLAY_OFF, (uint8_t*)NULL, 0);
	return 0;
}

int display_device_isbusy(const devfs_handle_t * handle, void * ctl){
	return 0;
}




