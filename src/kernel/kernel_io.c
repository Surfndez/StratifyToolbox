
#include <errno.h>
#include <mcu/core.h>
#include <mcu/pio.h>
#include "kernel_shared.h"
#include "kernel_io.h"
#include "pcal6416a.h"


#define IN PIO_FLAG_SET_INPUT
#define OUT PIO_FLAG_SET_OUTPUT

#if defined ___debug
#define DEBUG_UART_DIRECTION OUT
#else
#define DEBUG_UART_DIRECTION IN
#endif

typedef struct {
	u8 port;
	u8 pin;
	u8 slave_addr;
	u8 slave_pin;
} toolbox_pin_t;


typedef struct MCU_PACK {
	u8 expansion_port;
	u8 expansion_pin;
	u8 init_state;
	u8 resd; //alignment
	u32 init_flags;
} kernel_io_pin_t;

const kernel_io_pin_t internal_pin_table[kernel_io_last+1] = {
	{ PCAL6116A_PORTA, 4+8, 0, 0, OUT }, //kernel_io_wifi_reset
	{ PCAL6116A_PORTA, 5+8, 0, 0, OUT }, //kernel_io_wifi_chip_enable
	{ PCAL6116A_PORTA, 6+8, 0, 0, OUT }, //kernel_io_wifi_wake
	{ PCAL6116A_PORTB, 4+8, 0, 0, OUT }, //kernel_io_power_enable
	{ PCAL6116A_PORTB, 3+8, 0, 0, OUT }, //kernel_io_vbat_measure_enable
	{ PCAL6116A_PORTB, 2+8, 0, 0, IN | PIO_FLAG_IS_FLOAT }, //kernel_io_charge_fast_enable
	{ PCAL6116A_PORTB, 1+8, 0, 0, IN | PIO_FLAG_IS_PULLUP }, //kernel_io_charge_stat1
	{ PCAL6116A_PORTB, 0+8, 0, 0, IN | PIO_FLAG_IS_PULLUP }, //kernel_io_charge_stat2
	{ PCAL6116A_PORTB, 6+0, 0, 0, IN | PIO_FLAG_IS_FLOAT }, //kernel_io_charge_proximity_mode
	{ PCAL6116A_PORTB, 5+8, 0, 0, OUT }, //kernel_io_charge_vref_enable
	{ PCAL6116A_PORTB, 6+8, 1, 0, OUT }, //kernel_io_charge_div10_enable
	{ PCAL6116A_PORTB, 7+8, 1, 0, OUT }, //kernel_io_charge_vdd_out_enable
};

#define SPI(port) {CORE_PERIPH_SPI, port}
#define UART(port) {CORE_PERIPH_UART, port}
#define I2C(port) {CORE_PERIPH_I2C, port}
#define I2S(port) {CORE_PERIPH_I2S, port}
#define PIO(port) {CORE_PERIPH_PIO, port}
#define TMR(port) {CORE_PERIPH_TMR, port}
#define NONE(port) {CORE_PERIPH_RESERVED, port}

#define PA(x) {0, x}
#define PB(x) {1, x}
#define PC(x) {2, x}
#define PD(x) {3, x}
#define PE(x) {4, x}
#define PX(x) {0xff, x}

typedef struct MCU_PACK {
	u8 peripheral;
	u8 port;
} kernel_io_core_peripheral_t;

typedef struct MCU_PACK {
	u8 expansion_port;
	u8 expansion_pin;
	u8 resd; //alignment
	u8 init_flags;
	mcu_pin_t primary;
	mcu_pin_t secondary;
} kernel_io_external_pin_t;

