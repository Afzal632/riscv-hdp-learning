#include <stdio.h>

#define SAMPLE_COUNT 18

int main(void)
{
    unsigned int count = 0;

    for (unsigned int sample = 0; sample < SAMPLE_COUNT; sample++) {
        printf("Count value is: %u\n", count);

        count++;

        if (count == 16U)
            count = 0U;
    }

    return 0;
}