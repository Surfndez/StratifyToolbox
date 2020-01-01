
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
static u16 m_row_buffer[DISPLAY_WIDTH];
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

		case I_DISPLAY_CLEAR:
			display_device_clear(handle, ctl);
			return SYSFS_RETURN_SUCCESS;

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

				if( m_window.point.x + m_window.area.width > DISPLAY_WIDTH ){
					if( m_window.point.x >= DISPLAY_WIDTH ){
						m_window.point.x = 0;
					}
					m_window.area.width = DISPLAY_WIDTH - m_window.point.x;
				}

				if( m_window.point.y + m_window.area.height > DISPLAY_HEIGHT ){
					if( m_window.point.y >= DISPLAY_HEIGHT ){
						m_window.point.y = 0;
					}
					m_window.area.height = DISPLAY_HEIGHT - m_window.point.y;
				}

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

	ST7789H2_SetWindow(
				m_window.point.x,
				m_window.point.y,
				m_window.area.width,
				m_window.area.height
				);

	ST7789H2_WriteReg(ST7789H2_WRITE_RAM, (uint8_t*)NULL, 0);   /* RAM write data command */

	if( m_o_flags & DISPLAY_FLAG_IS_MODE_PALETTE ){
		sg_cursor_t x_cursor;
		sg_cursor_t y_cursor;
		sg_bmap_t * bmap = async->buf;

		//is the bmap memory in the application memory space
		//devfs will check async->buf, but async->buf points to another bmap
		if( task_validate_memory(bmap->data, sg_calc_bmap_size(bmap, bmap->area)) < 0 ){
			MCU_DEBUG_LINE_TRACE();
			return SYSFS_SET_RETURN(EPERM);
		}

		//bitmap must be at least as big as the window
		if( bmap->area.width < m_window.area.width ){
			MCU_DEBUG_LINE_TRACE();
			return SYSFS_SET_RETURN(EINVAL);
		}

		if( bmap->area.height < m_window.area.height ){
			MCU_DEBUG_LINE_TRACE();
			return SYSFS_SET_RETURN(EINVAL);
		}

		//the entire whole bitmap is written to the window
		sg_cursor_set(&y_cursor, bmap, sg_point(0,0));

		/* Prepare to write to LCD RAM */

		for(sg_size_t h=0; h < m_window.area.height; h++){
			sg_cursor_copy(&x_cursor, &y_cursor);
			for(sg_size_t w=0; w < m_window.area.width; w++){
				sg_color_t color = sg_cursor_get_pixel_increment(&x_cursor, 1, 0);
				//get the LCD color from the palette
				m_row_buffer[w] = m_display_colors[color & 0x0f];
			}
			//use a row buffer
			LCD_IO_WriteDataBlockRgb(
						(u8*)m_row_buffer,
						m_window.area.width*2
						);

			sg_cursor_inc_y(&y_cursor);
		}
	} else {
		//in raw mode just write data directly to the LCD
		//-- useful for writing images
		LCD_IO_WriteDataBlockRgb(
					async->buf,
					async->nbyte
					);
	}
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
	m_display_colors[0] = ST7789H2_RGB(0,0,0);
	m_display_colors[1] = ST7789H2_RGB(10,10,10);
	m_display_colors[2] = ST7789H2_RGB(20,20,20);
	m_display_colors[3] = ST7789H2_RGB(31,31,31); //White

	m_display_colors[4] = ST7789H2_RGB(8,0,0);
	m_display_colors[5] = ST7789H2_RGB(16,0,0);
	m_display_colors[6] = ST7789H2_RGB(24,0,0);
	m_display_colors[7] = ST7789H2_RGB(31,0,0);  //Red

	m_display_colors[8] = ST7789H2_RGB(0,0,8);
	m_display_colors[9] = ST7789H2_RGB(0,0,16);
	m_display_colors[10] = ST7789H2_RGB(0,0,24);
	m_display_colors[11] = ST7789H2_RGB(0,0,31); //Blue

	m_display_colors[12] = ST7789H2_RGB(0,8,0);
	m_display_colors[13] = ST7789H2_RGB(0,16,0);
	m_display_colors[14] = ST7789H2_RGB(0,24,0);
	m_display_colors[15] = ST7789H2_RGB(0,31,0); //Green


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




