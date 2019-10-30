#include <cortexm/cortexm.h>
#include <mcu/debug.h>
#include <mcu/arch.h>
#include <mcu/arch/stm32/stm32h7xx/stm32h7xx_hal.h>
#include <mcu/arch/stm32/stm32h7xx/stm32h7xx_hal_sram.h>
#include <mcu/arch/stm32/stm32h7xx/stm32h7xx_hal_gpio.h>
#include <mcu/arch/stm32/stm32h7xx/stm32h7xx_hal_rcc.h>

#include "stm32_bsp.h"


static SRAM_HandleTypeDef hsram1 MCU_SYS_MEM;
static int FMC_Initialized = 0;
static void system_initialize();


void SystemClock_Config(){

	system_initialize();

	__HAL_RCC_SYSCFG_CLK_ENABLE();


	RCC_OscInitTypeDef RCC_OscInitStruct = {0};
	RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
	RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};

	/** Supply configuration update enable
	  */
	HAL_PWREx_ConfigSupply(PWR_LDO_SUPPLY);
	/** Configure the main internal regulator output voltage
	  */
	__HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

	while(!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY)) {}
	/** Initializes the CPU, AHB and APB busses clocks
	  */
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI48|RCC_OSCILLATORTYPE_HSI;
	RCC_OscInitStruct.HSIState = RCC_HSI_DIV1;
	RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
	RCC_OscInitStruct.HSI48State = RCC_HSI48_ON;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
	RCC_OscInitStruct.PLL.PLLM = 4;
	RCC_OscInitStruct.PLL.PLLN = 50;
	RCC_OscInitStruct.PLL.PLLP = 2;
	RCC_OscInitStruct.PLL.PLLQ = 4;
	RCC_OscInitStruct.PLL.PLLR = 2;
	RCC_OscInitStruct.PLL.PLLRGE = RCC_PLL1VCIRANGE_3;
	RCC_OscInitStruct.PLL.PLLVCOSEL = RCC_PLL1VCOWIDE;
	RCC_OscInitStruct.PLL.PLLFRACN = 0;
	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
	{
		while(1){

		}
	}
	/** Initializes the CPU, AHB and APB busses clocks
	  */
	RCC_ClkInitStruct.ClockType =
			RCC_CLOCKTYPE_HCLK |
			RCC_CLOCKTYPE_SYSCLK |
			RCC_CLOCKTYPE_PCLK1 |
			RCC_CLOCKTYPE_PCLK2 |
			RCC_CLOCKTYPE_D3PCLK1 |
			RCC_CLOCKTYPE_D1PCLK1;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.SYSCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_HCLK_DIV2;
	RCC_ClkInitStruct.APB3CLKDivider = RCC_APB3_DIV2;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_APB1_DIV2;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_APB2_DIV2;
	RCC_ClkInitStruct.APB4CLKDivider = RCC_APB4_DIV2;

	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
	{
		while(1){

		}
	}

	PeriphClkInitStruct.PeriphClockSelection =
			RCC_PERIPHCLK_USART3 |
			RCC_PERIPHCLK_USB |
			RCC_PERIPHCLK_FMC;

	PeriphClkInitStruct.Usart234578ClockSelection = RCC_USART234578CLKSOURCE_D2PCLK1;
	PeriphClkInitStruct.FmcClockSelection = RCC_FMCCLKSOURCE_D1HCLK;
	PeriphClkInitStruct.UsbClockSelection = RCC_USBCLKSOURCE_HSI48;
	if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
	{
		while(1){

		}
	}
	/** Enable USB Voltage detector
	  */
	//HAL_PWREx_EnableUSBVoltageDetector();

}


