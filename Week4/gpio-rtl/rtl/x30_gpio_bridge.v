`timescale 1ns/1ps
module x30_gpio_bridge (
    input  wire        clk,
    input  wire        reset_n,

    input  wire        x30_write_enable,
    input  wire [31:0] x30_write_data,

    input  wire        system_armed,
    input  wire        enclosure_open,
    input  wire        tamper_detected,
    input  wire        alarm_reset,

    output wire [31:0] x30_read_data,

    output wire        sos_request,
    output wire        local_alarm,
    output wire        alert_led,
    output wire        armed_led
);

    reg [3:0] output_register;

    /*
     * Software owns x30[3:0]. Only these four bits are stored when
     * the processor writes x30.
     */
    always @(posedge clk or negedge reset_n) begin
        if (!reset_n)
            output_register <= 4'b0000;
        else if (x30_write_enable)
            output_register <= x30_write_data[3:0];
    end

    /*
     * External GPIO pins own x30[31:28].
     * Reserved bits x30[27:4] read as zero.
     * Stored output bits are readable through x30[3:0].
     */
    assign x30_read_data = {
        system_armed,
        enclosure_open,
        tamper_detected,
        alarm_reset,
        24'b0,
        output_register
    };

    assign sos_request = output_register[3];
    assign local_alarm = output_register[2];
    assign alert_led   = output_register[1];
    assign armed_led   = output_register[0];

endmodule