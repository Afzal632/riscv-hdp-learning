#ifndef X30_GPIO_H
#define X30_GPIO_H

#include <stdint.h>

#include "controller.h"

uint32_t x30_read_snapshot(void);

uint32_t x30_digital_read(unsigned int bit_position);

void x30_write_outputs(uint32_t output_bits);

void controller_step_x30(controller_state_t *state);

#endif