static const kernel_io_external_pin_t external_pin_table[last_kernel_shared_direction_channel+1] = {
	{ PCAL6116A_PORTX, 0, 0, IN, PX(0), PX(0) }, //IO0 - doesn't exist
	{ PCAL6116A_PORTA, 0, 0, IN, PB(8), PX(0) }, //IO1 RX
	{ PCAL6116A_PORTA, 1, 0, DEBUG_UART_DIRECTION, PB(9), PX(0) }, //IO2 TX -- debugging output
	{ PCAL6116A_PORTA, 2, 0, IN, PE(11), PX(0) }, //IO3 CS
	{ PCAL6116A_PORTA, 3, 0, IN, PE(12), PX(0) }, //IO4 SCK
	{ PCAL6116A_PORTA, 4, 0, IN, PE(13), PC(6) }, //IO5 MISO
	{ PCAL6116A_PORTA, 5, 0, IN, PE(14), PC(7) }, //IO6 MOSI
	{ PCAL6116A_PORTX, 0, 0, 0, PX(0), PX(0) }, //IO7
	{ PCAL6116A_PORTA, 6, 0, IN, PA(2), PB(5) }, //IO8 TX
	{ PCAL6116A_PORTA, 7, 0, IN, PA(3), PE(3) }, //IO9
	{ PCAL6116A_PORTA, 8, 0, IN, PA(6), PE(4) }, //IO10
	{ PCAL6116A_PORTA, 1+8, 0, IN, PA(7), PE(5) }, //IO11
	{ PCAL6116A_PORTA, 2+8, 0, IN, PA(15), PE(6) }, //IO12
	{ PCAL6116A_PORTA, 3+8, 0, IN, PA(10), PX(0) }, //IO13
	{ PCAL6116A_PORTX, 3+8, 0, IN, PX(0), PX(0) }, //IO14
	{ PCAL6116A_PORTX, 3+8, 0, IN, PX(0), PX(0) }, //IO15
	{ PCAL6116A_PORTX, 3+8, 0, IN, PC(4), PX(0) }, //IO16
	{ PCAL6116A_PORTX, 3+8, 0, IN, PC(5), PX(0) }, //IO17
	{ PCAL6116A_PORTX, 3+8, 0, IN, PA(4), PX(0) }, //IO18
	{ PCAL6116A_PORTX, 3+8, 0, IN, PA(5), PX(0) }, //IO19
	{ PCAL6116A_PORTX, 3+8, 0, IN, PX(0), PX(0) }, //IO20
	{ PCAL6116A_PORTB, 5, 0, IN, PA(9), PX(0) }, //SWDIO_TMS --SWDIO
	{ PCAL6116A_PORTB, 0, 0, IN, PB(13), PX(0) }, //SWDIO_TCK
	{ PCAL6116A_PORTB, 1, 0, IN, PD(2), PX(0)  }, //SWDIO_TDO (UART5-RX)
	{ PCAL6116A_PORTB, 2, 0, IN, PD(3), PX(0) }, //SWDIO_TDI
	{ PCAL6116A_PORTB, 3, 0, IN, PD(6), PX(0) }, //SWDIO_RESET
	{ PCAL6116A_PORTB, 4, 0, IN, PE(0), PX(0) } //RTCK -- will probably not be available
};

static int init_internal_pins();
static int init_external_pins();
static int set_pin_direction(u32 pin_number, int io_flags);
static void svcall_set_pin_input(void * args);
static int set_io_function(const toolbox_io_attr_t * attributes);
static int acquire_pin(
		enum kernel_shared_direction_channels channel,
		u8 peripheral_function,
		u8 peripheral_port,
		u8 io_flags,
		const char * description
		);

static int get_pin_direction_mask();
static int get_io_info(toolbox_io_info_t * info);
static int get_io_pin_description(
		toolbox_io_pin_description_t * pin_description
		);

