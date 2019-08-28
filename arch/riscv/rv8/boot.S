.section .text
.globl _start
_start:
    
    la  gp, _gp
    la  sp, _stack
    la  a0, _bss_end
    la  a1, _bss_start
BSS_CLEAR:
    sw  zero, 0(a1)
    
    
    
    nop

    jal hyper_init
    nop

neverend:
    j neverend
    nop


    .align 2
    .section .text
    .global gpr_context_save
gpr_context_save:
    nop
    
    .align 2
    .section .text
    .global gpr_context_restore
gpr_context_restore:
    nop
    
