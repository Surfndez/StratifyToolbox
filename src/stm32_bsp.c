#include <cortexm/cortexm.h>
#include <mcu/debug.h>
#include <mcu/arch.h>
#include <mcu/arch/stm32/stm32h7xx/stm32h7xx_hal_sram.h>
#include <mcu/arch/stm32/stm32h7xx/stm32h7xx_hal_gpio.h>
#include <mcu/arch/stm32/stm32h7xx/stm32h7xx_hal_rcc.h>

static SRAM_HandleTypeDef hsram1 MCU_SYS_MEM;
static int FMC_Initialized = 0;

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
	hsram1.Init.MemoryDataWidth = FMC_NORSRAM_MEM_BUS_WIDTH_8;
	hsram1.Init.BurstAccessMode = FMC_BURST_ACCESS_MODE_DISABLE;
	hsram1.Init.WaitSignalPolarity = FMC_WAIT_SIGNAL_POLARITY_LOW;
	hsram1.Init.WaitSignalActive = FMC_WAIT_TIMING_BEFORE_WS;
	hsram1.Init.WriteOperation = FMC_WRITE_OPERATION_ENABLE;
	hsram1.Init.WaitSignal = FMC_WAIT_SIGNAL_DISABLE;
	hsram1.Init.ExtendedMode = FMC_EXTENDED_MODE_DISABLE;
	hsram1.Init.AsynchronousWait = FMC_ASYNCHRONOUS_WAIT_DISABLE;
	hsram1.Init.WriteBurst = FMC_WRITE_BURST_DISABLE;
	hsram1.Init.ContinuousClock = FMC_CONTINUOUS_CLOCK_SYNC_ONLY;
	hsram1.Init.WriteFifo = FMC_WRITE_FIFO_ENABLE;
	hsram1.Init.PageSize = FMC_PAGE_SIZE_NONE;
	/* Timing */
	Timing.AddressSetupTime = 15;
	Timing.AddressHoldTime = 15;
	Timing.DataSetupTime = 255;
	Timing.BusTurnAroundDuration = 15;
	Timing.CLKDivision = 16;
	Timing.DataLatency = 17;
	Timing.AccessMode = FMC_ACCESS_MODE_A;
	/* ExtTiming */

	if (HAL_SRAM_Init(&hsram1, &Timing, NULL) != HAL_OK)
	{
		;
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
			GPIO_PIN_7;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStruct.Alternate = GPIO_AF12_FMC;
	HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

	//use this as data/command switch
	GPIO_InitStruct.Pin = GPIO_PIN_3;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

	/* USER CODE BEGIN FMC_MspInit 1 */

	/* USER CODE END FMC_MspInit 1 */
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