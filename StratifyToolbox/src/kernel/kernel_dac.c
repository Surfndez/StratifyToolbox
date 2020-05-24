#include <ToolboxAPI/toolbox_dac.h>

#include <fcntl.h>
#include <math.h>
#include <string.h>
#include <unistd.h>
#include <mcu/debug.h>
#include <sos/dev/tmr.h>
#include <sos/dev/dac.h>
#include <sos/dev/stream_ffifo.h>

#include "kernel_dac.h"
#include "kernel_shared.h"
#include "kernel_service.h"

static int init_dac(kernel_shared_dac_channel_t * dac, const char* device_name);

static int start_dac_stream(kernel_shared_dac_channel_t * dac);
static int stop_dac_stream(kernel_shared_dac_channel_t * dac);
static void write_dac_stream(kernel_shared_dac_channel_t * dac);

//static void generate_triangle_wave(kernel_shared_dac_channel_t* dac);
static void generate_flat_wave(kernel_shared_dac_channel_t* dac);
static void generate_square_wave(kernel_shared_dac_channel_t* dac);
static void generate_sine_wave(kernel_shared_dac_channel_t * dac);

int kernel_dac_request(void * args){
	toolbox_dac_attr_t * attr = args;
	if( attr->channel < 2 ){
		kernel_shared_dac_channel_t *	dac = kernel_shared_dac()->channel + attr->channel;

		//validate the structure
		if( attr->amplitude < 0.0f ){ attr->amplitude = 0.0f; }
		if( attr->amplitude > 1.0f ){ attr->amplitude = 1.0f; }
		if( attr->frequency < 0.0f ){ attr->frequency = 0.0f; }
		if( attr->offset + attr->amplitude > 1.0f ){
			attr->offset = 1.0f - attr->amplitude;
		} else if( attr->offset - attr->amplitude < 1.0f ){
			attr->offset = -1.0 + attr->amplitude;
		}

		//all is good -- time to copy
		pthread_mutex_lock(&dac->mutex);
		memcpy(&dac->attr, attr, sizeof(toolbox_dac_attr_t));
		pthread_mutex_unlock(&dac->mutex);
	} else {
		errno = EINVAL;
		return -1;
	}
	return 0;
}

void * kernel_dac_thread_function(void * args){
	kernel_shared_dac_channel_t *	dac = args;

	while(1){
		//if a signal is active, write the outputs

		if( (dac->attr.o_flags & TOOLBOX_DAC_FLAG_START) &&
				((dac->o_flags & TOOLBOX_DAC_FLAG_IS_ACTIVE) == 0) ){
			//write two packets so the FIFO is full
			dac->t = 0.0f;
			write_dac_stream(dac);
			write_dac_stream(dac);
			//now that the FIFO is filled, start running
			start_dac_stream(dac);
			dac->attr.o_flags &= ~TOOLBOX_DAC_FLAG_START;
			dac->o_flags |= TOOLBOX_DAC_FLAG_IS_ACTIVE;
		}

		if( (dac->attr.o_flags & TOOLBOX_DAC_FLAG_STOP) &&
				((dac->o_flags & TOOLBOX_DAC_FLAG_IS_ACTIVE) != 0) ){
			//STOP!!!
			dac->o_flags &= ~(TOOLBOX_DAC_FLAG_IS_ACTIVE);
			stop_dac_stream(dac);

			dac->attr.o_flags &= ~(TOOLBOX_DAC_FLAG_STOP);
		}

		if( dac->o_flags & TOOLBOX_DAC_FLAG_IS_ACTIVE ){
			write_dac_stream(dac);
		} else {
			usleep(100*1000);
		}

	}
}

int init_dac(kernel_shared_dac_channel_t * dac, const char * device_name){
	memset(dac, 0, sizeof(kernel_shared_dac_channel_t));
	dac->file.fs = &(sysfs_list[SOS_BOARD_DEVFS_OFFSET]); //device filesystem
	dac->file.flags = O_RDWR;
	dac->file.loc = 0;
	dac->file.handle = NULL;

	if( sysfs_file_open(
				&dac->file,
				device_name,
				0
				) < 0 ){
		//ASSERT
		return -1;
	}

	if( sysfs_file_ioctl(
				&dac->file,
				I_DAC_SETATTR,
				NULL
				) < 0 ){
		//ASSERT
		return -1;
	}


	dac_info_t dac_info;
	if( sysfs_file_ioctl(
				&dac->file,
				I_DAC_GETINFO,
				&dac_info
				) < 0 ){
		//ASSERT
		return -1;
	}

	dac->scale = dac_info.maximum;
	return 0;
}

int start_dac_stream(kernel_shared_dac_channel_t * dac){
	stream_ffifo_attr_t attributes = {0};
	attributes.o_flags = STREAM_FFIFO_FLAG_START;
	return sysfs_file_ioctl(
				&dac->file,
				I_STREAM_FFIFO_SETATTR,
				&attributes
				);
}