/* FMC initialization function */
void MX_FMC_Init(){

	/* USER CODE BEGIN FMC_Init 0 */

	/* USER CODE END FMC_Init 0 */

	FMC_NORSRAM_TimingTypeDef Timing = {0};

	/* USER CODE BEGIN FMC_Init 1 */

	/* USER CODE END FMC_Init 1 */

	/** Perform the SRAM1 memory initialization sequence
  */
	hsram1.Instance = FMC_NORSRAM_DEVICE;
	hsram1.Extended = FMC_NORSRAM_EXTENDED_DEVICE;
	/* hsram1.Init */
	hsram1.Init.NSBank = FMC_NORSRAM_BANK1;
	hsram1.Init.DataAddressMux = FMC_DATA_ADDRESS_MUX_DISABLE;
	hsram1.Init.MemoryType = FMC_MEMORY_TYPE_SRAM;
	hsram1.Init.MemoryDataWidth = FMC_NORSRAM_MEM_BUS_WIDTH_32;
	hsram1.Init.BurstAccessMode = FMC_BURST_ACCESS_MODE_DISABLE;
	hsram1.Init.WaitSignalPolarity = FMC_WAIT_SIGNAL_POLARITY_LOW;
	hsram1.Init.WaitSignalActive = FMC_WAIT_TIMING_BEFORE_WS;
	hsram1.Init.WriteOperation = FMC_WRITE_OPERATION_ENABLE;
	hsram1.Init.WaitSignal = FMC_WAIT_SIGNAL_DISABLE;
	hsram1.Init.ExtendedMode = FMC_EXTENDED_MODE_DISABLE;
	hsram1.Init.AsynchronousWait = FMC_ASYNCHRONOUS_WAIT_DISABLE;
	hsram1.Init.WriteBurst = FMC_WRITE_BURST_DISABLE;
	hsram1.Init.ContinuousClock = FMC_CONTINUOUS_CLOCK_SYNC_ONLY;
	hsram1.Init.WriteFifo = FMC_WRITE_FIFO_DISABLE;
	hsram1.Init.PageSize = FMC_PAGE_SIZE_NONE;
	/* Timing */
	Timing.AddressSetupTime = 0;
	Timing.AddressHoldTime = 0;
	Timing.DataSetupTime = 2;
	Timing.BusTurnAroundDuration = 10;
	Timing.CLKDivision = 0;
	Timing.DataLatency = 1;
	Timing.AccessMode = FMC_ACCESS_MODE_A;
	/* ExtTiming */

	if (HAL_SRAM_Init(&hsram1, &Timing, NULL) != HAL_OK)
	{
		mcu_debug_printf("Failed to init SRAM\n");
	}

	/* USER CODE BEGIN FMC_Init 2 */

	/* USER CODE END FMC_Init 2 */
}

static void HAL_FMC_MspInit(void){
	/* USER CODE BEGIN FMC_MspInit 0 */

	/* USER CODE END FMC_MspInit 0 */
	GPIO_InitTypeDef GPIO_InitStruct ={0};
	if (FMC_Initialized) {
		return;
	}
	FMC_Initialized = 1;
	/* Peripheral clock enable */
	__HAL_RCC_FMC_CLK_ENABLE();

	/** FMC GPIO Configuration
  PE7   ------> FMC_D4
  PE8   ------> FMC_D5
  PE9   ------> FMC_D6
  PE10   ------> FMC_D7
  PD11   ------> FMC_A16 --Using PC3 as a GPIO instead
  PD14   ------> FMC_D0
  PD15   ------> FMC_D1
  PD0   ------> FMC_D2
  PD1   ------> FMC_D3
  PD4   ------> FMC_NOE
  PD5   ------> FMC_NWE
  PD7   ------> FMC_NE1
  */
	GPIO_InitStruct.Pin =
			GPIO_PIN_7|
			GPIO_PIN_8|
			GPIO_PIN_9|
			GPIO_PIN_10;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStruct.Alternate = GPIO_AF12_FMC;
	HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

	GPIO_InitStruct.Pin =
			GPIO_PIN_14|
			GPIO_PIN_15|
			GPIO_PIN_0|
			GPIO_PIN_1|
			GPIO_PIN_4|
			GPIO_PIN_5|
		#if MANUAL_FMC_CHIP_SELECT == 0
			GPIO_PIN_7|
		#endif
			0;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStruct.Alternate = GPIO_AF12_FMC;
	HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

	//use this as data/command switch
	GPIO_InitStruct.Pin = GPIO_PIN_0;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

	/* USER CODE BEGIN FMC_MspInit 1 */
	GPIO_InitStruct.Pin = GPIO_PIN_1;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);
	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_1, SET);

	//backlight
	GPIO_InitStruct.Pin = GPIO_PIN_13;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, SET);

#if MANUAL_FMC_CHIP_SELECT > 0
	GPIO_InitStruct.Pin = GPIO_PIN_7;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_7, RESET);
#endif

	/* USER CODE END FMC_MspInit 1 */
}

#define DATA_IN_D2_SRAM 1
#if !defined __FPU_USED
#define __FPU_USED 1
#endif

