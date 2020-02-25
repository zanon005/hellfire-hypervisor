
.equ REGBYTES, 8

.macro lxsp a, b
ld \a, ((\b)*REGBYTES)(sp)
.endm

.macro sxsp a, b
sd \a, ((\b)*REGBYTES)(sp)
.endm
    

    .align 2
trap_vector:
    
    csrw    mscratch, sp
    la      sp, _stack          # set hypervisor stack
    addi    sp, sp, -256

    # Save registers.
    sxsp    ra, 1
    sxsp    gp, 3
    sxsp    tp, 4
    sxsp    t0, 5
    sxsp    t1, 6
    sxsp    t2, 7
    sxsp    s0, 8
    sxsp    s1, 9
    sxsp    a0, 10
    sxsp    a1, 11
    sxsp    a2, 12
    sxsp    a3, 13
    sxsp    a4, 14
    sxsp    a5, 15
    sxsp    a6, 16
    sxsp    a7, 17
    sxsp    s2, 18
    sxsp    s3, 19
    sxsp    s4, 20
    sxsp    s5, 21
    sxsp    s6, 22
    sxsp    s7, 23
    sxsp    s8, 24
    sxsp    s9, 25
    sxsp    s10, 26
    sxsp    s11, 27
    sxsp    t3, 28
    sxsp    t4, 29
    sxsp    t5, 30
    sxsp    t6, 31
    
    # Save sp
    csrr    a0, mscratch
    sxsp    a0, 2

    # Invoke the handler.
    csrr    a0, mcause
    csrr    a1, mepc
    jal     general_exception_handler

    # Restore registers.
    lxsp    ra, 1
    lxsp    gp, 3
    lxsp    tp, 4
    lxsp    t0, 5
    lxsp    t1, 6
    lxsp    t2, 7
    lxsp    s0, 8
    lxsp    s1, 9
    lxsp    a0, 10
    lxsp    a1, 11
    lxsp    a2, 12
    lxsp    a3, 13
    lxsp    a4, 14
    lxsp    a5, 15
    lxsp    s6, 16
    lxsp    s7, 17
    lxsp    s2, 18
    lxsp    s3, 19
    lxsp    s4, 20
    lxsp    s5, 21
    lxsp    s6, 22
    lxsp    s7, 23
    lxsp    s8, 24
    lxsp    s9, 25
    lxsp    s10, 26
    lxsp    s11, 27
    lxsp    t3, 28
    lxsp    t4, 29
    lxsp    t5, 30
    lxsp    t6, 31
    lxsp    sp, 2
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

    
