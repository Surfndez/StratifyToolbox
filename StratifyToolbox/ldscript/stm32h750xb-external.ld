flash_page_count = 1;


MEMORY {
	 flash : ORIGIN = 0x90000000, LENGTH = 2048K
	 ram : ORIGIN = 0x24000000, LENGTH = 64K
	 tcim : ORIGIN = 0x00000000, LENGTH = 64K
	 sysmem : ORIGIN = 0x20000000, LENGTH = 64K
	 ram1 : ORIGIN = 0x30000000, LENGTH = 288K
	 ram2 : ORIGIN = 0x38000000, LENGTH = 64K
}

PROVIDE ( _flash_start = 0x08000000 );
INCLUDE ldscripts/stm32h7xx.ld