int kernel_io_init(){
	if( pcal6416a_init() < 0 ){
		mcu_debug_log_error(MCU_DEBUG_USER0, "Failed to initialize GPIO expander");
		return -1;
	}

	if( init_internal_pins() < 0 ){
		return -1;
	}

	if( init_external_pins() < 0 ){
		return -1;
	}

	acquire_pin(kernel_shared_direction_channel0, CORE_PERIPH_RESERVED, 0, IN, "nc");
	acquire_pin(kernel_shared_direction_channel7, CORE_PERIPH_RESERVED, 0, IN, "gnd");
	acquire_pin(kernel_shared_direction_channel14, CORE_PERIPH_RESERVED, 0, IN, "ref");
	acquire_pin(kernel_shared_direction_channel15, CORE_PERIPH_RESERVED, 0, IN, "gnd");
	acquire_pin(kernel_shared_direction_channel16, CORE_PERIPH_RESERVED, 0, IN, "adc0");
	acquire_pin(kernel_shared_direction_channel17, CORE_PERIPH_RESERVED, 0, IN, "adc1");
	acquire_pin(kernel_shared_direction_channel18, CORE_PERIPH_RESERVED, 0, IN, "dac0");
	acquire_pin(kernel_shared_direction_channel19, CORE_PERIPH_RESERVED, 0, IN, "dac1");
	acquire_pin(kernel_shared_direction_channel20, CORE_PERIPH_RESERVED, 0, IN, "gnd");

	mcu_debug_log_info(MCU_DEBUG_USER0, "kernel IO init complete");
	return 0;
}

int kernel_io_set(enum kernel_io_pins pin){
	const kernel_io_pin_t * io_pin = internal_pin_table + pin;
	return pcal6416a_setmask(
				io_pin->expansion_port,
				1<<io_pin->expansion_pin
				);
}

int kernel_io_clear(enum kernel_io_pins pin){
	const kernel_io_pin_t * io_pin = internal_pin_table + pin;
	return pcal6416a_clrmask(
				io_pin->expansion_port,
				1<<io_pin->expansion_pin
				);
}

int kernel_io_request(
		void * args
		){
	const toolbox_io_attr_t * attributes = args;

	if( attributes->o_flags & TOOLBOX_IO_FLAG_SET_IO_FUNCTION ){
		if( set_io_function(attributes) < 0 ){
			return -1;
		}
	} else if( attributes->o_flags & TOOLBOX_IO_FLAG_SET_SWD_FUNCTION ){

	} else if( attributes->o_flags & TOOLBOX_IO_FLAG_GET_DIRECTION ){
		return get_pin_direction_mask();
	} else if( attributes->o_flags & TOOLBOX_IO_FLAG_GET_INFO ){
		return get_io_info(args);
	} else if( attributes->o_flags & TOOLBOX_IO_FLAG_GET_PIN_DESCRIPTION ){
		return get_io_pin_description(args);
	}

	if( attributes->o_flags & TOOLBOX_IO_FLAG_ENABLE_DIV10_OUT ){
		kernel_io_clear(kernel_io_vref_enable); //active low
	} else if( attributes->o_flags & TOOLBOX_IO_FLAG_DISABLE_DIV10_OUT ){
		kernel_io_set(kernel_io_vref_enable); //active low
	}

	if( attributes->o_flags & TOOLBOX_IO_FLAG_ENABLE_VREF ){
		kernel_io_set(kernel_io_vref_enable); //active high
	} else if( attributes->o_flags & TOOLBOX_IO_FLAG_DISABLE_VREF ){
		kernel_io_clear(kernel_io_vref_enable);
	}

	if( attributes->o_flags & TOOLBOX_IO_FLAG_ENABLE_VDD_OUT ){
		kernel_io_clear(kernel_io_vdd_out_enable);
	} else if( attributes->o_flags & TOOLBOX_IO_FLAG_DISABLE_VDD_OUT ){
		kernel_io_set(kernel_io_vdd_out_enable);
	}

	return 0;
}

int kernel_io_is_direction_assigned(
		enum kernel_shared_direction_channels channel,
		u8 peripheral_function,
		u32 io_flags
		){


	const kernel_shared_direction_state_t * direction_state =
			kernel_shared_get_direction_state(channel);

	if( direction_state->peripheral_function
			!= peripheral_function ){
		mcu_debug_printf("function mismatch %d (%d != %d)\n",
										 channel,
										 direction_state->peripheral_function,
										 peripheral_function
										 );
		return SYSFS_SET_RETURN(EINVAL);
	}

	//if pin is OUT and voltage tranlastor is IN there will be two driving sources
	if( (io_flags & OUT) && (direction_state->io_flags & IN) ){
		mcu_debug_printf("dir conflict %d\n", channel);
		return SYSFS_SET_RETURN(EPROTO);
	}

	return 0;
}

