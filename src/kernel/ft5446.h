#ifndef KERNEL_FT5446_H
#define KERNEL_FT5446_H

#include <mcu/types.h>

typedef struct MCU_PACK {
	u8 x_high;
	u8 x_low;
	u8 y_high;
	u8 y_low;
	u8 w;
	u8 m;
} ft5446_touch_t;

typedef struct MCU_PACK {
	u8 count;
	ft5446_touch_t point[2];
} ft5446_packet_t;

int ft5446_init();
int ft5446_read(ft5446_packet_t * touch_packet);

#endif // KERNEL_FT5446_H
