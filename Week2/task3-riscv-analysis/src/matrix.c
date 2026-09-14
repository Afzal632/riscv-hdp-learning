#include <stdio.h>

#define ROWS_A 2
#define COLS_A 3
#define COLS_B 2

__attribute__((noinline, noclone))
void matrix_multiply(const int a[ROWS_A][COLS_A],
                     const int b[COLS_A][COLS_B],
                     int result[ROWS_A][COLS_B])
{
    for (int i = 0; i < ROWS_A; i++) {
        for (int j = 0; j < COLS_B; j++) {
            int sum = 0;

            for (int k = 0; k < COLS_A; k++)
                sum += a[i][k] * b[k][j];

            result[i][j] = sum;
        }
    }
}

int main(void)
{
    const int a[ROWS_A][COLS_A] = {
        {1, 2, 3},
        {4, 5, 6}
    };

    const int b[COLS_A][COLS_B] = {
        {7,  8},
        {9, 10},
        {11, 12}
    };

    const int expected[ROWS_A][COLS_B] = {
        {58, 64},
        {139, 154}
    };

    int result[ROWS_A][COLS_B] = {0};
    int failures = 0;

    matrix_multiply(a, b, result);

    printf("Result matrix:\n");

    for (int i = 0; i < ROWS_A; i++) {
        for (int j = 0; j < COLS_B; j++) {
            printf("%d%c", result[i][j],
                   j == COLS_B - 1 ? '\n' : ' ');

            if (result[i][j] != expected[i][j])
                failures++;
        }
    }

    printf("%s\n",
           failures ? "MATRIX TEST FAILED" : "MATRIX TEST PASSED");

    return failures ? 1 : 0;
}