typedef struct {
	u8 peripheral_function;
	u8 peripheral_port;
	u8 io_flags;
	enum kernel_shared_direction_channels direction_channel;
	const char * description;
	int result;
} svcall_acquire_pin_t;

void svcall_aquire_pin(void * args){
	CORTEXM_SVCALL_ENTER();
	svcall_acquire_pin_t * p = args;

	const kernel_shared_direction_state_t * direction_state =
			kernel_shared_get_direction_state(p->direction_channel);

	if( direction_state->peripheral_function == CORE_PERIPH_PIO ){

		kernel_shared_root_set_direction_state(
					p->direction_channel,
					p->peripheral_function,
					p->peripheral_port,
					p->io_flags,
					p->description
					);

	} else if( (p->peripheral_function != direction_state->peripheral_function) ||
						 (p->peripheral_port != direction_state->peripheral_port) ||
						 (p->io_flags != direction_state->io_flags) ){
		p->result = SYSFS_SET_RETURN(EINVAL);
	}
}

int acquire_pin(
		enum kernel_shared_direction_channels channel,
		u8 peripheral_function,
		u8 peripheral_port,
		u8 io_flags,
		const char * description
		){
	svcall_acquire_pin_t args;
	args.direction_channel = channel;
	args.io_flags = io_flags;
	args.peripheral_function = peripheral_function;
	args.peripheral_port = peripheral_port;
	args.description = description;
	args.result = 0;
	cortexm_svcall(svcall_aquire_pin, &args);
	return args.result;
}

int get_io_pin_description(
		toolbox_io_pin_description_t * pin_description
		){


	if( pin_description->pin_number ==
			kernel_shared_direction_channel0 ){
		return -1 * __LINE__;
	}

	if( pin_description->pin_number >
			last_kernel_shared_direction_channel ){
		return -1 * __LINE__;
	}

	//check kernel if pins are busy on a different peripheral
	u32 pin_number =  pin_description->pin_number;
	const kernel_shared_direction_state_t * state =
			kernel_shared_get_direction_state(pin_number);

	pin_description->peripheral_port = state->peripheral_port;
	pin_description->peripheral_function = state->peripheral_function;
	pin_description->is_slave = 0;
	if( state->description != 0 ){
		strncpy(pin_description->description, state->description, TOOLBOX_IO_PIN_NAME_MAX_LENGTH);
	} else {
		strncpy(pin_description->description, "na", TOOLBOX_IO_PIN_NAME_MAX_LENGTH);
	}



	return 0;
}

