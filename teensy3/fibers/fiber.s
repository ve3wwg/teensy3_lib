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
@   void fiber_create(fiber_t,stacksize,func,arg)
@                     r0      r1        r2   r3
fiber_create:
            push    {r5,r6,r12}         @ Save regs that we use
@
@   Test if we need to allow for main thread's minimum head room
@
            ldr     r6,=headroom	@ Address of headroom variable
            ldr     r12,[r6,#_stackroot]
            cmp     r12,#0
            it      eq                  @ stackroot == 0?
            moveq   r12,sp              @ Main stack not set yet, r12 = sp
@
@   r12 = new coroutine's stack pointer
@
            ldr     r5,[r6,#0]          @ Load headroom needed for main stack		
            sub     r12,r5              @ Subtract stack headroom needed
            str     r12,[r6,#_stackroot] @ Save new stack root (coroutine's sp)
            str     r1,[r6,#_headroom]   @ Save next headroom
@
@   Finalize coroutine startup params
@
            stmia   r0!,{r12}           @ Save coroutine's new sp
            stmia   r0!,{r3}            @ Save arg1 for function in it's r0
            pop     {r5,r6,r12}         @ restore regs that we used
            stmia   r0!,{r2-r12}        @ Save r1-r12 egs to tfiber_t struct
            stmia   r0!,{r2}            @ This func addr will be lr for coroutine

            bx      lr 

@
@       void fiber_swap(tfiber_t *newfiber,const tfiber_t *curfiber);
@
fiber_swap:
            push    {r0}
            mov     r0,sp
            add     r0,#4               @ Adjust sp value for push above
            stmia   r1!,{r0}            @ Save sp for context switch
            pop     {r0}                @ Restore r0
            stmia   r1!,{r0,r2-r12,lr}  @ Save remaining regs: r0,r2-r12,lr

            ldmia   r0!,{r1}            @ Restore sp into r1
            mov     sp,r1               @ Set sp for coroutine
            ldmia   r0,{r0,r2-r12,lr}   @ restore r0, r2-r12, lr
            bx      lr                  @ return from tfiber_swap()

@datasect:  .word   .data
            .end
