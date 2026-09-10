#include <stdio.h>

enum {
    ALU_ADD = 0,
    ALU_SUB = 1,
    ALU_AND = 2,
    ALU_OR  = 3,
    ALU_XOR = 4
};

unsigned int alu4(unsigned int a, unsigned int b, unsigned int operation)
{
    unsigned int result;

    switch (operation) {
    case ALU_ADD:
        result = a + b;
        break;

    case ALU_SUB:
        result = a - b;
        break;

    case ALU_AND:
        result = a & b;
        break;

    case ALU_OR:
        result = a | b;
        break;

    case ALU_XOR:
        result = a ^ b;
        break;

    default:
        result = 0;
        break;
    }

    return result & 0xF;
}

int signed4(unsigned int value)
{
    value &= 0xF;

    if (value & 0x8)
        return (int)value - 16;

    return (int)value;
}

void print_binary4(unsigned int value)
{
    int bit;

    for (bit = 3; bit >= 0; bit--)
        putchar((value & (1U << bit)) ? '1' : '0');
}

int main(void)
{
    unsigned int a;
    unsigned int b;
    unsigned int operation;
    unsigned int result;

    printf("4-bit ALU operations:\n");
    printf("0 = ADD\n");
    printf("1 = SUBTRACT\n");
    printf("2 = AND\n");
    printf("3 = OR\n");
    printf("4 = XOR\n");
    printf("Enter A B operation: ");

    if (scanf("%u %u %u", &a, &b, &operation) != 3) {
        printf("Invalid input\n");
        return 1;
    }

    if (a > 15 || b > 15) {
        printf("A and B must be between 0 and 15\n");
        return 1;
    }

    if (operation > ALU_XOR) {
        printf("Operation must be between 0 and 4\n");
        return 1;
    }

    result = alu4(a, b, operation);

    printf("A       = %2u = ", a);
    print_binary4(a);

    printf("\nB       = %2u = ", b);
    print_binary4(b);

    printf("\nResult  = %2u = ", result);
    print_binary4(result);

    printf("\nSigned result = %d\n", signed4(result));

    return 0;
}