int stop_dac_stream(kernel_shared_dac_channel_t * dac){
	stream_ffifo_attr_t attributes = {0};
	attributes.o_flags = STREAM_FFIFO_FLAG_STOP;
	return sysfs_file_ioctl(
				&dac->file,
				I_STREAM_FFIFO_SETATTR,
				&attributes
				);
}

void write_dac_stream(kernel_shared_dac_channel_t * dac){
	//write to the output -- this blocks
	if( dac->o_flags & TOOLBOX_DAC_FLAG_IS_SQUARE ){
		generate_square_wave(dac);
	} else if( dac->o_flags & TOOLBOX_DAC_FLAG_IS_TRIANGLE ){
		generate_sine_wave(dac); //needs to be triangle
	} else if( dac->o_flags & TOOLBOX_DAC_FLAG_IS_SINE ){
		generate_sine_wave(dac);
	} else {
		generate_flat_wave(dac);
	}
	sysfs_file_write(
				&dac->file,
				dac->data,
				TOOLBOX_DAC_PACKET_SIZE
				);
}

int kernel_dac_init(){
	kernel_shared_dac_t * dac = kernel_shared_dac();
	static const char * const devices[] = {
		"dac0", "dac1"
	};

	dac->tmr_file.fs = &(sysfs_list[SOS_BOARD_DEVFS_OFFSET]); //device filesystem
	dac->tmr_file.flags = O_RDWR;
	dac->tmr_file.loc = 0;
	dac->tmr_file.handle = NULL;

	if( sysfs_file_open(
				&dac->tmr_file,
				"tmr14",
				0
				) < 0 ){
		//ASSERT
		mcu_debug_log_fatal(
					MCU_DEBUG_USER0,
					"failed to open tmr14"
					);
		return -1;
	}


	//TMR14 triggers DAC conversions -- it needs to be running
	if( sysfs_file_ioctl(
				&dac->tmr_file,
				I_TMR_SETATTR,
				NULL) < 0 ){
		mcu_debug_log_fatal(
					MCU_DEBUG_USER0,
					"failed to set tmr14 attributes"
					);
	}

	if( sysfs_file_ioctl(
				&dac->tmr_file,
				I_TMR_ENABLE,
				NULL) < 0 ){
		mcu_debug_log_fatal(
					MCU_DEBUG_USER0,
					"failed to enable tmr14"
					);
	}

	for(u32 i=0; i < 2; i++){
		kernel_shared_dac_channel_t * dac = kernel_shared_dac()->channel + i;
		if( init_dac(
					dac,
					devices[i]
					) < 0 ){
			return -1;
		}

		kernel_service_start_thread(
					kernel_dac_thread_function,
					dac,
					2048,
					SCHED_FIFO,
					8
					);
	}


	return 0;
}

void generate_square_wave(kernel_shared_dac_channel_t* dac){
	pthread_mutex_lock(&dac->mutex);
	const float frequency = dac->attr.frequency;
	const float amplitude = dac->attr.amplitude * 0.5f;
	const float offset = dac->attr.offset * 0.5f + 0.5f;
	float t = dac->t;
	const u32 scale = dac->scale;
	u32 * output = dac->data;

	float half_period;
	if( frequency > 0.0f ){
		half_period = 1.0f / (2.0f * frequency);
	} else {
		half_period = HUGE_VALF;
	}

	float start = t;
	float level = 1.0f;
	for(u32 i=0; i < TOOLBOX_DAC_SAMPLES_PER_PACKET; i++){
		//use CMSIS to do a fast SIN calculation
		if( t - start >= half_period ){
			level *= -1.0f;
		}
		*output++ =
				(amplitude * level + offset) * scale;
		t += 0.0000001f;
	}
	dac->t = t;

	pthread_mutex_unlock(&dac->mutex);


}

void generate_sine_wave(kernel_shared_dac_channel_t* dac){

	pthread_mutex_lock(&dac->mutex);
	const float frequency = dac->attr.frequency;
	const float amplitude = dac->attr.amplitude * 0.5f;
	const float offset = dac->attr.offset * 0.5f + 0.5f;
	float t = dac->t;
	const u32 scale = dac->scale;

	u32 * output = dac->data;
	for(u32 i=0; i < TOOLBOX_DAC_SAMPLES_PER_PACKET; i++){
		//use CMSIS to do a fast SIN calculation
		*output++ =
				(amplitude * sinf(2 * MCU_PI_FLOAT * frequency * t) + offset) * scale;
		t += 0.0000001f;
	}
	dac->t = t;

	pthread_mutex_unlock(&dac->mutex);
}

void generate_flat_wave(kernel_shared_dac_channel_t* dac){
	pthread_mutex_lock(&dac->mutex);
	const float offset = dac->attr.offset * 0.5f + 0.5f;
	float t = dac->t;
	const u32 scale = dac->scale;

	u32 * output = dac->data;
	for(u32 i=0; i < TOOLBOX_DAC_SAMPLES_PER_PACKET; i++){
		//use CMSIS to do a fast SIN calculation
		*output++ = (offset) * scale;
		t += 0.0000001f;
	}
	dac->t = t;

	pthread_mutex_unlock(&dac->mutex);
}
