#include "x30_gpio.h"

uint32_t x30_read_snapshot(void)
{
    uint32_t gpio_value;

    __asm__ volatile(
        "addi %0, x30, 0\n\t"
        : "=r"(gpio_value)
        :
        : "memory"
    );

    return gpio_value;
}

uint32_t x30_digital_read(unsigned int bit_position)
{
    uint32_t gpio_value;

    gpio_value = x30_read_snapshot();

    return digital_read(gpio_value, bit_position);
}

void x30_write_outputs(uint32_t output_bits)
{
    const uint32_t clear_output_mask = ~GPIO_OUTPUT_MASK;

    /*
     * Only x30[3:0] are changed. Input and reserved fields are
     * preserved by clearing and replacing only the output field.
     */
    output_bits &= GPIO_OUTPUT_MASK;

    __asm__ volatile(
        "and x30, x30, %0\n\t"
        "or  x30, x30, %1\n\t"
        :
        : "r"(clear_output_mask),
          "r"(output_bits)
        : "x30", "memory"
    );
}

void controller_step_x30(controller_state_t *state)
{
    uint32_t gpio_snapshot;
    uint32_t updated_gpio;

    gpio_snapshot = x30_read_snapshot();
    updated_gpio = controller_step(gpio_snapshot, state);

    x30_write_outputs(updated_gpio);
}