void system_initialize(){
#if defined (DATA_IN_D2_SRAM)
	__IO uint32_t tmpreg;
#endif /* DATA_IN_D2_SRAM */

	/* FPU settings ------------------------------------------------------------*/
#if (__FPU_PRESENT == 1) && (__FPU_USED == 1)
	SCB->CPACR |= ((3UL << (10*2))|(3UL << (11*2)));  /* set CP10 and CP11 Full Access */
#endif
	/* Reset the RCC clock configuration to the default reset state ------------*/
	/* Set HSION bit */
	RCC->CR |= RCC_CR_HSION;

	/* Reset CFGR register */
	RCC->CFGR = 0x00000000;

	/* Reset HSEON, CSSON , CSION,RC48ON, CSIKERON PLL1ON, PLL2ON and PLL3ON bits */
	RCC->CR &= 0xEAF6ED7FU;

	/* Reset D1CFGR register */
	RCC->D1CFGR = 0x00000000;

	/* Reset D2CFGR register */
	RCC->D2CFGR = 0x00000000;

	/* Reset D3CFGR register */
	RCC->D3CFGR = 0x00000000;

	/* Reset PLLCKSELR register */
	RCC->PLLCKSELR = 0x00000000;

	/* Reset PLLCFGR register */
	RCC->PLLCFGR = 0x00000000;
	/* Reset PLL1DIVR register */
	RCC->PLL1DIVR = 0x00000000;
	/* Reset PLL1FRACR register */
	RCC->PLL1FRACR = 0x00000000;

	/* Reset PLL2DIVR register */
	RCC->PLL2DIVR = 0x00000000;

	/* Reset PLL2FRACR register */

	RCC->PLL2FRACR = 0x00000000;
	/* Reset PLL3DIVR register */
	RCC->PLL3DIVR = 0x00000000;

	/* Reset PLL3FRACR register */
	RCC->PLL3FRACR = 0x00000000;

	/* Reset HSEBYP bit */
	RCC->CR &= 0xFFFBFFFFU;

	/* Disable all interrupts */
	RCC->CIER = 0x00000000;

#if defined (DATA_IN_D2_SRAM)
	/* in case of initialized data in D2 SRAM (AHB SRAM) , enable the D2 SRAM clock ((AHB SRAM clock) */
#if defined(RCC_AHB2ENR_D2SRAM1EN)
	RCC->AHB2ENR |= (RCC_AHB2ENR_D2SRAM1EN | RCC_AHB2ENR_D2SRAM2EN | RCC_AHB2ENR_D2SRAM3EN);
#else
	RCC->AHB2ENR |= (RCC_AHB2ENR_AHBSRAM1EN | RCC_AHB2ENR_AHBSRAM2EN);
#endif /* RCC_AHB2ENR_D2SRAM1EN */

	tmpreg = RCC->AHB2ENR;
	(void) tmpreg;
#endif /* DATA_IN_D2_SRAM */

	/* dual core CM7 or single core line */
	if((DBGMCU->IDCODE & 0xFFFF0000U) < 0x20000000U)
	{
		/* if stm32h7 revY*/
		/* Change  the switch matrix read issuing capability to 1 for the AXI SRAM target (Target 7) */
		*((__IO uint32_t*)0x51008108) = 0x000000001U;
	}

}

void HAL_SRAM_MspInit(SRAM_HandleTypeDef* hsram){
	/* USER CODE BEGIN SRAM_MspInit 0 */

	/* USER CODE END SRAM_MspInit 0 */
	HAL_FMC_MspInit();
	/* USER CODE BEGIN SRAM_MspInit 1 */

	/* USER CODE END SRAM_MspInit 1 */
}

static uint32_t FMC_DeInitialized = 0;

static void HAL_FMC_MspDeInit(void){
	/* USER CODE BEGIN FMC_MspDeInit 0 */

	/* USER CODE END FMC_MspDeInit 0 */
	if (FMC_DeInitialized) {
		return;
	}
	FMC_DeInitialized = 1;
	/* Peripheral clock enable */
	__HAL_RCC_FMC_CLK_DISABLE();

	/** FMC GPIO Configuration
  PE7   ------> FMC_D4
  PE8   ------> FMC_D5
  PE9   ------> FMC_D6
  PE10   ------> FMC_D7
  PD11   ------> FMC_A16  -- using PC3 as a GPIO instead for command/data switch
  PD14   ------> FMC_D0
  PD15   ------> FMC_D1
  PD0   ------> FMC_D2
  PD1   ------> FMC_D3
  PD4   ------> FMC_NOE
  PD5   ------> FMC_NWE
  PD7   ------> FMC_NE1
  */
	HAL_GPIO_DeInit(
				GPIOE,
				GPIO_PIN_7|
				GPIO_PIN_8|
				GPIO_PIN_9|
				GPIO_PIN_10
				);

	HAL_GPIO_DeInit(
				GPIOD,
				GPIO_PIN_14|
				GPIO_PIN_15|
				GPIO_PIN_0 |
				GPIO_PIN_1|
				GPIO_PIN_4|
				GPIO_PIN_5|
				GPIO_PIN_7
				);

	/* USER CODE BEGIN FMC_MspDeInit 1 */

	/* USER CODE END FMC_MspDeInit 1 */
}

void HAL_SRAM_MspDeInit(SRAM_HandleTypeDef* hsram){
	/* USER CODE BEGIN SRAM_MspDeInit 0 */

	/* USER CODE END SRAM_MspDeInit 0 */
	HAL_FMC_MspDeInit();
	/* USER CODE BEGIN SRAM_MspDeInit 1 */

	/* USER CODE END SRAM_MspDeInit 1 */
}
