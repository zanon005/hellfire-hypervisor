
.equ REGBYTES, 8

.macro lxsp a, b
ld \a, ((\b)*REGBYTES)(sp)
.endm

.macro sxsp a, b
sd \a, ((\b)*REGBYTES)(sp)
.endm
    
    .align 2
trap_vector:
    # Save registers.
    addi    sp, sp, -190
    sxsp    ra, 0
    sxsp    a0, 1
    sxsp    a1, 2
    sxsp    a2, 3
    sxsp    a3, 4
    sxsp    a4, 5
    sxsp    a5, 6
    sxsp    a6, 7
    sxsp    a7, 8
    sxsp    t0, 9
    sxsp    t1, 10
    sxsp    t2, 11
    sxsp    t3, 12
    sxsp    t4, 13
    sxsp    t5, 14
    sxsp    t6, 15

    # Invoke the handler.
    mv      a0, sp
    csrr    a1, mcause
    csrr    a2, mepc
    jal     general_exception_handler

    # Restore registers.
    lxsp    ra, 0
    lxsp    a0, 1
    lxsp    a1, 2
    lxsp    a2, 3
    lxsp    a3, 4
    lxsp    a4, 5
    lxsp    a5, 6
    lxsp    a6, 7
    lxsp    a7, 8
    lxsp    t0, 9
    lxsp    t1, 10
    lxsp    t2, 11
    lxsp    t3, 12
    lxsp    t4, 13
    lxsp    t5, 14
    lxsp    t6, 15
    addi sp, sp, 190
    mret

.globl _start
_start:
    
    # setup default trap vector
    la      t0, trap_vector
    csrw    mtvec, t0

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
    
