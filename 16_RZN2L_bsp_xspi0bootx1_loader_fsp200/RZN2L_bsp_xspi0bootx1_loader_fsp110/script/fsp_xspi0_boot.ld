/*
                  Linker File for Renesas RZ/N2L FSP
*/

MEMORY
{
    ATCM : ORIGIN = 0x00000000, LENGTH = 0x00020000
    BTCM : ORIGIN = 0x00100000, LENGTH = 0x00020000
    SYSTEM_RAM : ORIGIN = 0x10000000, LENGTH = 0x00180000
    SYSTEM_RAM_MIRROR : ORIGIN = 0x30000000, LENGTH = 0x00180000
    xSPI0_CS0_SPACE_MIRROR : ORIGIN = 0x40000000, LENGTH = 0x04000000
    xSPI0_CS1_SPACE_MIRROR : ORIGIN = 0x44000000, LENGTH = 0x04000000
    xSPI1_CS0_SPACE_MIRROR : ORIGIN = 0x48000000, LENGTH = 0x04000000
    xSPI1_CS1_SPACE_MIRROR : ORIGIN = 0x4C000000, LENGTH = 0x04000000
    CS0_SPACE_MIRROR : ORIGIN = 0x50000000, LENGTH = 0x04000000
    CS2_SPACE_MIRROR : ORIGIN = 0x54000000, LENGTH = 0x04000000
    CS3_SPACE_MIRROR : ORIGIN = 0x58000000, LENGTH = 0x04000000
    CS5_SPACE_MIRROR : ORIGIN = 0x5C000000, LENGTH = 0x04000000
    xSPI0_CS0_SPACE : ORIGIN = 0x60000000, LENGTH = 0x04000000
    xSPI0_CS1_SPACE : ORIGIN = 0x64000000, LENGTH = 0x04000000
    xSPI1_CS0_SPACE : ORIGIN = 0x68000000, LENGTH = 0x04000000
    xSPI1_CS1_SPACE : ORIGIN = 0x6C000000, LENGTH = 0x04000000
    CS0_SPACE : ORIGIN = 0x70000000, LENGTH = 0x04000000
    CS2_SPACE : ORIGIN = 0x74000000, LENGTH = 0x04000000
    CS3_SPACE : ORIGIN = 0x78000000, LENGTH = 0x04000000
    CS5_SPACE : ORIGIN = 0x7C000000, LENGTH = 0x04000000
}

