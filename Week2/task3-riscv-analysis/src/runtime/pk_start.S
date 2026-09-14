    .section .text.init
    .globl _start
    .type _start, @function

_start:
    .option push
    .option norelax
    la gp, __global_pointer$
    .option pop

    call main
    call _exit

1:
    j 1b

    .size _start, .-_start
