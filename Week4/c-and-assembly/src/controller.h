#ifndef ENCLOSURE_CONTROLLER_H
#define ENCLOSURE_CONTROLLER_H

#include <stdint.h>

typedef enum {
    CONTROLLER_DISARMED = 0,
    CONTROLLER_ARMED    = 1,
    CONTROLLER_ALERT    = 2
} controller_state_t;

/* Input fields in the custom x30 GPIO view. */
#define GPIO_ARMED_BIT          31u
#define GPIO_ENCLOSURE_OPEN_BIT 30u
#define GPIO_TAMPER_BIT         29u
#define GPIO_ALARM_RESET_BIT    28u

/* Processor-controlled output fields. */
#define GPIO_SOS_REQUEST_BIT     3u
#define GPIO_ALARM_BIT           2u
#define GPIO_ALERT_LED_BIT       1u
#define GPIO_ARMED_LED_BIT       0u

#define GPIO_BIT(bit) (UINT32_C(1) << (bit))

#define GPIO_INPUT_MASK ( \
    GPIO_BIT(GPIO_ARMED_BIT)          | \
    GPIO_BIT(GPIO_ENCLOSURE_OPEN_BIT) | \
    GPIO_BIT(GPIO_TAMPER_BIT)         | \
    GPIO_BIT(GPIO_ALARM_RESET_BIT))

#define GPIO_OUTPUT_MASK ( \
    GPIO_BIT(GPIO_SOS_REQUEST_BIT) | \
    GPIO_BIT(GPIO_ALARM_BIT)       | \
    GPIO_BIT(GPIO_ALERT_LED_BIT)   | \
    GPIO_BIT(GPIO_ARMED_LED_BIT))

uint32_t digital_read(uint32_t gpio_value, unsigned int bit_position);

uint32_t digital_write(
    uint32_t gpio_value,
    unsigned int bit_position,
    uint32_t new_value
);

uint32_t controller_step(
    uint32_t gpio_value,
    controller_state_t *state
);

#endif