SECTIONS
{
    .loader_param 0x60000000 : AT (0x60000000)
    {
        KEEP(*(.loader_param))
    } > xSPI0_CS0_SPACE
    .flash_contents 0x6000004C : AT (0x6000004C)
    {
        _mloader_text = .;
        . = . + (_loader_text_end - _loader_text_start);
        _mloader_data = .;
        . = . + (_loader_data_end - _loader_data_start);
        _mfvector = .;
        . = . + (_fvector_end - _fvector_start);
        _mtext = .;
        . = . + (_text_end - _text_start);
        _mdata = .;
        . = . + (_data_end - _data_start);
        flash_contents_end = .;
    } > xSPI0_CS0_SPACE
    .loader_text 0x00102000 : AT (_mloader_text)
    {
        _loader_text_start = .;
        *(.loader_text)
        */fsp/src/bsp/cmsis/Device/RENESAS/Source/*.o(.text*)
        */fsp/src/bsp/mcu/all/bsp_clocks.o(.text*)
        */fsp/src/bsp/mcu/all/bsp_irq.o(.text*)
        */fsp/src/bsp/mcu/all/bsp_register_protection.o(.text*)
        */fsp/src/r_ioport/r_ioport.o(.text*)
        KEEP(*(.warm_start))
        . = . + (512 - ((. - _loader_text_start) % 512));
        _loader_text_end = .;
    } > BTCM
    .loader_data : AT (_mloader_data)
    {
        _loader_data_start = .;
        __loader_data_start = .;
        */fsp/src/bsp/cmsis/Device/RENESAS/Source/*.o(.data*)
        */fsp/src/bsp/mcu/all/bsp_clocks.o(.data*)
        */fsp/src/bsp/mcu/all/bsp_irq.o(.data*)
        */fsp/src/bsp/mcu/all/bsp_register_protection.o(.data*)
        */fsp/src/r_ioport/r_ioport.o(.data*)
        __loader_data_end = .;
        __loader_bss_start = .;
        */fsp/src/bsp/cmsis/Device/RENESAS/Source/*.o(.bss*)
        */fsp/src/bsp/mcu/all/bsp_clocks.o(.bss*)
        */fsp/src/bsp/mcu/all/bsp_irq.o(.bss*)
        */fsp/src/bsp/mcu/all/bsp_register_protection.o(.bss*)
        */fsp/src/r_ioport/r_ioport.o(.bss*)
        */fsp/src/bsp/cmsis/Device/RENESAS/Source/*.o(COMMON)
        */fsp/src/bsp/mcu/all/bsp_clocks.o(COMMON)
        */fsp/src/bsp/mcu/all/bsp_irq.o(COMMON)
        */fsp/src/bsp/mcu/all/bsp_register_protection.o(.COMMON)
        */fsp/src/r_ioport/r_ioport.o(.COMMON)
        __loader_bss_end = . ;
        _loader_data_end = .;
    } > BTCM
    .intvec 0x00000000 : AT (_mfvector)
    {
        _fvector_start = .;
        KEEP(*(.intvec))
        _fvector_end = .;
    } > ATCM
    .text 0x00000100 : AT (_mtext)
    {
        _text_start = .;
        *(.text*)

        KEEP(*(.init))
        KEEP(*(.fini))

        /* .ctors */
        *crtbegin.o(.ctors)
        *crtbegin?.o(.ctors)
        *(EXCLUDE_FILE(*crtend?.o *crtend.o) .ctors)
        *(SORT(.ctors.*))
        *(.ctors)
        _ctor_end = .;

        /* .dtors */
        *crtbegin.o(.dtors)
        *crtbegin?.o(.dtors)
        *(EXCLUDE_FILE(*crtend?.o *crtend.o) .dtors)
        *(SORT(.dtors.*))
        *(.dtors)
        _dtor_end = .;

        KEEP(*(.eh_frame*))
    } > ATCM
    .rvectors :
    {
        _rvectors_start = .;
        KEEP(*(.rvectors))
        _rvectors_end = .;
    } > ATCM
    .ARM.extab :
    {
        *(.ARM.extab* .gnu.linkonce.armextab.*)
    } > ATCM
    __exidx_start = .;
    .ARM.exidx :
    {
        *(.ARM.exidx* .gnu.linkonce.armexidx.*)
    } > ATCM
    __exidx_end = .;
    .got :
    {
        *(.got)
        *(.got.plt)
        _text_end = .;
    } > ATCM
    .data :  AT (_mdata)
    {
        _data_start = .;

        *(vtable)
        *(.data.*)
        *(.data)

        *(.rodata*)
        _erodata = .;

        . = ALIGN(4);
        /* preinit data */
        PROVIDE_HIDDEN (__preinit_array_start = .);
        KEEP(*(.preinit_array))
        PROVIDE_HIDDEN (__preinit_array_end = .);

        . = ALIGN(4);
        /* init data */
        PROVIDE_HIDDEN (__init_array_start = .);
        KEEP(*(SORT(.init_array.*)))
        KEEP(*(.init_array))
        PROVIDE_HIDDEN (__init_array_end = .);

        . = ALIGN(4);
        /* finit data */
        PROVIDE_HIDDEN (__fini_array_start = .);
        KEEP(*(SORT(.fini_array.*)))
        KEEP(*(.fini_array))
        PROVIDE_HIDDEN (__fini_array_end = .);

        KEEP(*(.jcr*))

        . = ALIGN(4);

        /* All data end */
        _data_end = .;
    } > ATCM
    .bss :
    {
        . = ALIGN(4);
        __bss_start__ = .;
        _bss = .;
        *(.bss*)
        *(COMMON)
        . = ALIGN(4);
        __bss_end__ = .;
        _ebss = .;
        _end = .;
    } > ATCM
    .heap (NOLOAD) :
    {
        . = ALIGN(8);
        __HeapBase = .;
        /* Place the STD heap here. */
        KEEP(*(.heap))
        __HeapLimit = .;
    } > ATCM
    .thread_stack (NOLOAD):
    {
        . = ALIGN(8);
        __ThreadStackBase = .;
        /* Place the Thread stacks here. */
        KEEP(*(.stack*))
        __ThreadStackLimit = .;
    } > ATCM
    .sys_stack (NOLOAD) :
    {
        . = ALIGN(8);
        __SysStackBase = .;
        /* Place the sys_stack here. */
        KEEP(*(.sys_stack))
        __SysStackLimit = .;
    } > BTCM
    .svc_stack (NOLOAD) :
    {
        . = ALIGN(8);
        __SvcStackBase = .;
        /* Place the svc_stack here. */
        KEEP(*(.svc_stack))
        __SvcStackLimit = .;
    } > BTCM
    .irq_stack (NOLOAD) :
    {
        . = ALIGN(8);
        __IrqStackBase = .;
        /* Place the irq_stack here. */
        KEEP(*(.irq_stack))
        __IrqStackLimit = .;
    } > BTCM
    .fiq_stack (NOLOAD) :
    {
        . = ALIGN(8);
        __FiqStackBase = .;
        /* Place the fiq_stack here. */
        KEEP(*(.fiq_stack))
        __FiqStackLimit = .;
    } > BTCM
    .und_stack (NOLOAD) :
    {
        . = ALIGN(8);
        __UndStackBase = .;
        /* Place the und_stack here. */
        KEEP(*(.und_stack))
        __UndStackLimit = .;
    } > BTCM
    .abt_stack (NOLOAD) :
    {
        . = ALIGN(8);
        __AbtStackBase = .;
        /* Place the abt_stack here. */
        KEEP(*(.abt_stack))
        __AbtStackLimit = .;
    } > BTCM
    .shared_noncache_buffer 0x300C0000 : AT (0x300C0000)
    {
        . = ALIGN(32);
        _sncbuffer_start = .;
        KEEP(*(.shared_noncache_buffer*))
        _sncbuffer_end = .;
    } > SYSTEM_RAM_MIRROR
    .noncache_buffer 0x30100000 : AT (0x30100000)
    {
        . = ALIGN(32);
        _ncbuffer_start = .;
        KEEP(*(.noncache_buffer*))
        _ncbuffer_end = .;
    } > SYSTEM_RAM_MIRROR
}

