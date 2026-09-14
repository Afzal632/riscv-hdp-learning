#include <assert.h>
#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>

#include "controller.h"

static const char *state_name(controller_state_t state)
{
    switch (state) {
    case CONTROLLER_DISARMED:
        return "DISARMED";

    case CONTROLLER_ARMED:
        return "ARMED";

    case CONTROLLER_ALERT:
        return "ALERT";

    default:
        return "INVALID";
    }
}

static uint32_t make_inputs(
    uint32_t armed,
    uint32_t enclosure_open,
    uint32_t tamper,
    uint32_t alarm_reset)
{
    uint32_t gpio_value = 0u;

    gpio_value |= (armed & 1u) << GPIO_ARMED_BIT;
    gpio_value |=
        (enclosure_open & 1u) << GPIO_ENCLOSURE_OPEN_BIT;
    gpio_value |= (tamper & 1u) << GPIO_TAMPER_BIT;
    gpio_value |=
        (alarm_reset & 1u) << GPIO_ALARM_RESET_BIT;

    return gpio_value;
}

static void run_case(
    const char *name,
    uint32_t input_gpio,
    controller_state_t *state,
    controller_state_t expected_state,
    uint32_t expected_outputs)
{
    uint32_t result;
    uint32_t actual_outputs;

    result = controller_step(input_gpio, state);
    actual_outputs = result & GPIO_OUTPUT_MASK;

    printf(
        "%-31s input=0x%08" PRIx32
        " output=0x%01" PRIx32
        " state=%s\n",
        name,
        input_gpio,
        actual_outputs,
        state_name(*state)
    );

    assert(*state == expected_state);
    assert(actual_outputs == expected_outputs);

    /*
     * Controller output writes must preserve all externally supplied
     * input bits.
     */
    assert(
        (result & GPIO_INPUT_MASK) ==
        (input_gpio & GPIO_INPUT_MASK)
    );
}

int main(void)
{
    controller_state_t state = CONTROLLER_DISARMED;

    const uint32_t disarmed_outputs = 0u;

    const uint32_t armed_outputs =
        GPIO_BIT(GPIO_ARMED_LED_BIT);

    const uint32_t alert_outputs =
        GPIO_BIT(GPIO_SOS_REQUEST_BIT) |
        GPIO_BIT(GPIO_ALARM_BIT) |
        GPIO_BIT(GPIO_ALERT_LED_BIT);

    run_case(
        "Startup",
        make_inputs(0u, 0u, 0u, 0u),
        &state,
        CONTROLLER_DISARMED,
        disarmed_outputs
    );

    run_case(
        "Open while disarmed",
        make_inputs(0u, 1u, 0u, 0u),
        &state,
        CONTROLLER_DISARMED,
        disarmed_outputs
    );

    run_case(
        "Arm with enclosure closed",
        make_inputs(1u, 0u, 0u, 0u),
        &state,
        CONTROLLER_ARMED,
        armed_outputs
    );

    run_case(
        "Open while armed",
        make_inputs(1u, 1u, 0u, 0u),
        &state,
        CONTROLLER_ALERT,
        alert_outputs
    );

    run_case(
        "Remove trigger without reset",
        make_inputs(0u, 0u, 0u, 0u),
        &state,
        CONTROLLER_ALERT,
        alert_outputs
    );

    run_case(
        "Reset while enclosure open",
        make_inputs(0u, 1u, 0u, 1u),
        &state,
        CONTROLLER_ALERT,
        alert_outputs
    );

    run_case(
        "Safe alarm reset",
        make_inputs(0u, 0u, 0u, 1u),
        &state,
        CONTROLLER_DISARMED,
        disarmed_outputs
    );

    run_case(
        "Tamper while disarmed",
        make_inputs(0u, 0u, 1u, 0u),
        &state,
        CONTROLLER_ALERT,
        alert_outputs
    );

    run_case(
        "Reset while tamper active",
        make_inputs(0u, 0u, 1u, 1u),
        &state,
        CONTROLLER_ALERT,
        alert_outputs
    );

    run_case(
        "Reset after tamper removed",
        make_inputs(0u, 0u, 0u, 1u),
        &state,
        CONTROLLER_DISARMED,
        disarmed_outputs
    );

    puts("All controller tests passed.");

    return 0;
}