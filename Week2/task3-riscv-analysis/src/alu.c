#include <stdio.h>

enum {
    ALU_ADD = 0,
    ALU_SUB,
    ALU_AND,
    ALU_OR,
    ALU_XOR
};

__attribute__((noinline, noclone))
unsigned int alu4(unsigned int a, unsigned int b, unsigned int operation)
{
    unsigned int result;

    switch (operation) {
    case ALU_ADD: result = a + b; break;
    case ALU_SUB: result = a - b; break;
    case ALU_AND: result = a & b; break;
    case ALU_OR:  result = a | b; break;
    case ALU_XOR: result = a ^ b; break;
    default:      result = 0;     break;
    }

    return result & 0xFU;
}

int signed4(unsigned int value)
{
    value &= 0xFU;
    return (value & 0x8U) ? (int)value - 16 : (int)value;
}

int main(void)
{
    static const struct {
        unsigned int a, b, operation, expected;
        const char *name;
    } tests[] = {
        { 9,  8, ALU_ADD,  1, "ADD" },
        { 3,  5, ALU_SUB, 14, "SUB" },
        {12, 10, ALU_AND,  8, "AND" },
        {12,  3, ALU_OR,  15, "OR"  },
        {12, 10, ALU_XOR,  6, "XOR" }
    };

    unsigned int failures = 0;

    for (unsigned int i = 0; i < sizeof(tests) / sizeof(tests[0]); i++) {
        unsigned int result =
            alu4(tests[i].a, tests[i].b, tests[i].operation);
        int passed = result == tests[i].expected;

        printf("%s: A=%u B=%u result=%u signed=%d %s\n",
               tests[i].name, tests[i].a, tests[i].b,
               result, signed4(result), passed ? "PASS" : "FAIL");

        if (!passed)
            failures++;
    }

    printf("%s\n", failures ? "ALU TEST FAILED" : "ALU TEST PASSED");
    return failures ? 1 : 0;
}