int set_io_function(const toolbox_io_attr_t * attributes){

	//check kernel if pins are busy on a different peripheral
	int result = 0;

	if( attributes->peripheral_function == CORE_PERIPH_I2C ){
		switch(attributes->peripheral_port){
			case 0:
				result |= acquire_pin(kernel_shared_direction_channel1, CORE_PERIPH_I2C, 0, IN, "i2c0:scl");
				result |= acquire_pin(kernel_shared_direction_channel2, CORE_PERIPH_I2C, 0, IN, "i2c0:sda");
				if( result == 0 ){
					set_pin_direction(kernel_shared_direction_channel1, IN);
					set_pin_direction(kernel_shared_direction_channel2, IN);
				}
				break;

			case 1:
				result |= acquire_pin(kernel_shared_direction_channel3, CORE_PERIPH_I2C, 0, IN, "i2c1:scl");
				result |= acquire_pin(kernel_shared_direction_channel4, CORE_PERIPH_I2C, 0, IN, "i2c1:sda");
				if( result == 0 ){
					set_pin_direction(kernel_shared_direction_channel3, IN);
					set_pin_direction(kernel_shared_direction_channel4, IN);
				}
				break;
		}

	} else if( attributes->peripheral_function == CORE_PERIPH_SPI ){
		if( attributes->o_flags & TOOLBOX_IO_FLAG_IS_INVERT_DIRECTION ){
			//slave
			result |= acquire_pin(kernel_shared_direction_channel3, CORE_PERIPH_SPI, TOOLBOX_IO_SPI_PORT, IN, "spi:cs");
			result |= acquire_pin(kernel_shared_direction_channel4, CORE_PERIPH_SPI, TOOLBOX_IO_SPI_PORT, IN, "spi:sck");
			result |= acquire_pin(kernel_shared_direction_channel5, CORE_PERIPH_SPI, TOOLBOX_IO_SPI_PORT, OUT, "spi:miso");
			result |= acquire_pin(kernel_shared_direction_channel6, CORE_PERIPH_SPI, TOOLBOX_IO_SPI_PORT, IN, "spi:mosi");
			if( result == 0 ){
				set_pin_direction(kernel_shared_direction_channel3, IN);
				set_pin_direction(kernel_shared_direction_channel4, IN);
				set_pin_direction(kernel_shared_direction_channel5, OUT);
				set_pin_direction(kernel_shared_direction_channel6, IN);
			}
		} else {
			//master
			result |= acquire_pin(kernel_shared_direction_channel3, CORE_PERIPH_SPI, TOOLBOX_IO_SPI_PORT, OUT, "spi:cs");
			result |= acquire_pin(kernel_shared_direction_channel4, CORE_PERIPH_SPI, TOOLBOX_IO_SPI_PORT, OUT, "spi:sck");
			result |= acquire_pin(kernel_shared_direction_channel5, CORE_PERIPH_SPI, TOOLBOX_IO_SPI_PORT, IN, "spi:miso");
			result |= acquire_pin(kernel_shared_direction_channel6, CORE_PERIPH_SPI, TOOLBOX_IO_SPI_PORT, OUT, "spi:mosi");
			if( result == 0 ){
				set_pin_direction(kernel_shared_direction_channel3, OUT);
				set_pin_direction(kernel_shared_direction_channel4, OUT);
				set_pin_direction(kernel_shared_direction_channel5, IN);
				set_pin_direction(kernel_shared_direction_channel6, OUT);
			}
		}
	} else if( attributes->peripheral_function == CORE_PERIPH_I2S ){
		if( attributes->o_flags & TOOLBOX_IO_FLAG_IS_INVERT_DIRECTION ){
			//slave
			result |= acquire_pin(kernel_shared_direction_channel9, CORE_PERIPH_I2S, TOOLBOX_IO_I2S_PORT, IN, "i2s:din");
			result |= acquire_pin(kernel_shared_direction_channel10, CORE_PERIPH_I2S, TOOLBOX_IO_I2S_PORT, IN, "i2s:ws");
			result |= acquire_pin(kernel_shared_direction_channel11, CORE_PERIPH_I2S, TOOLBOX_IO_I2S_PORT, IN, "i2s:bclk");
			result |= acquire_pin(kernel_shared_direction_channel12, CORE_PERIPH_I2S, TOOLBOX_IO_I2S_PORT, OUT, "i2s:dout");
			if( result == 0 ){
				set_pin_direction(kernel_shared_direction_channel9, IN);
				set_pin_direction(kernel_shared_direction_channel10, IN);
				set_pin_direction(kernel_shared_direction_channel11, IN);
				set_pin_direction(kernel_shared_direction_channel12, OUT);
			}
		} else {
			//master
			result |= acquire_pin(kernel_shared_direction_channel9, CORE_PERIPH_I2S, TOOLBOX_IO_I2S_PORT, IN, "i2s:din");
			result |= acquire_pin(kernel_shared_direction_channel10, CORE_PERIPH_I2S, TOOLBOX_IO_I2S_PORT, OUT, "i2s:ws");
			result |= acquire_pin(kernel_shared_direction_channel11, CORE_PERIPH_I2S, TOOLBOX_IO_I2S_PORT, OUT, "i2s:bclk");
			result |= acquire_pin(kernel_shared_direction_channel12, CORE_PERIPH_I2S, TOOLBOX_IO_I2S_PORT, OUT, "i2s:dout");
			if( result == 0 ){
				set_pin_direction(kernel_shared_direction_channel9, IN);
				set_pin_direction(kernel_shared_direction_channel10, OUT);
				set_pin_direction(kernel_shared_direction_channel11, OUT);
				set_pin_direction(kernel_shared_direction_channel12, OUT);
			}
		}
	} else if( attributes->peripheral_function == CORE_PERIPH_UART ){
		switch(attributes->peripheral_port){
			case 0:
				result |= acquire_pin(kernel_shared_direction_channel12, CORE_PERIPH_UART, 0, OUT, "uart0:rx");
				if( result == 0 ){
					set_pin_direction(kernel_shared_direction_channel13, IN); //sole pin
				}
				break;

			case 1:
				result |= acquire_pin(kernel_shared_direction_channel8, CORE_PERIPH_UART, 1, OUT, "uart1:tx");
				result |= acquire_pin(kernel_shared_direction_channel3, CORE_PERIPH_UART, 1, IN, "uart1:rx");
				if( result == 0 ){
					set_pin_direction(kernel_shared_direction_channel8, OUT);
					set_pin_direction(kernel_shared_direction_channel3, IN);
				}
				break;

			case 3:
				result |= acquire_pin(kernel_shared_direction_channel1, CORE_PERIPH_UART, 3, IN, "uart3:rx");
				result |= acquire_pin(kernel_shared_direction_channel2, CORE_PERIPH_UART, 3, OUT, "uart3:tx");
				if( result == 0 ){
					set_pin_direction(kernel_shared_direction_channel1, IN);
					set_pin_direction(kernel_shared_direction_channel2, OUT);
				}
				break;

			case 5:
				result |= acquire_pin(kernel_shared_direction_channel5, CORE_PERIPH_UART, 5, OUT, "uart5:tx");
				result |= acquire_pin(kernel_shared_direction_channel6, CORE_PERIPH_UART, 5, IN, "uart5:rx");
				if( result == 0 ){
					set_pin_direction(kernel_shared_direction_channel5, OUT);
					set_pin_direction(kernel_shared_direction_channel6, IN);
				}
				break;
		}
	} else if( attributes->peripheral_function == CORE_PERIPH_PIO ){

		if( (attributes->peripheral_port == 0) &&
				(attributes->pin_number <= last_kernel_shared_direction_channel) ){

			u8 io_flags;
			if( attributes->o_flags & TOOLBOX_IO_FLAG_IS_OUTPUT ){
				io_flags = OUT;
			} else {
				io_flags = IN;
			}
			result |= acquire_pin(attributes->pin_number, CORE_PERIPH_PIO, 0, io_flags, "gpio");
			if( result == 0 ){
				set_pin_direction(attributes->pin_number, io_flags);
			} else {
				mcu_debug_printf("failed to acquire pin\n");
			}


		}
	}

	return 0;
}

