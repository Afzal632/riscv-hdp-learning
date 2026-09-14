#include "controller.h"

uint32_t digital_read(uint32_t gpio_value, unsigned int bit_position)
{
    if (bit_position >= 32u) {
        return 0u;
    }

    return (gpio_value >> bit_position) & 1u;
}

uint32_t digital_write(
    uint32_t gpio_value,
    unsigned int bit_position,
    uint32_t new_value)
{
    uint32_t bit_mask;

    if (bit_position >= 32u) {
        return gpio_value;
    }

    bit_mask = GPIO_BIT(bit_position);

    gpio_value &= ~bit_mask;
    gpio_value |= (new_value & 1u) << bit_position;

    return gpio_value;
}

static uint32_t update_outputs(
    uint32_t gpio_value,
    controller_state_t state)
{
    uint32_t armed_active;
    uint32_t alert_active;

    armed_active = (state == CONTROLLER_ARMED);
    alert_active = (state == CONTROLLER_ALERT);

    gpio_value = digital_write(
        gpio_value,
        GPIO_ARMED_LED_BIT,
        armed_active
    );

    gpio_value = digital_write(
        gpio_value,
        GPIO_ALERT_LED_BIT,
        alert_active
    );

    gpio_value = digital_write(
        gpio_value,
        GPIO_ALARM_BIT,
        alert_active
    );

    gpio_value = digital_write(
        gpio_value,
        GPIO_SOS_REQUEST_BIT,
        alert_active
    );

    return gpio_value;
}

uint32_t controller_step(
    uint32_t gpio_value,
    controller_state_t *state)
{
    uint32_t armed;
    uint32_t enclosure_open;
    uint32_t tamper_detected;
    uint32_t alarm_reset;

    armed = digital_read(gpio_value, GPIO_ARMED_BIT);
    enclosure_open = digital_read(
        gpio_value,
        GPIO_ENCLOSURE_OPEN_BIT
    );
    tamper_detected = digital_read(
        gpio_value,
        GPIO_TAMPER_BIT
    );
    alarm_reset = digital_read(
        gpio_value,
        GPIO_ALARM_RESET_BIT
    );

    /*
     * Tamper has the highest priority and causes an alert even when
     * the normal armed input is inactive.
     */
    if (tamper_detected != 0u) {
        *state = CONTROLLER_ALERT;
    } else if (*state == CONTROLLER_ALERT) {
        /*
         * The alert remains latched until reset is requested while
         * the enclosure is closed and tamper is inactive.
         */
        if ((alarm_reset != 0u) && (enclosure_open == 0u)) {
            *state = CONTROLLER_DISARMED;
        }
    } else if (armed == 0u) {
        *state = CONTROLLER_DISARMED;
    } else if (enclosure_open != 0u) {
        *state = CONTROLLER_ALERT;
    } else {
        *state = CONTROLLER_ARMED;
    }

    return update_outputs(gpio_value, *state);
}