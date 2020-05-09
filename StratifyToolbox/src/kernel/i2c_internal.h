#ifndef KERNEL_I2C_INTERNAL_H
#define KERNEL_I2C_INTERNAL_H

#include <mcu/types.h>

int i2c_internal_init();
int i2c_internal_lock(u8 slave_address);
int i2c_internal_unlock();

int i2c_internal_write_register(u8 address, u8 value);
u8 i2c_internal_read_register(u8 address);

int i2c_internal_write(int location, const void * buffer, int nbyte);
int i2c_internal_read(int location, void * buffer, int nbyte);


#endif // KERNEL_I2C_INTERNAL_H
