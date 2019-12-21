
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef STM32_BSP_H
#define STM32_BSP_H

#include <mcu/arch/stm32/stm32h7xx/stm32h7xx_hal_conf.h>
#include <mcu/arch/stm32/stm32h7xx/stm32h7xx_hal_sram.h>
#include <mcu/arch/stm32/stm32h7xx/stm32h7xx_hal_gpio.h>

#define MANUAL_FMC_CHIP_SELECT 0

void MX_FMC_Init();
void SystemClock_Config();


#endif /* STM32_BSP_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
