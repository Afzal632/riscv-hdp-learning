delay:
        push    rbp
        mov     rbp, rsp
        sub     rsp, 32
        mov     DWORD PTR [rbp-20], edi
        mov     eax, DWORD PTR [rbp-20]
        mov     DWORD PTR [rbp-4], eax
        call    clock
        mov     QWORD PTR [rbp-16], rax
        nop
.L2:
        call    clock
        mov     edx, DWORD PTR [rbp-4]
        movsxd  rcx, edx
        mov     rdx, QWORD PTR [rbp-16]
        add     rdx, rcx
        cmp     rax, rdx
        jl      .L2
        nop
        nop
        leave
        ret
.LC0:
        .string "Count value is: %d\n"
display:
        push    rbp
        mov     rbp, rsp
        sub     rsp, 16
        mov     DWORD PTR [rbp-4], edi
        mov     eax, DWORD PTR [rbp-4]
        mov     esi, eax
        mov     edi, OFFSET FLAT:.LC0
        mov     eax, 0
        call    printf
        nop
        leave
        ret
main:
        push    rbp
        mov     rbp, rsp
        sub     rsp, 16
        mov     DWORD PTR [rbp-4], 15
.L6:
        mov     eax, DWORD PTR [rbp-4]
        mov     edi, eax
        call    display
        sub     DWORD PTR [rbp-4], 1
        cmp     DWORD PTR [rbp-4], -1
        jne     .L5
        mov     DWORD PTR [rbp-4], 15
.L5:
        mov     edi, 500000
        call    delay
        jmp     .L6