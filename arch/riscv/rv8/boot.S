.section .text
.globl _start
_start:
    
    la  gp, _gp
    la  sp, _stack
    la  a0, _bss_end
    la  a1, _bss_start
BSS_CLEAR:
    sw  zero, 0(a1)
    slt v1, a1, a0
    addiu   a1, a1, 4
    bnez    v1, BSS_CLEAR
    nop

    jal hyper_init
    nop

neverend:
    j neverend
    nop
