#include <stdio.h>
#include <time.h>

void delay(int n)
{
    int us = n;
    clock_t start_time = clock();

    while (clock() <
           start_time + (us * CLOCKS_PER_SEC / 1000000));
}

void display(int count)
{
    printf("Count value is: %d\n", count);
}

int main(void)
{
    int count = 15;

    while (1)
    {
        display(count);
        count--;

        if (count == -1)
        {
            count = 15;
        }

        delay(500000);
    }
}
