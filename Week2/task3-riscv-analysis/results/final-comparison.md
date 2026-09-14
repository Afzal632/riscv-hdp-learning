# Task 3 Final Comparison

All six RV32IM executables ran under Spike and exited with status 0. For every program, the O0 and O2 visible outputs matched.

| Program | O0 static | O2 static | Reduction | O0 lw/sw | O2 lw/sw |
|---|---:|---:|---:|---:|---:|
| Counter | 31 | 26 | 16.1% | 7/7 | 4/4 |
| Four-bit ALU | 49 | 25 | 49.0% | 15/10 | 1/0 |
| Matrix multiplication | 69 | 29 | 58.0% | 20/13 | 12/2 |

The counter changed from stack-based variables and a conditional reset branch to register-held state and a mask-based reset using sltu, sub, and and.

The ALU retained jump-table dispatch but kept operands and results in ABI registers at O2. Each case performed its operation, applied andi 15, and returned directly.

The matrix function changed most strongly. O0 retained three loops and reused one static mul instruction. O2 unrolled each three-term dot product and the two columns, leaving one row loop. Both versions still perform 12 dynamic matrix multiplications for this input shape.

The reductions above describe static instructions inside the selected functions. They are not CPU-time or speedup measurements. Spike provides ISA-level functional execution, not a cycle-accurate hardware model. No Verilog implementation, hardware CPI, clock period, timing, area, power, or cache behaviour was measured.
