OUTPUT_ARCH( "riscv" )

ENTRY( _start )

MEMORY{
  ram   (wxa!ri) : ORIGIN = 0x80000000, LENGTH = RAM_SIZE
}

_stack = ORIGIN(ram) + LENGTH(ram);
_stack_size = STACK_SIZE;

PHDRS{
  text PT_LOAD;
  data PT_LOAD;
  bss PT_LOAD;
}

SECTIONS{
  .text : {
    PROVIDE(_text_start = .);
    *(.e_entry)
    *(.text.init) *(.text .text.*)
    PROVIDE(_text_end = .);
  } >ram AT>ram :text

  .rodata : {
    PROVIDE(_rodata_start = .);
    *(.rodata .rodata.*)
    PROVIDE(_rodata_end = .);
  } >ram AT>ram :text


  .data : {
    . = ALIGN(4096);
    PROVIDE(_data_start = .);
    *(.sdata .sdata.*) *(.data .data.*)
    PROVIDE(_data_end = .);
  } >ram AT>ram :data

  .bss :{
    PROVIDE(_bss_start = .);
    *(.sbss .sbss.*) *(.bss .bss.*)
    PROVIDE(_bss_end = .);
  } >ram AT>ram :bss

  PROVIDE(_memory_start = ORIGIN(ram));
  PROVIDE(_memory_end = ORIGIN(ram) + LENGTH(ram));
}

_end = .;
. = ALIGN(4); 
_heap_start = . ;
_heap_size = LENGTH(ram) - (_heap_start - ORIGIN(ram)) - 4 - _stack_size;
