# Counter Experiment Observations

The O0 and O2 executables both ran under Spike, exited with status 0, and printed the predicted sequence 0 through 15 followed by 0 and 1. Their visible outputs matched.

At O0, the compiler kept `count` and `sample` in stack memory. This produced repeated `lw` and `sw` instructions. The counter was incremented with `addi`, compared with 16 using `bne`, and explicitly reset by storing zero. The 18-iteration loop was controlled by `bgeu`.

At O2, `count`, the remaining iteration count, and the format-string address were retained in registers x8, x9, and x18. This reduced repeated memory access. The static instruction count fell from 31 to 26, while the static `lw` and `sw` occurrences each fell from 7 to 4.

O2 replaced the conditional counter-reset branch with a mask. For old counter values 0 through 14, `sltu` produces 1 and `sub x15,x0,x15` converts it to -1, represented as 0xFFFFFFFF. ANDing the incremented count with this mask preserves it. For an old count of 15, the mask becomes 0x00000000, so ANDing 16 with it produces 0.

The 31-to-26 reduction is approximately 16.1% in static instructions. It does not prove a 16.1% runtime improvement. Runtime also depends on dynamic instruction counts, instruction CPI, clock period, memory behaviour, and the cost of `printf`.

These observations demonstrate matching behaviour for the tested executions under the Spike ISA simulator. They do not verify a Verilog processor, prove exhaustive program correctness, or establish hardware performance.