int get_pin_direction_mask(){
	int result = 0;
	for(int i=first_kernel_shared_direction_channel;
			i <= last_kernel_shared_direction_channel;
			i++){
		if( kernel_shared_get_direction_state(i)->io_flags & OUT ){
			result |= (1<<i);
		}
	}
	return result;
}

int get_io_info(toolbox_io_info_t * info){
	int result = 0;
	memset(info, 0xff, sizeof(*info));
	info->o_output_pinmask = 0;
	for(int i=first_kernel_shared_direction_channel;
			i <= last_kernel_shared_direction_channel;
			i++){
		const kernel_shared_direction_state_t * const state =
				kernel_shared_get_direction_state(i);
		if( state->io_flags & OUT ){
			info->o_output_pinmask |= (1<<i);
		}

		info->peripheral_port[i] = state->peripheral_port;
		info->peripheral_function[i] = state->peripheral_function;
	}
	return result;
}

int kernel_io_enable_uart_debug_tx(){
	//enable VDD REF out
	kernel_io_clear(kernel_io_vdd_out_enable);
	return 0;
}

int set_pin_direction(u32 pin_number, int io_flags){
	u8 port = external_pin_table[pin_number].expansion_port;
	u32 o_pinmask = 1<<external_pin_table[pin_number].expansion_pin;
	if( port == PCAL6116A_PORTX ){
		return 0;
	}

	//set the mcu pins to inputs to prevent driving conflicts
	cortexm_svcall(
				svcall_set_pin_input,
				(void*)&external_pin_table[pin_number].primary
				);

	cortexm_svcall(
				svcall_set_pin_input,
				(void*)&external_pin_table[pin_number].secondary
				);

	if( io_flags & OUT ){
		pcal6416a_setmask(
					port,
					o_pinmask
					);
	} else {
		pcal6416a_clrmask(
					port,
					o_pinmask
					);
	}
	return 0;
}

