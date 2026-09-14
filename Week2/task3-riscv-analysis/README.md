# Task 3: RV32IM Program and Instruction Analysis

## Objective

This task examines how three C programs are translated into RV32IM instructions at GCC optimization levels O0 and O2:

- A finite modulo-16 counter
- A deterministic four-bit ALU
- A 2x3 by 3x2 matrix multiplication

The investigation compares static instructions, instruction mixes, stack-memory access, register use, branches, address calculations and compiler transformations.

Predictions were recorded before execution in [`results/predictions.md`](results/predictions.md).

## Evidence boundary

Object compilation establishes that GCC accepted the source and produced RV32 object files.

Disassembly establishes which static instructions were emitted.

Spike execution establishes the observed ISA-level behaviour of the linked binaries for the selected tests.

These results do not verify a Verilog processor and do not establish hardware performance, CPI, clock frequency, timing, area, power or physical implementation quality.

## Toolchain

The experiment used:

- RISC-V bare-metal GCC 13.2.0
- GNU objdump 2.42
- RV32IM with the ILP32 ABI
- Picolibc
- Spike RISC-V ISA Simulator 1.1.1-dev
- RV32 RISC-V Proxy Kernel
- Icarus Verilog 12.0
- Yosys 0.33

## Directory structure

    task3-riscv-analysis/
    ├── README.md
    ├── src/
    │   ├── counter.c
    │   ├── alu.c
    │   ├── matrix.c
    │   └── runtime/
    │       ├── pk_start.S
    │       └── pk_stdio.c
    ├── build/
    │   ├── O0/
    │   ├── O2/
    │   └── runtime/
    ├── results/
    │   ├── predictions.md
    │   ├── counter-observations.md
    │   ├── matrix-observations.md
    │   ├── alu-observations.md
    │   ├── final-comparison.csv
    │   ├── final-comparison.md
    │   ├── disassembly/
    │   ├── instruction-mix/
    │   └── spike-runs/
    └── screenshots/

## Compilation method

Each program was compiled into RV32IM relocatable objects at O0 and O2 using the equivalent of:

    riscv64-unknown-elf-gcc \
      --specs=picolibc.specs \
      -march=rv32im \
      -mabi=ilp32 \
      -std=c11 \
      -Wall -Wextra -Wpedantic \
      -O0-or-O2 \
      -g \
      -c program.c \
      -o program.o

The selected functions were disassembled with numeric register names and pseudoinstruction aliases disabled:

    riscv64-unknown-elf-objdump \
      -dr \
      -M no-aliases,numeric \
      --disassemble=function_name \
      program.o

The selected functions were:

- `main` for the counter
- `alu4` for the ALU
- `matrix_multiply` for matrix multiplication

Static instruction count means the number of instruction occurrences present in the selected disassembly. It is not the number executed at runtime.

## Spike runtime support

The Ubuntu RISC-V compiler uses Picolibc rather than the Newlib environment normally expected by the RISC-V Proxy Kernel.

Two runtime-support files were added:

- `pk_start.S` establishes the application entry point and exits through an `ecall`.
- `pk_stdio.c` defines Picolibc console streams and converts output characters into Proxy Kernel write requests.

Applications were executed using:

    spike \
      --isa=rv32im_zicsr_zifencei \
      -m128 \
      /usr/local/bin/pk32 \
      program.elf

The runtime adapter provides a software execution environment. It is not a Verilog processor or hardware ALU.

## Final comparison

| Program | O0 static | O2 static | Reduction | O0 `lw`/`sw` | O2 `lw`/`sw` | Runtime |
|---|---:|---:|---:|---:|---:|---|
| Counter | 31 | 26 | 16.1% | 7/7 | 4/4 | Matched; status 0 |
| Four-bit ALU | 49 | 25 | 49.0% | 15/10 | 1/0 | Matched; status 0 |
| Matrix multiplication | 69 | 29 | 58.0% | 20/13 | 12/2 | Matched; status 0 |

Detailed results are available in [`results/final-comparison.md`](results/final-comparison.md).

## Counter observations

At O0, `count` and `sample` were stored on the stack. Repeated `lw` and `sw` instructions accessed them. A conditional branch checked for 16 and explicitly reset the count.

