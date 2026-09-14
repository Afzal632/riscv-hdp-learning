#include <stdio.h>

static long pk_write_character(char character)
{
    register long a0 asm("a0") = 1;
    register const char *a1 asm("a1") = &character;
    register long a2 asm("a2") = 1;
    register long a7 asm("a7") = 64;

    asm volatile (
        "ecall"
        : "+r" (a0)
        : "r" (a1), "r" (a2), "r" (a7)
        : "memory"
    );

    return a0;
}

static int pk_putc(char character, FILE *stream)
{
    (void)stream;

    if (pk_write_character(character) != 1)
        return EOF;

    return (unsigned char)character;
}

static FILE pk_console =
    FDEV_SETUP_STREAM(pk_putc, NULL, NULL, _FDEV_SETUP_WRITE);

FILE *const stdout = &pk_console;
FILE *const stderr = &pk_console;
FILE *const stdin  = &pk_console;

__attribute__((noreturn))
void _exit(int status)
{
    register long a0 asm("a0") = status;
    register long a7 asm("a7") = 93;

    asm volatile (
        "ecall"
        :
        : "r" (a0), "r" (a7)
        : "memory"
    );

    for (;;)
        ;
}
