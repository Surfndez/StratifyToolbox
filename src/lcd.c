
#include <mcu/types.h>
#include <cortexm/cortexm.h>
#include <cortexm/mpu.h>
#include <cortexm/task.h>
#include <mcu/core.h>
#include <mcu/debug.h>
#include <mcu/arch/stm32/stm32h7xx/stm32h7xx_hal_sram.h>
#include <mcu/arch/stm32/stm32h7xx/stm32h7xx_hal_gpio.h>

#include "lcd.h"
#include "stm32_bsp.h"

#include "stm32_bsp.h"

#define IO_ADDRESS (0x60000000)
#define IO_ADDRESS_DEFERENCE *((__IO u8*)0x60000000)

#define SET_COMMAND() (GPIOC->BSRRH = GPIO_PIN_3) //RESET
#define SET_DATA() (GPIOC->BSRRL = GPIO_PIN_3) //SET

#define ASSERT_RESET() (GPIOE->BSRRH = GPIO_PIN_1) //RESET
#define DEASSERT_RESET() (GPIOE->BSRRL = GPIO_PIN_1) //SET

void LCD_IO_Init(){
	MX_FMC_Init();
	int result;
	//FMC Bank 2 needs to be non-cacheable because it is used to write the display driver chip
	result = mpu_enable_region(
				TASK_APPLICATION_DATA_USER_REGION,
				(void*)(IO_ADDRESS), //bank 1 sub bank 2
				0x04000000,
				MPU_ACCESS_PRW,
				MPU_MEMORY_PERIPHERALS,
				0
				);
	if( result < 0 ){
		mcu_debug_printf("failed to configure MPU for LCD memory\n");
	}
	//sychronize data since mpu has been updated
	mcu_core_disable_cache();
	mcu_core_enable_cache();

	SET_DATA();
	ASSERT_RESET();
	cortexm_delay_ms(10);
	DEASSERT_RESET();
}

void LCD_IO_WriteReg(u8 Reg){
	SET_COMMAND();
	cortexm_delay_us(1);
	IO_ADDRESS_DEFERENCE = Reg;
	__DSB();
}

void LCD_IO_WriteData(u16 RegValue){
	SET_DATA();
	cortexm_delay_us(1);
	IO_ADDRESS_DEFERENCE = RegValue >> 8;
	IO_ADDRESS_DEFERENCE = RegValue;
}

u16 LCD_IO_ReadData(){
	SET_DATA();
	cortexm_delay_us(1);
	u16 result;
	result = IO_ADDRESS_DEFERENCE << 8;
	result |= IO_ADDRESS_DEFERENCE;
	return result;
}

void LCD_IO_Delay(u32 delay){
	cortexm_delay_ms(delay);
}
