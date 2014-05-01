	.syntax unified
        .cpu cortex-m4
        .fpu softvfp
        .eabi_attribute 20, 1
        .eabi_attribute 21, 1
        .eabi_attribute 23, 3
        .eabi_attribute 24, 1
        .eabi_attribute 25, 1
        .eabi_attribute 26, 1
        .eabi_attribute 30, 4
        .eabi_attribute 34, 1
        .eabi_attribute 18, 4
        .thumb
@
@   Teensy 3.x fiber support
@
            .data
            .align  3,0
headroom:   .word   8192                @ No. of bytes to reserve for the main stack
stackroot:  .word   0                   @ Initialized later

            .equ    _headroom,0
            .equ    _stackroot,4

            .text
            .align  3,0
            .global fiber_create,fiber_swap,fiber_set_main,fiber_getsp

@
@   void fiber_set_main(uint32_t stack_size)
@
@   Call is ignored if one or more fibers have already been created.
@
fiber_set_main:
            push    {r1,r2}
            ldr     r1,=headroom        @ Get address of headroom variable
            ldr     r2,[r1,#4]          @ Check stackroot
            cmp     r2,#0               @ Has a coroutine already been created?
            it      eq
            stmeq   r1,{r0}             @ If not, save main stack head_room (stackroot == 0)
            pop     {r1,r2}
            bx      lr

@
@   uint32_t *fiber_getsp()
@
fiber_getsp:
            mov     r0,sp               @ Return current sp value
            bx      lr
@
@   fiber_t offsets
@
            .equ    _sp,0
            .equ    _r2,_sp+4
            .equ    _r3,_r2+4
            .equ    _r4,_r3+4
            .equ    _r5,_r4+4
            .equ    _r6,_r5+4
            .equ    _r7,_r6+4
            .equ    _r8,_r7+4
            .equ    _r9,_r8+4
            .equ    _r10,_r9+4
            .equ    _r11,_r10+4
            .equ    _r12,_r11+4
            .equ    _lr,_r12+4
            .equ    _funcptr,_lr+4
            .equ    _arg,_funcptr+4
            .equ    _stack_size,_arg+4
@
@   void fiber_create(fiber_t,stacksize,func,arg)
@                     r0      r1        r2   r3
fiber_create:
            str     r0,[r0,#_r11]        @ Save &fiber_t in r11
            str     r1,[r0,#_stack_size] @ Save stack_size argument
            str     r2,[r0,#_funcptr]    @ Save function ptr
            str     r3,[r0,#_arg]        @ Save function argument
@
@   Test if we need to allow for main thread's minimum head room
@
            push    {r5,r12}             @ Save regs that we use
            ldr     r3,=headroom	@ Address of headroom variable
            ldr     r12,[r3,#_stackroot]
            cmp     r12,#0
            it      eq                  @ stackroot == 0?
            moveq   r12,sp              @ Main stack not set yet, r12 = sp
@
@   r12 = new coroutine's stack pointer
@
            ldr     r5,[r3,#_headroom]  @ Load headroom needed for main stack		
            sub     r12,r5              @ Subtract stack headroom needed
            str     r12,[r3,#_stackroot] @ Save new stack root (coroutine's sp)
            str     r1,[r3,#_headroom]   @ Save next headroom
@
@   Finalize coroutine startup params
@
            ldr     r1,=fibstart        @ Start routine for fiber
            add     r1,#1               @ Make it odd
            str     r1,[r0,#_lr]        @ Stuff return addr (lr) with fibestart

            stmia   r0!,{r12}           @ Save coroutine's new sp @ _sp
            pop     {r5,r12}            @ restore regs that we used
            stmia   r0!,{r2-r10}        @ Save r2-r10 egs to tfiber_t struct
            add     r0,#4               @ Skip _r11 that we set already
            stmia   r0!,{r12}           @ Save r12
            bx      lr                  @ Return to caller
@
@   Fiber startup
@
fibstart:   ldr     r0,[r11,#_arg]      @ Fetch calling arg (r11 = &fiber_t)
            ldr     r1,[r11,#_funcptr]  @ Fiber function to start

loop:       push    {r0,r1,r11}         @ Save arg and funcptr
            blx     r1                  @ Call func (r1) with arg (r0)
            pop     {r0,r1,r11}         @ Restore arg and funcptr
            b       loop                @ And repeat
@
@       void fiber_swap(tfiber_t *newfiber,const tfiber_t *curfiber);
@
fiber_swap: push    {r0}                @ push &newfiber
            mov     r0,sp               
            add     r0,#4               @ Adjust sp value for push above
            stmia   r1!,{r0}            @ Save current sp for context switch
            pop     {r0}                @ Restore r0, which is &newfiber
            stmia   r1!,{r2-r12,lr}  	@ Save remaining regs: r2-r12,lr

            ldmia   r0!,{r1}            @ Restore sp into r1
            mov     sp,r1               @ Set sp for coroutine
            ldmia   r0,{r2-r12,lr}   	@ restore r2-r12, lr
            bx      lr                  @ return from tfiber_swap()

            .end
