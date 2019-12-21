
#include <mcu/pio.h>
#include "kernel/kernel_io.h"

#include "pio_device.h"


#if 0
const mcu_pin_t pin_table[TOOLBOX_SWD_PIN_COUNT] = {
   { .port = 2, .pin = 8 }, //IO1 SCL
   { .port = 1, .pin = 8 }, //IO1 RX
   { .port = 0, .pin = 1 }, //IO2 SDA
   { .port = 1, .pin = 9 }, //IO2 TX
   { .port = 4, .pin = 11 }, //IO3 CS
   { .port = 1, .pin = 10 }, //IO3 SCL
   { .port = 4, .pin = 12 }, //IO4 SCK
   { .port = 1, .pin = 11 }, //IO4 SDA
   { .port = 4, .pin = 13 }, //IO5
   { .port = 2, .pin = 6 }, //IO5
   { .port = 4, .pin = 14 }, //IO6
   { .port = 2, .pin = 7 }, //IO6
   { .port = 0, .pin = 2 }, //IO8
   { .port = 1, .pin = 5 }, //IO8
   { .port = 0, .pin = 3 }, //IO9
   { .port = 4, .pin = 3 }, //IO9
   { .port = 0, .pin = 6 }, //IO10
   { .port = 4, .pin = 4 }, //IO10
   { .port = 0, .pin = 7 }, //IO11
   { .port = 4, .pin = 5 }, //IO11
   { .port = 0, .pin = 15 }, //IO12
   { .port = 4, .pin = 6 }, //IO12
   { .port = 0, .pin = 10 }, //IO13
   { .port = 0, .pin = 10 }, //IO13 -- special case -- only one value

   //SWD
   { .port = 0, .pin = 9 }, //SWDIO_TMS
   { .port = 1, .pin = 13 }, //SWDIO_TCK
   { .port = 3, .pin = 2 }, //SWDIO_TDO
   { .port = 3, .pin = 3 }, //SWDIO_TDI
   { .port = 3, .pin = 6 }, //SWDIO_RESET
   { .port = 4, .pin = 0 }, //RTCK
};
#endif


#if 0
typedef struct {
   GPIO_TypeDef * gpio;
   u32 pin_mask;
} pio_device_entry_t;


const pio_device_entry_t pio_table[TOOLBOX_IO_PIN_COUNT + TOOLBOX_SWD_PIN_COUNT] = {
   { GPIOC, GPIO_PIN_8 }, //IO1 SCL
   { GPIOB, GPIO_PIN_8 }, //IO1 RX
   { GPIOA, GPIO_PIN_1 }, //IO2 SDA
   { GPIOB, GPIO_PIN_9 }, //IO2 TX
   { GPIOE, GPIO_PIN_11 }, //IO3 CS
   { GPIOB, GPIO_PIN_10 }, //IO3 SCL
   { GPIOE, GPIO_PIN_12 }, //IO4 SCK
   { GPIOB, GPIO_PIN_11 }, //IO4 SDA
   { GPIOE, GPIO_PIN_13 }, //IO5
   { GPIOC, GPIO_PIN_6 }, //IO5
   { GPIOE, GPIO_PIN_14 }, //IO6
   { GPIOC, GPIO_PIN_7 }, //IO6
   { GPIOA, GPIO_PIN_2 }, //IO8
   { GPIOB, GPIO_PIN_5 }, //IO8
   { GPIOA, GPIO_PIN_3 }, //IO9
   { GPIOE, GPIO_PIN_3 }, //IO9
   { GPIOA, GPIO_PIN_6 }, //IO10
   { GPIOE, GPIO_PIN_4 }, //IO10
   { GPIOA, GPIO_PIN_7 }, //IO11
   { GPIOE, GPIO_PIN_5 }, //IO11
   { GPIOA, GPIO_PIN_15 }, //IO12
   { GPIOE, GPIO_PIN_6 }, //IO12
   { GPIOA, GPIO_PIN_10 }, //IO13
   { GPIOA, GPIO_PIN_10 }, //IO13 -- special case -- only one value

   //SWD
   { GPIOA, GPIO_PIN_9 }, //SWDIO_TMS
   { GPIOB, GPIO_PIN_13 }, //SWDIO_TCK
   { GPIOD, GPIO_PIN_2 }, //SWDIO_TDO
   { GPIOD, GPIO_PIN_3 }, //SWDIO_TDI
   { GPIOD, GPIO_PIN_6 }, //SWDIO_RESET
   { GPIOE, GPIO_PIN_0 }, //RTCK

};
#endif

int pio_device_open(
      const devfs_handle_t * handle
      ){

   return SYSFS_SET_RETURN(EINVAL);
}

int pio_device_close(
      const devfs_handle_t * handle
      ){
   MCU_UNUSED_ARGUMENT(handle);
   return SYSFS_SET_RETURN(EINVAL);
}

int pio_device_ioctl(
      const devfs_handle_t * handle,
      int request,
      void * ctl
      ){
   //verify with gate that IO mode is enabled
}

int pio_device_read(
      const devfs_handle_t * handle,
      devfs_async_t * async){
   return SYSFS_SET_RETURN(ENOTSUP);
}

int pio_device_write(
      const devfs_handle_t * handle,
      devfs_async_t * async
      ){
   return SYSFS_SET_RETURN(ENOTSUP);
}
