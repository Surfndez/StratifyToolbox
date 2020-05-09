
#include <mcu/types.h>
#include <cortexm/cortexm.h>
#include <cortexm/mpu.h>
#include <cortexm/task.h>
#include <mcu/core.h>
#include <mcu/debug.h>

#include "lcd.h"
#include "../mcu/stm32_bsp.h"

#include "../mcu/stm32_bsp.h"

#define IO_ADDRESS (0x60000000)
#define IO_ADDRESS_DEFERENCE *((__IO u8*)0x60000000)

volatile u64 * const m_reg_location = (u64*)0x60000000;

//PE1
#define SET_COMMAND() (GPIOE->BSRR = (GPIO_PIN_1 << 16)) //RESET
#define SET_DATA() (GPIOE->BSRR = GPIO_PIN_1) //SET

//PB12
#define ASSERT_RESET() (GPIOB->BSRR = (GPIO_PIN_12 << 16)) //RESET
#define DEASSERT_RESET() (GPIOB->BSRR = GPIO_PIN_12) //SET

//PD7
#define ASSERT_CS() (GPIOD->BSRR = (GPIO_PIN_7<<16)) //RESET
#define DEASSERT_CS() (GPIOD->BSRR = GPIO_PIN_7) //SET

#define DELAY() cortexm_delay_us(1)

void LCD_IO_Assert_CS(){
#if MANUAL_FMC_CHIP_SELECT > 0
	ASSERT_CS();
	cortexm_delay_us(5);
#endif
}

void LCD_IO_Deassert_CS(){
#if MANUAL_FMC_CHIP_SELECT > 0
	DEASSERT_CS();
	cortexm_delay_us(5);
#endif
}

typedef struct MCU_PACK {
	u8 first;
	u8 dummy[3];
	u8 second;
	u8 dummy0[3];
} transaction_data_t;

typedef union {
	u64 raw;
	transaction_data_t data;
} transaction_t;


void LCD_IO_Init(){
	MX_FMC_Init();
	int result;
	//FMC Bank 2 needs to be non-cacheable because it is used to write the display driver chip
	result = mpu_enable_region(
				TASK_APPLICATION_DATA_USER_REGION,
				(void*)(IO_ADDRESS), //bank 1 sub bank 2
				0x04000000,
				MPU_ACCESS_PRW,
				MPU_MEMORY_LCD,
				0
				);
	if( result < 0 ){
		mcu_debug_printf("failed to configure MPU for LCD memory\n");
	}
	//sychronize data since mpu has been updated
	//mcu_core_disable_cache();
	//mcu_core_enable_cache();

#if 0
	while(1){
		LCD_IO_Deassert_CS();
		SET_DATA();
		ASSERT_RESET();
		cortexm_delay_ms(10);
		DEASSERT_RESET();
		SET_COMMAND();
		LCD_IO_Assert_CS();
		cortexm_delay_ms(10);
	}
#else
	LCD_IO_Deassert_CS();
	SET_DATA();
	ASSERT_RESET();
	cortexm_delay_ms(10);
	DEASSERT_RESET();
	cortexm_delay_ms(120);
#endif
}

void LCD_IO_WriteReg(u8 Reg){
	SET_COMMAND();
	cortexm_delay_us(2);
	transaction_t transaction;
	transaction.data.first = 0; //NOP
	transaction.data.second = Reg; //register value
	*m_reg_location = transaction.raw;
	__DSB();
	DELAY();
	SET_DATA();
	DELAY();
}

void LCD_IO_WriteDataBlock(const u8 * data, int nbyte){
	transaction_t transaction;
	for(u32 i=0; i < nbyte; i+=2 ){
		transaction.data.first = data[i];
		transaction.data.second = data[i+1];
		*m_reg_location = transaction.raw;
		__DSB();
	}
}

void LCD_IO_WriteDataBlockRgb(const u8 * data, int nbyte){
	transaction_t transaction;
	for(u32 i=0; i < nbyte; i+=2 ){
		transaction.data.first = data[i+1];
		transaction.data.second = data[i];
		*m_reg_location = transaction.raw;
		__DSB();
	}
}

void LCD_IO_WriteData64(u64 value){
	*m_reg_location = value;
	__DSB();
}

void LCD_IO_WriteData8(u8 RegValue){
	SET_DATA();
	cortexm_delay_us(2);
	m_reg_location[0] = RegValue;
	__DSB();
}


void LCD_IO_WriteData(u16 RegValue){
	SET_DATA();
	cortexm_delay_us(2);
	m_reg_location[0] = RegValue >> 8;
	__DSB();
	m_reg_location[0] = RegValue;
	__DSB();
}

u16 LCD_IO_ReadData(){
	SET_DATA();
	cortexm_delay_us(2);
	u16 result;
	result = m_reg_location[0] << 8;
	__DSB();
	result |= m_reg_location[0];
	__DSB();
	return result;
}

void LCD_IO_Delay(u32 delay){
	cortexm_delay_ms(delay);
}