At O2, the compiler retained state in registers and replaced the conditional reset with a mask generated using `sltu` and `sub`.

The mask was either:

- `0xFFFFFFFF`, which preserved the incremented value with `and`
- `0x00000000`, which changed the incremented value to zero

Detailed observations are in [`results/counter-observations.md`](results/counter-observations.md).

## ALU observations

The `alu4` function receives A, B and the operation selector in ABI registers `a0`, `a1` and `a2`, corresponding to `x10`, `x11` and `x12`.

Both versions used a jump table for the C switch. `slli` multiplied the operation index by four to address a four-byte table entry. This was address generation, not one of the five requested ALU operations.

At O2, operands remained in registers. Each case performed its operation directly in `a0`, applied `andi 15`, and returned.

Detailed observations are in [`results/alu-observations.md`](results/alu-observations.md).

## Four-bit signed and unsigned values

The ALU retains only the lowest four bits using `result & 0xF`.

The same bits can represent different signed and unsigned values:

- `1110` is unsigned 14 or signed two's-complement -2
- `1000` is unsigned 8 or signed two's-complement -8
- `1111` is unsigned 15 or signed two's-complement -1

For example, the low four bits of `3 - 5` are `1110`.

## Matrix observations

At O0, GCC closely retained the three nested loops. Shifts and additions calculated array addresses.

Because each integer occupies four bytes, `slli index,index,2` converts an integer index into a byte offset.

O0 contained one static `mul` reused by the loops. It dynamically executes:

    2 rows × 2 columns × 3 products = 12 multiplications

At O2, GCC unrolled the two columns and three terms of each dot product. Six static `mul` instructions remained inside a two-iteration row loop, still producing 12 dynamic multiplications.

Detailed observations are in [`results/matrix-observations.md`](results/matrix-observations.md).

## Static versus dynamic instructions

A static instruction is one instruction occurrence in executable code.

A dynamic instruction is one execution of an instruction while the program runs. One static instruction inside a loop can execute many times.

Therefore:

- Static instruction count is not runtime instruction count.
- Fewer static instructions do not prove proportional speedup.
- Loop unrolling can increase static `mul` occurrences without increasing the mathematical multiplication count.

## Screenshot evidence

### Toolchain

- [T3-01 Toolchain audit](screenshots/T3-01-toolchain-audit.png)

### Counter

- [T3-02 Counter source](screenshots/T3-02-counter-source.png)
- [T3-03 Counter Spike output](screenshots/T3-03-counter-spike-output.png)
- [T3-04 Counter O0 disassembly](screenshots/T3-04-counter-O0-disassembly.png)
- [T3-05 Counter O2 disassembly](screenshots/T3-05-counter-O2-disassembly.png)
- [T3-06 Counter analysis](screenshots/T3-06-counter-analysis.png)

### Matrix multiplication

- [T3-07 Matrix source](screenshots/T3-07-matrix-source.png)
- [T3-08 Matrix Spike output](screenshots/T3-08-matrix-spike-output.png)
- [T3-09 Matrix O0/O2 disassembly](screenshots/T3-09-matrix-O0-O2-disassembly.png)
- [T3-10 Matrix analysis](screenshots/T3-10-matrix-analysis.png)

### Four-bit ALU

- [T3-11 ALU source](screenshots/T3-11-alu-source.png)
- [T3-12 ALU Spike output](screenshots/T3-12-alu-spike-output.png)
- [T3-13 ALU O0/O2 disassembly](screenshots/T3-13-alu-O0-O2-disassembly.png)
- [T3-14 ALU analysis](screenshots/T3-14-alu-analysis.png)

### Final comparison

- [T3-15 Final comparison](screenshots/T3-15-final-comparison.png)

## Conclusion

Optimization changed how the same C behaviour was expressed as RISC-V instructions.

O2 generally retained values in registers, reduced stack traffic, simplified control flow and used fixed offsets or loop unrolling. Optimized assembly can therefore look substantially different from the original C structure while preserving the tested behaviour.

This experiment connects software to the ISA: C was translated into RV32IM instructions, and Spike executed those instructions according to RISC-V architectural rules. It did not implement or verify the microarchitecture that would execute those instructions in hardware.
