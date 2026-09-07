delay:
        push    {r7, lr}
        sub     sp, sp, #16
        add     r7, sp, #0
        str     r0, [r7, #4]
        ldr     r3, [r7, #4]
        str     r3, [r7, #12]
        bl      clock
        str     r0, [r7, #8]
        nop
.L2:
        bl      clock
        mov     r1, r0
        ldr     r2, [r7, #8]
        ldr     r3, [r7, #12]
        add     r3, r3, r2
        cmp     r1, r3
        blt     .L2
        nop
        nop
        adds    r7, r7, #16
        mov     sp, r7
        pop     {r7, pc}
.LC0:
        .ascii  "Count value is: %d\012\000"
display:
        push    {r7, lr}
        sub     sp, sp, #8
        add     r7, sp, #0
        str     r0, [r7, #4]
        ldr     r1, [r7, #4]
        movw    r0, #:lower16:.LC0
        movt    r0, #:upper16:.LC0
        bl      printf
        nop
        adds    r7, r7, #8
        mov     sp, r7
        pop     {r7, pc}
main:
        push    {r7, lr}
        sub     sp, sp, #8
        add     r7, sp, #0
        movs    r3, #15
        str     r3, [r7, #4]
.L6:
        ldr     r0, [r7, #4]
        bl      display
        ldr     r3, [r7, #4]
        subs    r3, r3, #1
        str     r3, [r7, #4]
        ldr     r3, [r7, #4]
        cmp     r3, #-1
        bne     .L5
        movs    r3, #15
        str     r3, [r7, #4]
.L5:
        movw    r0, #41248
        movt    r0, 7
        bl      delay
        b       .L6