`timescale 1ns/1ps
`default_nettype none

module alu4_tb;

    reg  [3:0] a;
    reg  [3:0] b;
    reg  [2:0] operation;
    wire [3:0] result;
    wire       zero;

    integer errors;

    alu4 dut (
        .a(a),
        .b(b),
        .operation(operation),
        .result(result),
        .zero(zero)
    );

    task check;
        input [3:0] test_a;
        input [3:0] test_b;
        input [2:0] test_operation;
        input [3:0] expected_result;
        input       expected_zero;

        begin
            a         = test_a;
            b         = test_b;
            operation = test_operation;

            #10;

            if ((result !== expected_result) ||
                (zero !== expected_zero)) begin

                $display(
                    "FAIL: A=%0d B=%0d OP=%0d RESULT=%0d EXPECTED=%0d",
                    a, b, operation, result, expected_result
                );

                errors = errors + 1;
            end
            else begin
                $display(
                    "PASS: A=%0d B=%0d OP=%0d RESULT=%0d ZERO=%0b",
                    a, b, operation, result, zero
                );
            end
        end
    endtask

    initial begin
        $dumpfile("build/alu4.vcd");
        $dumpvars(0, alu4_tb);

        errors = 0;

        check(4'd5,  4'd3, 3'b000, 4'd8,  1'b0);
        check(4'd5,  4'd3, 3'b001, 4'd2,  1'b0);
        check(4'd3,  4'd5, 3'b001, 4'd14, 1'b0);
        check(4'd5,  4'd3, 3'b010, 4'd1,  1'b0);
        check(4'd5,  4'd3, 3'b011, 4'd7,  1'b0);
        check(4'd5,  4'd3, 3'b100, 4'd6,  1'b0);
        check(4'd15, 4'd1, 3'b000, 4'd0,  1'b1);

        if (errors == 0)
            $display("ALL VERILOG ALU TESTS PASSED");
        else
            $display("%0d VERILOG ALU TEST(S) FAILED", errors);

        $finish;
    end

endmodule

`default_nettype wire
