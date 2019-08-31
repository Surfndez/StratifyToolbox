
#include <mcu/types.h>
#include <cortexm/cortexm.h>
#include <cortexm/mpu.h>
#include <cortexm/task.h>
#include <mcu/core.h>
#include <mcu/debug.h>
#include <mcu/arch/stm32/stm32h7xx/stm32h7xx_hal_sram.h>

#include "lcd.h"
#include "stm32_bsp.h"

#include "stm32_bsp.h"

#define IO_ADDRESS (0x60000000)
#define IO_ADDRESS_DEFERENCE *((u8*)0x60000000)

void LCD_IO_Init(){
	MX_FMC_Init();
	int result;
	//FMC Bank 2 needs to be non-cacheable because it is used to write the display driver chip
	result = mpu_enable_region(TASK_APPLICATION_DATA_USER_REGION,
							(void*)(IO_ADDRESS), //bank 1 sub bank 2
							0x04000000,
							MPU_ACCESS_PRW,
							MPU_MEMORY_PERIPHERALS,
							0);
	if( result < 0 ){
		mcu_debug_printf("failed to configure MPU for LCD memory\n");
	}
	//sychronize data since mpu has been updated
	mcu_core_disable_cache();
	mcu_core_enable_cache();


}

void LCD_IO_WriteMultipleData(u8 *pData, u32 Size){

	for(u32 i=0; i < Size; i++){
		IO_ADDRESS_DEFERENCE = pData[i];
		__DSB();
	}
}

void LCD_IO_WriteReg(u8 Reg){

	IO_ADDRESS_DEFERENCE = Reg;
	__DSB();

}

void LCD_IO_WriteData(u8 RegValue){
	IO_ADDRESS_DEFERENCE = RegValue;
	__DSB();
}

u16 LCD_IO_ReadData(){
	return 0;
}

void LCD_IO_Delay(u32 delay){
	cortexm_delay_ms(delay);
}
