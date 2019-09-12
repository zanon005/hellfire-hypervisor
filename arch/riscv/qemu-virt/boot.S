.globl _start
_start:
    
    la  gp, _gp
    la  sp, _stack
    la  a0, _bss_end
    la  a1, _bss_start
BSS_CLEAR:
1:  sw  zero, 0(a1)
    addi a1, a1, 4
    bne a1, a0, 1b
    nop
    jal hyper_init
    nop
2:  j 2b
    nop

    .section .text
    .global gpr_context_save
gpr_context_save:
    nop
    
    .section .text
    .global gpr_context_restore
gpr_context_restore:
    nop
    
