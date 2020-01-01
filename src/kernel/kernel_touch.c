
#include <ToolboxAPI/toolbox_touch.h>
#include "kernel_touch.h"

#include "ft5446.h"

static int is_pressed(const ft5446_touch_t * touch){
	return (touch->x_high & 0xC0) == 0x00;
}

static int is_released(const ft5446_touch_t * touch){
	return (touch->x_high & 0xC0) == 0x40;
}

static int is_active(const ft5446_touch_t * touch){
	return (touch->x_high & 0xC0) == 0x80;
}

static int touch_id(const ft5446_touch_t * touch){
	return touch->y_high & 0xE0;
}

static u16 x(const ft5446_touch_t * touch){
	return ((touch->x_high << 8)|touch->x_low) & 0x0fff;
}

static u16 y(const ft5446_touch_t * touch){
	return ((touch->y_high << 8)|touch->y_low) & 0x0fff;
}

void translate_touch(toolbox_touch_t * toolbox_touch, const ft5446_touch_t * device_touch){

	toolbox_touch->x = x(device_touch);
	toolbox_touch->y = y(device_touch);
	if( is_pressed(device_touch) ){
		toolbox_touch->o_flags |= TOOLBOX_TOUCH_FLAG_IS_PRESSED;
	}

	if( is_released(device_touch) ){
		toolbox_touch->o_flags |= TOOLBOX_TOUCH_FLAG_IS_RELEASED;
	}

	if( is_active(device_touch) ){
		toolbox_touch->o_flags |= TOOLBOX_TOUCH_FLAG_IS_ACTIVE;
	}

	toolbox_touch->id = touch_id(device_touch);
}

int kernel_touch_request(void * args){
   toolbox_touch_packet_t * touch_packet = args;
   ft5446_packet_t device_packet;

   int count = ft5446_read(&device_packet);
   if( count == 0 ){
      touch_packet->count = 0;
      return 0;
   }


   translate_touch(touch_packet->point, device_packet.point);
   translate_touch(touch_packet->point+1, device_packet.point+1);

   touch_packet->count = device_packet.count;
   return touch_packet->count;
}
