/*

Copyright 2011-2018 Stratify Labs, Inc

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.

*/


#include <mcu/types.h>
#include <mcu/core.h>
#include <mcu/bootloader.h>
#include <mcu/arch.h>
#include <mcu/pio.h>
#include <mcu/boot_debug.h>

#include "boot_link_config.h"
#include "board_config.h"


const struct __sFILE_fake __sf_fake_stdin;
const struct __sFILE_fake __sf_fake_stdout;
const struct __sFILE_fake __sf_fake_stderr;

#include "../src/config.h"
//Bootloader configuration (don't need Stratify OS configuration for just the bootloader)

extern u32 _flash_start; 
const bootloader_board_config_t boot_board_config = { 
    .sw_req_loc = 0x20004000, 
    .sw_req_value = 0x55AA55AA, 
    .program_start_addr = 0x40000 + (u32)&_flash_start, 
    .hw_req = {BOOT_HARDWARE_REQUEST_PORT, BOOT_HARDWARE_REQUEST_PIN},
    .o_flags = BOOT_BOARD_CONFIG_FLAG_HW_REQ_ACTIVE_HIGH, 
    .link_transport_driver = &link_transport, 
    .id = __HARDWARE_ID 
};

extern void boot_main();

//Execute the Stratify OS default bootloader
void _main(){ boot_main(); }

void board_event_handler(int event, void * args){
    devfs_handle_t pio_handle;
    pio_attr_t pio_attr;

    switch(event){
    case BOOT_EVENT_INIT:
        //This sets the USB HS Pins to high impedance so they don't interfere with USB FS during development
        pio_handle.port = 1; //PORTB
        pio_handle.config = 0;
        pio_handle.state = 0;
        pio_attr.o_flags = PIO_FLAG_SET_INPUT | PIO_FLAG_IS_FLOAT;
        pio_attr.o_pinmask = (1<<14) | (1<<15);
        mcu_pio_setattr(&pio_handle, &pio_attr);
        break;
    }
}
