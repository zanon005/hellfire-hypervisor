
OUTPUT_FORMAT("elf64-littleriscv", "elf64-littleriscv",
          "elf64-littleriscv")
OUTPUT_ARCH(riscv)
ENTRY(_start)

MEMORY 
{
    ram     (rw!x) : ORIGIN = 0x00001000, LENGTH = 64K /* RAM area. */
}

/* define a global symbol _stack  */

_stack = ORIGIN(ram) + LENGTH(ram);
_stack_size = 1024;

/* now define the output sections  */

SECTIONS 
{
    .kernel 0x00001000:
    {
        _text = .;
        *(.text)
        *(.text.*)
        _etext = .;
    } > ram

    .__drivers_table_init :
    {
        . = ALIGN(4);
        __drivers_table_init_start = .;
        *(.__drivers_table_init)
        __drivers_table_init_end = .;
    } > ram
    
    .reginfo : {
        . = ALIGN(4);
        *(.reginfo)
    } > ram

    .rodata :
    {
        . = ALIGN(4);
        _rodata = .;
        *(.rodata)
        *(.rodata.*)
        _erodata = .;
    } > ram

    .data : 
    {
        . = ALIGN(4); 
        _data = .;
        *(.data)
        *(.data.*)
        _gp = .;
        *(.sdata)
        *(.sdata.*)
        _edata = .;
    } > ram

    .bss :
    {
        . = ALIGN(4);
        _bss_start = .; 
        *(.bss) 
        *(.bss.*)
        *(.sbss) 
        *(.sbss.*)
        *(.scommon)
        _bss_end = . ;
    } > ram
    

}
_end = .;
_heap_start = . ;
_heap_size = LENGTH(ram) - (_heap_start - ORIGIN(ram)) - 4 - _stack_size;
    

