# Task 3 Predictions

These predictions were recorded before compilation and execution.

## Counter

The program should print 18 values:

0, 1, 2, ..., 15, 0, 1

The program should then terminate successfully.

At `-O0`, the assembly will probably contain more stack accesses and
explicit loads and stores than at `-O2`.

At `-O2`, the compiler may implement wrapping with `andi` because the
counter range is a power of two.

## Matrix multiplication

The result should be:

58 64
139 154

The final message should be `MATRIX TEST PASSED`.

When compiled for RV32IM, multiplication operations should produce one
or more `mul` instructions. At `-O2`, loop control and address
calculations should require fewer instructions than at `-O0`.

## ALU

All five tests should pass.

The subtraction test should produce the 4-bit pattern 1110. Its unsigned
value is 14 and its signed two's-complement interpretation is -2.

The disassembly of `alu4` should contain operations implementing
addition, subtraction, AND, OR and XOR.

## Evidence limits

Successful compilation will prove only that the source is accepted and
linked for the selected target.

Correct program output under Spike will provide functional evidence for
these particular test cases.

Disassembly and static instruction counts will not measure CPU time,
clock frequency, CPI, hardware area or real processor performance.

Spike is an ISA simulator and does not provide cycle-accurate
microarchitecture performance measurements.
