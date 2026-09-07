__SP_H__ = 0x3e
__SP_L__ = 0x3d
__SREG__ = 0x3f
__tmp_reg__ = 0
__zero_reg__ = 1
delay:
        push r8
        push r9
        push r10
        push r11
        push r12
        push r13
        push r14
        push r15
        push r16
        push r17
        push r28
        push r29
        in r28,__SP_L__
        in r29,__SP_H__
        sbiw r28,8
        in __tmp_reg__,__SREG__
        cli
        out __SP_H__,r29
        out __SREG__,__tmp_reg__
        out __SP_L__,r28
.L__stack_usage = 20
        std Y+8,r25
        std Y+7,r24
        ldd r24,Y+7
        ldd r25,Y+8
        std Y+2,r25
        std Y+1,r24
        rcall clock
        mov r27,r25
        mov r26,r24
        mov r25,r23
        mov r24,r22
        std Y+3,r24
        std Y+4,r25
        std Y+5,r26
        std Y+6,r27
        nop
.L2:
        rcall clock
        mov r12,r22
        mov r13,r23
        mov r14,r24
        mov r15,r25
        ldd r24,Y+1
        ldd r25,Y+2
        mov r9,r25
        mov r8,r24
        lsl r25
        sbc r10,r10
        sbc r11,r11
        lds r24,_CLOCKS_PER_SEC_
        lds r25,_CLOCKS_PER_SEC_+1
        mov __tmp_reg__,r25
        lsl r0
        sbc r26,r26
        sbc r27,r27
        mov r18,r24
        mov r19,r25
        mov r20,r26
        mov r21,r27
        mov r25,r11
        mov r24,r10
        mov r23,r9
        mov r22,r8
        rcall __mulsi3
        mov r27,r25
        mov r26,r24
        mov r25,r23
        mov r24,r22
        ldi r16,lo8(64)
        ldi r17,lo8(66)
        ldi r18,lo8(15)
        ldi r19,0
        mov r22,r24
        mov r23,r25
        mov r24,r26
        mov r25,r27
        mov r21,r19
        mov r20,r18
        mov r19,r17
        mov r18,r16
        rcall __udivmodsi4
        mov r27,r21
        mov r26,r20
        mov r25,r19
        mov r24,r18
        mov r20,r24
        mov r21,r25
        mov r22,r26
        mov r23,r27
        ldd r24,Y+3
        ldd r25,Y+4
        ldd r26,Y+5
        ldd r27,Y+6
        add r24,r20
        adc r25,r21
        adc r26,r22
        adc r27,r23
        cp r12,r24
        cpc r13,r25
        cpc r14,r26
        cpc r15,r27
        brsh .+2
        rjmp .L2
        nop
        nop
adiw r28,8
        in __tmp_reg__,__SREG__
        cli
        out __SP_H__,r29
        out __SREG__,__tmp_reg__
        out __SP_L__,r28
        pop r29
        pop r28
        pop r17
        pop r16
        pop r15
        pop r14
        pop r13
        pop r12
        pop r11
        pop r10
        pop r9
        pop r8
        ret
.LC0:
        .string "Count value is: %d\n"
display:
        push r28
        push r29
        rcall .
        in r28,__SP_L__
        in r29,__SP_H__
.L__stack_usage = 4
        std Y+2,r25
        std Y+1,r24
        ldd r24,Y+2
        push r24
        ldd r24,Y+1
        push r24
        ldi r24,lo8(.LC0)
        ldi r25,hi8(.LC0)
        mov r24,r25
        push r24
        ldi r24,lo8(.LC0)
        ldi r25,hi8(.LC0)
        push r24
        rcall printf
        pop __tmp_reg__
        pop __tmp_reg__
        pop __tmp_reg__
        pop __tmp_reg__
        nop
pop __tmp_reg__
        pop __tmp_reg__
        pop r29
        pop r28
        ret
main:
        push r28
        push r29
        rcall .
        in r28,__SP_L__
        in r29,__SP_H__
.L__stack_usage = 4
        ldi r24,lo8(15)
        ldi r25,0
        std Y+2,r25
        std Y+1,r24
.L6:
        ldd r24,Y+1
        ldd r25,Y+2
        rcall display
        ldd r24,Y+1
        ldd r25,Y+2
        sbiw r24,1
        std Y+2,r25
        std Y+1,r24
        ldd r24,Y+1
        ldd r25,Y+2
        cpi r24,-1
        ldi r18,-1
        cpc r25,r18
        brne .L5
        ldi r24,lo8(15)
        ldi r25,0
        std Y+2,r25
        std Y+1,r24
.L5:
        ldi r24,lo8(32)
        ldi r25,lo8(-95)
        rcall delay
        rjmp .L6