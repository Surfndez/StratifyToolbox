flash_page_count = 1;


MEMORY {
	 flash : ORIGIN = 0x90000000, LENGTH = 2048K - 64K
	 ram : ORIGIN = 0x24000000, LENGTH = 64K
	 tcim : ORIGIN = 0x00000000, LENGTH = 64K
	 sysmem : ORIGIN = 0x20010000, LENGTH = 64K
	 ram1 : ORIGIN = 0x30000000, LENGTH = 288K
	 ram2 : ORIGIN = 0x38000000, LENGTH = 64K
}

PROVIDE ( _flash_start = 0x08000000 );


SECTIONS {

	.sysmem : {
		_sys = . ;
		*(.sysmem)
		. = ALIGN(1024);
	} > sysmem
	_esys = .;

	. = 0;
	.tcim : {
    	_tcim = .;
    	*\libsos_mcu_stm32h750xx*.a:*(.startup)
    	*\libsos_mcu_stm32h750xx*.a:*(.reset_vector)
    	*\libsos_mcu_stm32h750xx*.a:*(.isr_vector)
    	*\libsos_mcu_stm32h750xx*.a:*(.text.*_isr)
    	*\libsos_mcu_stm32h750xx*.a:*(.text.*_IRQHandler*)
    	*\libsos_mcu_stm32h750xx*.a:*(.text.*mcu_pio_*)
    	*\libsos_mcu_stm32h750xx*.a:*(.text.*HAL_GPIO_*)
    	*\libsos_mcu_stm32h750xx*.a:*(.text.*mcu_usb_*)
    	*\libsos_mcu_stm32h750xx*.a:*(.text.*HAL_PCD_*)
		*\libsos_mcu_stm32h750xx*.a:*(.text.*mcu_qspi_*)
    	*\libsos_mcu_stm32h750xx*.a:*(.text.*HAL_QSPI_*)
		*\libsos_mcu_stm32h750xx*.a:*(.text.*mcu_tmr_*)
    	*\libsos_mcu_stm32h750xx*.a:*(.text.*HAL_TIM_*)
		*\libsos_mcu_stm32h750xx*.a:*(.text.*mcu_sdio_*)
    	*\libsos_mcu_stm32h750xx*.a:*(.text.*HAL_SD_*)
    	*\libsos_mcu_stm32h750xx*.a:*(.text.*stm32_dma*)
    	*\libsos_mcu_stm32h750xx*.a:*(.text.*read)
    	*\libsos_mcu_stm32h750xx*.a:*(.text.*write)
    	*\libsos_sys*.a:*(.text.cortexm_*)
    	*\libsos_sys*.a:*(.text.task_*)
    	*\libsos_sys*.a:*(.text.devfs_*)
    	*\libsos_sys*.a:*(.text.scheduler_*)
    	*\libsos_sys*.a:*(.text.pthread_*)
    	*\libsos_sys*.a:*(.text.sysfs_*)
    	*\libsos_sys*.a:*(.text.usbd_*)
    	*\libsos_sys*.a:*(.text.open)
    	*\libsos_sys*.a:*(.text.close)
    	*\libsos_sys*.a:*(.text.read)
    	*\libsos_sys*.a:*(.text.write)
    	*\libsos_sys*.a:*(.text.ioctl)
    	*\libsos_sys*.a:*(.text.link_*)
		*(.text.nm_*)
		*(.text.m2m_*)
		*(.text.*_device_*)
		*(.text.LCD_IO*)
		_priv_code = .;
        *(.priv_code)
        _priv_exec_code = .;
        *(.priv_exec_code)
        _epriv_code = .;
  	} >tcim
	. = ALIGN(32);
  	_etcim = .;

	.text : {
		_text = .;
  		 KEEP(*(.startup))
  		 KEEP(*(.reset_vector))
  		 KEEP(*(.isr_vector))
         *(.text)
         *(.init)
         *(.fini)
         *(.eh_frame)
         _ctors = .;
         KEEP(*(.init_array))
         KEEP(*(.ctors))
         _dtors = .;
         KEEP(*(.dtors))
         KEEP(*(.fini_array))
         _dtors_end = .;
         *(.jcr)
         *(.rodata)
         *(.rodata*)
         *(.glue_7)
         *(.glue_7t)

       	 *(.text.*)

	} >flash
  
	.ARM.extab : 
	{
		*(.ARM.extab* .gnu.linkonce.armextab.*)
	} > flash
	__exidx_start = .;
	.ARM.exidx   : { 
    	*(.ARM.exidx* .gnu.linkonce.armexidx.*) 
  	} >flash
  
  	. = ALIGN(4);
  	__exidx_end = .;
  	_etext = .;
  	.data : {
         _data = .;
         *(.data)
         *(.data.*)
         } >ram AT>flash
  	_edata = .;
  	.bss : {
    	_bss = .;
         *(.bss)
         *(.bss.*)
         } >ram
  	. = ALIGN(4);
  	_ebss = .;

}

PROVIDE ( _end = . );
PROVIDE ( _top_of_stack = (ORIGIN(sysmem) + LENGTH(sysmem)) );
PROVIDE ( _estack = (ORIGIN(sysmem) + LENGTH(sysmem)) );
PROVIDE ( _flash_start = ORIGIN(flash) );
PROVIDE ( _flash_pages = flash_page_count );
PROVIDE ( _flash_size = LENGTH(flash) );
PROVIDE ( _ram_pages = ( (LENGTH(ram) + LENGTH(ram1) + LENGTH(ram2)) / 1024 ) );
PROVIDE ( _sram_size = LENGTH(ram) );
PROVIDE ( _sram_start = ORIGIN(ram) );
PROVIDE ( _sram1_size = LENGTH(ram1) );
PROVIDE ( _sram1_start = ORIGIN(ram1) );
PROVIDE ( _sram2_size = LENGTH(ram2) );
PROVIDE ( _sram2_start = ORIGIN(ram2) );
PROVIDE ( _unique_id = 0x1FF0F420 );
