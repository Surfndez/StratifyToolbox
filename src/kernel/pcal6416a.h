#ifndef PCAL6416A_HPP
#define PCAL6416A_HPP

#include <sos/fs/sysfs.h>
#include <sos/dev/pio.h>

#define PCAL6116A_PORTA 0x20
#define PCAL6116A_PORTB 0x21

int pcal6416a_init();
int pcal6416a_setattr(u8 slave_address, const pio_attr_t * attributes);
u16 pcal6416a_read(u8 slave_address);
int pcal6416a_setmask(u8 slave_address, u16 o_mask);
int pcal6416a_clrmask(u8 slave_address, u16 o_mask);


#endif // PCAL6416A_HPP
