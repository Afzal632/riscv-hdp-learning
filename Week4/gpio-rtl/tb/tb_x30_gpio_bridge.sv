`timescale 1ns/1ps

module tb_x30_gpio_bridge;

    logic        clk;
    logic        reset_n;
    logic        x30_write_enable;
    logic [31:0] x30_write_data;

    logic system_armed;
    logic enclosure_open;
    logic tamper_detected;
    logic alarm_reset;

    wire [31:0] x30_read_data;
    wire        sos_request;
    wire        local_alarm;
    wire        alert_led;
    wire        armed_led;

    integer check_count;

    x30_gpio_bridge dut (
        .clk              (clk),
        .reset_n          (reset_n),
        .x30_write_enable (x30_write_enable),
        .x30_write_data   (x30_write_data),
        .system_armed     (system_armed),
        .enclosure_open   (enclosure_open),
        .tamper_detected  (tamper_detected),
        .alarm_reset      (alarm_reset),
        .x30_read_data    (x30_read_data),
        .sos_request      (sos_request),
        .local_alarm      (local_alarm),
        .alert_led        (alert_led),
        .armed_led        (armed_led)
    );

    always #5 clk = ~clk;

    task automatic write_outputs(input logic [31:0] value);
        begin
            @(negedge clk);
            x30_write_data   = value;
            x30_write_enable = 1'b1;

            @(negedge clk);
            x30_write_enable = 1'b0;
            x30_write_data   = 32'b0;
        end
    endtask

    task automatic check_state(
        input logic [31:0] expected_read,
        input logic [3:0]  expected_outputs,
        input string       label
    );
        begin
            #1;
            check_count = check_count + 1;

            if (x30_read_data !== expected_read) begin
                $display(
                    "FAIL %-32s read=0x%08h expected=0x%08h",
                    label,
                    x30_read_data,
                    expected_read
                );
                $fatal(1);
            end

            if ({sos_request, local_alarm, alert_led, armed_led}
                !== expected_outputs) begin
                $display(
                    "FAIL %-32s outputs=0x%1h expected=0x%1h",
                    label,
                    {sos_request, local_alarm, alert_led, armed_led},
                    expected_outputs
                );
                $fatal(1);
            end

            $display(
                "PASS %-32s x30=0x%08h outputs=0x%1h",
                label,
                x30_read_data,
                {sos_request, local_alarm, alert_led, armed_led}
            );
        end
    endtask

    initial begin
        `ifdef GATE_LEVEL
        $dumpfile("Week4/synthesis/reports/x30-gpio-gate-level.vcd");
             `else
        $dumpfile("Week4/gpio-rtl/waves/x30-gpio.vcd");
        `endif
             $dumpvars(0, tb_x30_gpio_bridge);

        clk              = 1'b0;
        reset_n          = 1'b1;
        x30_write_enable = 1'b0;
        x30_write_data   = 32'b0;

        system_armed   = 1'b0;
        enclosure_open = 1'b0;
        tamper_detected = 1'b0;
        alarm_reset     = 1'b0;

        check_count = 0;

        #1 reset_n = 1'b0;
        repeat (2) @(negedge clk);
        reset_n = 1'b1;

        check_state(32'h0000_0000, 4'h0, "Reset");

        /*
         * Even a 32-bit all-ones write may only affect the
         * processor-owned output field x30[3:0].
         */
        write_outputs(32'hffff_ffff);
        check_state(32'h0000_000f, 4'hf, "Only low output field stored");

        write_outputs(32'h0000_0000);

        enclosure_open = 1'b1;
        check_state(32'h4000_0000, 4'h0, "Open while disarmed");

        enclosure_open = 1'b0;
        system_armed   = 1'b1;
        write_outputs(32'h0000_0001);
        check_state(32'h8000_0001, 4'h1, "Armed with enclosure closed");

        enclosure_open = 1'b1;
        write_outputs(32'h0000_000e);
        check_state(32'hc000_000e, 4'he, "Open while armed");

        system_armed   = 1'b0;
        enclosure_open = 1'b0;
        check_state(32'h0000_000e, 4'he, "Alert outputs retained");

        enclosure_open = 1'b1;
        alarm_reset    = 1'b1;
        check_state(32'h5000_000e, 4'he, "Unsafe reset retains alert");

        enclosure_open = 1'b0;
        write_outputs(32'h0000_0000);
        check_state(32'h1000_0000, 4'h0, "Safe reset clears outputs");

        alarm_reset     = 1'b0;
        tamper_detected = 1'b1;
        write_outputs(32'h0000_000e);
        check_state(32'h2000_000e, 4'he, "Tamper activates outputs");

        alarm_reset = 1'b1;
        check_state(32'h3000_000e, 4'he, "Reset blocked by tamper");

        tamper_detected = 1'b0;
        write_outputs(32'h0000_0000);
        check_state(32'h1000_0000, 4'h0, "Safe reset after tamper");

        $display("All %0d GPIO bridge checks passed.", check_count);
        $finish;
    end

endmodule