# Week 2 — RV32IM Program and Instruction Analysis

## Overview

Week 2 investigated how C programs are translated into RV32IM instructions and how compiler optimization changes the resulting assembly.

Three programs were studied:

- A finite modulo-16 counter
- A deterministic four-bit ALU
- A 2×3 by 3×2 matrix multiplication

Each program was compiled at `-O0` and `-O2`. The generated object files, disassembly, instruction mixes and Spike results were compared.

## Work Completed

The complete experiment is documented in:

- [Task 3: RV32IM Program and Instruction Analysis](task3-riscv-analysis/README.md)

Supporting environment information is recorded in:

- [Tool audit](tool-audit.txt)
- [Compiler audit](compiler-audit.txt)
- [Source audit](source-audit.txt)

## Main Understanding

The experiment established the distinction between C source code, ISA-level instructions and hardware implementation.

Important observations included:

- Optimized assembly may look very different from its original C source while preserving tested behaviour.
- Local variables frequently use stack memory at `-O0`, while optimization can retain more values in registers.
- Static instruction count is different from dynamic instruction count.
- A single instruction inside a loop can execute many times.
- Address-calculation instructions are not necessarily part of the arithmetic operation being studied.
- Function arguments and return values follow the RISC-V ABI register convention.
- Successful Spike execution verifies ISA-level program behaviour, not Verilog processor behaviour.

## Evidence Boundary

Compilation proves that GCC accepted the source and generated RV32IM object code.

Disassembly identifies the static instructions generated for the selected functions.

Spike execution demonstrates the observed ISA-level behaviour for the selected inputs.

The experiment does not verify an RTL processor and does not establish CPI, clock frequency, timing, area, power or physical implementation quality.