void svcall_set_pin_input(void * args){
	CORTEXM_SVCALL_ENTER();
	mcu_pin_t * pin = args;
	devfs_handle_t handle = {0};
	if( pin->port != 0xff ){
		handle.port = pin->port;
		pio_attr_t attributes;
		attributes.o_pinmask = 1<<pin->pin;
		attributes.o_flags = PIO_FLAG_SET_INPUT | PIO_FLAG_IS_PULLDOWN;
		mcu_pio_setattr(&handle, &attributes);
	}
}

int init_external_pins(){
	for(enum kernel_shared_direction_channels i = first_kernel_shared_direction_channel;
			i < last_kernel_shared_direction_channel+1;
			i++){
		int result;
		const kernel_io_external_pin_t * io_pin = external_pin_table + i;
		if( io_pin->expansion_port != PCAL6116A_PORTX ){
			pio_attr_t attributes;

			//set the direction control pin
			if( io_pin->init_flags & OUT ){
				if( pcal6416a_setmask(
							io_pin->expansion_port,
							1<<io_pin->expansion_pin
							) < 0 ){
					mcu_debug_log_error(MCU_DEBUG_USER0, "failed to set PCA mask");
				}
			} else {
				if( pcal6416a_clrmask(
							io_pin->expansion_port,
							1<<io_pin->expansion_pin
							) < 0 ){
					mcu_debug_log_error(MCU_DEBUG_USER0, "failed to clear PCA mask");
				}
			}

			//all direction control pins are outputs
			attributes.o_flags = OUT;
			attributes.o_pinmask = 1 << (io_pin->expansion_pin);

#if 1
			mcu_debug_printf(
						"set attributes for 0x%X:%d:0x%X\n",
						io_pin->expansion_port,
						io_pin->expansion_pin,
						io_pin->init_flags
						);
#endif

			result = pcal6416a_setattr(
						io_pin->expansion_port,
						&attributes
						);

#if ___debug
			if( i == kernel_shared_direction_channel2 ){
				acquire_pin(kernel_shared_direction_channel2, CORE_PERIPH_UART, 3, OUT, "uart3:tx");
			} else
#endif
			acquire_pin(
						i,
						CORE_PERIPH_PIO,
						0,
						io_pin->init_flags,
						"gpio"
						);

			if( result < 0 ){
				mcu_debug_log_error(
							MCU_DEBUG_USER0,
							"failed to set pin attributes",
							result
							);
				return -1;
			}
		}
	}
	return 0;
}

int init_internal_pins(){
	for(enum kernel_io_pins i=kernel_io_first; i < kernel_io_last+1; i++){
		int result;
		const kernel_io_pin_t * io_pin
				= internal_pin_table + i;
		pio_attr_t attributes;

		if( io_pin->init_state ){
			kernel_io_set(i);
		} else {
			kernel_io_clear(i);
		}

		attributes.o_flags = io_pin->init_flags;
		attributes.o_pinmask = 1<<io_pin->expansion_pin;

		result = pcal6416a_setattr(
					io_pin->expansion_port,
					&attributes
					);
		if( result < 0 ){
			mcu_debug_log_error(
						MCU_DEBUG_USER0,
						"failed to set pin attributes",
						result
						);
			return -1;
		}
	}
	return 0;
}


