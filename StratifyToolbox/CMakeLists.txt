cmake_minimum_required (VERSION 3.6)

if( ${CMAKE_HOST_SYSTEM_NAME} STREQUAL "Darwin" )
	set(SOS_TOOLCHAIN_CMAKE_PATH /Applications/StratifyLabs-SDK/Tools/gcc/arm-none-eabi/cmake)
endif()
if( ${CMAKE_HOST_SYSTEM_NAME} STREQUAL "Windows" )
	set(SOS_TOOLCHAIN_CMAKE_PATH C:/StratifyLabs-SDK/Tools/gcc/arm-none-eabi/cmake)
endif()

include(${SOS_TOOLCHAIN_CMAKE_PATH}/sos-variables.cmake)
include(${SOS_TOOLCHAIN_CMAKE_PATH}/sos-sdk.cmake)
set(CMAKE_TOOLCHAIN_FILE ${SOS_TOOLCHAIN_CMAKE_PATH}/sos-bsp-toolchain.cmake)

get_filename_component(SOS_NAME ${CMAKE_SOURCE_DIR} NAME)
project(${SOS_NAME} CXX C)

#Add sources to the project
sos_sdk_add_subdirectory(SOS_KERNEL_SOURCELIST src)
list(APPEND SOS_KERNEL_SOURCELIST ${SOURCES} sl_settings.json)

set(SOS_ARCH v7em_f5dh)
set(SOS_DEVICE stm32h750xbram)
set(SOS_DEVICE_FAMILY stm32h750xx)
set(SOS_HARDWARD_ID 0x00000007)
set(SOS_LIBRARIES
	jansson_kernel
	mbedtls_kernel
	sgfx_kernel
	fatfs_kernel
	)

set(SOS_INCLUDE_DIRECTORIES
	${TOOLCHAIN_INC_DIR}/mcu/arch/stm32/cmsis
	src/atwinc1500
	src
	)

set(SOS_KERNEL_START_ADDRESS 0x24000000)
set(SOS_BOOT_START_ADDRESS 0x08000000)

set(SOS_DEFINITIONS _IS_BOOT=0 _IS_FLASH=0 _IS_RAM=1)
set(SOS_START_ADDRESS ${SOS_KERNEL_START_ADDRESS})
set(SOS_SOURCELIST ${SOS_KERNEL_SOURCELIST})
set(SOS_CONFIG release)
include(${SOS_TOOLCHAIN_CMAKE_PATH}/sos-bsp.cmake)
set(SOS_CONFIG debug)
include(${SOS_TOOLCHAIN_CMAKE_PATH}/sos-bsp.cmake)

set(SOS_DEVICE stm32h750xb)
set(SOS_DEFINITIONS _IS_BOOT=1 _IS_FLASH=0 _IS_RAM=0)
set(SOS_START_ADDRESS ${SOS_BOOT_START_ADDRESS})
set(SOS_SOURCELIST ${SOS_KERNEL_SOURCELIST})
set(SOS_CONFIG boot_release)
include(${SOS_TOOLCHAIN_CMAKE_PATH}/sos-bsp.cmake)
set(SOS_CONFIG boot_debug)
include(${SOS_TOOLCHAIN_CMAKE_PATH}/sos-bsp.cmake)

set(SOS_KERNEL_START_ADDRESS 0x90000000)

set(SOS_LINKER_FILE ${CMAKE_SOURCE_DIR}/ldscript/stm32h750xb-external.ld)
set(SOS_DEFINITIONS _IS_BOOT=0  _IS_FLASH=1 _IS_RAM=0)
set(SOS_START_ADDRESS ${SOS_KERNEL_START_ADDRESS})
set(SOS_SOURCELIST ${SOS_KERNEL_SOURCELIST})
set(SOS_CONFIG flash_release)
include(${SOS_TOOLCHAIN_CMAKE_PATH}/sos-bsp.cmake)
set(SOS_CONFIG flash_debug)
include(${SOS_TOOLCHAIN_CMAKE_PATH}/sos-bsp.cmake)

add_custom_target(
	tcim_${SOS_NAME}_${SOS_CONFIG} DEPENDS ${BUILD_TARGET} COMMAND ${CMAKE_OBJCOPY} -j .tcim -O binary ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${BUILD_TARGET} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${SOS_NAME}.tcim.bin
	DEPENDS size_${SOS_NAME}_${SOS_CONFIG}
	)

