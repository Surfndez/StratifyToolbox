
#include <stdio.h>

#include <ToolboxAPI/toolbox_touch.h>
#include "kernel_touch.h"
#include "kernel_shared.h"

#include "ft5446.h"

static int is_pressed(const ft5446_touch_t * touch){
	return (touch->x_high & 0xC0) == 0x00;
}

static int is_active(const ft5446_touch_t * touch){
	return (touch->x_high & 0xC0) == 0x80;
}

static u16 x(const ft5446_touch_t * touch){
	u16 device_y = ((touch->y_high << 8)|touch->y_low) & 0x0fff;
	return device_y; //device y is the x axis
}

static u16 y(const ft5446_touch_t * touch){
	u16 device_x = ((touch->x_high << 8)|touch->x_low) & 0x0fff;
	return 239 - device_x;
}

void translate_touch(toolbox_touch_t * toolbox_touch, const ft5446_touch_t * device_touch){

	toolbox_touch->x = x(device_touch);
	toolbox_touch->y = y(device_touch);
	toolbox_touch->o_flags = 0;

	if( is_pressed(device_touch) ){
		toolbox_touch->o_flags |= TOOLBOX_TOUCH_FLAG_IS_PRESSED;
	}

	if( is_active(device_touch) ){
		toolbox_touch->o_flags |= TOOLBOX_TOUCH_FLAG_IS_ACTIVE;
	}

	toolbox_touch->pressure = device_touch->pressure;
	toolbox_touch->area = device_touch->area;
}

int kernel_touch_init(){
	kernel_shared_last_touch_packet()->count = 0;
	return 0;
}

int kernel_touch_request(void * args){
   toolbox_touch_packet_t * touch_packet = args;
   ft5446_packet_t device_packet;

   touch_packet->count = ft5446_read(&device_packet);

   int count = touch_packet->count;

   for(int i = 0; i < 2; i++){
      if( touch_packet->count < i+1 ){
         //check to see if the last touch was active and release it
         if( kernel_shared_last_touch_packet()->count > i ){
            touch_packet->point[i] = kernel_shared_last_touch_packet()->point[i];
            touch_packet->point[i].o_flags = TOOLBOX_TOUCH_FLAG_IS_RELEASED;
            count = i+1;
         }
      } else {
         //this is an active touch
         translate_touch(touch_packet->point + i, device_packet.point + i);
      }
   }

   //last packet is copied without adjusted count
   *(kernel_shared_last_touch_packet()) = *touch_packet;

   //adjust the count that is returned to account for release events
   touch_packet->count = count;
   return count;
}
