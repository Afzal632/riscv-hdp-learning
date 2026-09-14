# RISC-V Hardware Design Program

This repository documents my work from the **RISC-V Hardware Design Program by [VLSI System Design](https://www.vlsisystemdesign.com/)**.

It records what I learned each week and the experiments I completed using C, RISC-V assembly, Verilog, simulation and synthesis tools.

The referenced public course projects were used to understand the concepts and workflow. The enclosure security project, source code, testbenches, results and documentation in this repository were developed specifically for this repository.

## Course Organization

Some referenced repositories organize this material across six weeks. My learning and implementation were organized into four weeks. Topics such as inline assembly, GPIO configuration, testbenches, UART bypassing, synthesis and gate-level simulation were therefore completed during Week 4.

## Week 1 — Binary Arithmetic, ALU Design and ISA Comparison

Week 1 introduced binary representation and the connection between C operations, assembly instructions and digital hardware.

The work covered:

- Sign-and-magnitude, one's complement and two's complement
- Signed and unsigned interpretation of binary values
- Four-bit addition, subtraction, AND, OR and XOR
- Overflow caused by retaining only four result bits
- A four-bit ALU implemented in both C and Verilog
- Self-checking Verilog testbenches
- Waveform inspection with GTKWave
- Comparison of counter assembly across RISC-V, AVR, ARM and x86-64
- Changes in assembly when an up-counter becomes a down-counter

Documentation:

- [Binary Arithmetic and Four-Bit ALU](Week1/alu-task2/README.md)
- [Counter ISA Comparison](Week1/counter-isa-comparison/README.md)

## Week 2 — RV32IM Program and Instruction Analysis

Week 2 examined how GCC translates C programs into RV32IM instructions.

A counter, four-bit ALU and matrix-multiplication program were compiled at `-O0` and `-O2`.

The work covered:

- RISC-V ABI registers
- Function arguments and return values
- Stack frames and local variables
- Load and store instructions
- Branches and function calls
- Address calculations
- Instruction-frequency analysis
- Static versus dynamic instructions
- Compiler optimization and register allocation
- Loop unrolling
- Executing RV32IM programs using Spike

Documentation:

- [Week 2 Overview](Week2/README.md)
- [RV32IM Instruction Analysis](Week2/task3-riscv-analysis/README.md)

## Week 3 — Application-Specific RISC-V GPIO Architecture

Week 3 connected application software, RISC-V registers, Verilog modules and external GPIO signals.

The work covered:

- The 32 RV32I integer registers
- The special behaviour of `x0`
- Custom use of register `x30` as a GPIO interface
- GPIO input and output ownership
- Reading bits using shifts and masks
- Safe output updates using read-modify-write operations
- `digitalRead` and `digitalWrite`
- Polling and interrupts
- Static and dynamic instruction execution
- Application-specific instruction selection
- Verilog wrappers and top-level modules
- FPGA LUTs, ASIC logic and physical GPIO cost

Week 3 was used to understand the architecture and plan the project implemented during Week 4.

Documentation:

- [Week 3 GPIO Architecture](Week3/README.md)

## Week 4 — C, Assembly, GPIO RTL and Synthesis

Week 4 followed the design from application behaviour to a synthesized gate-level netlist.

The work covered:

- C variables, data memory and stack allocation
- Compiler-generated RV32I assembly
- Initialization using `x0`
- Function calls and control flow
- Inline assembly notation
- Reading and writing custom `x30` GPIO fields
- GPIO masking
- Program memory, bootloading and UART bypass concepts
- Verilog GPIO configuration
- RTL simulation with Icarus Verilog
- Waveform inspection using GTKWave
- Logic synthesis using Yosys
- Generic cells and foundry-library concepts
- Gate-level simulation

The optimized controller decreased from 165 static instructions at `-O0` to 61 at `-O2`. Its code size decreased from 660 bytes to 244 bytes.

The GPIO bridge passed all 11 RTL checks. Yosys synthesized it into four enabled flip-flops, and the gate-level netlist passed the same 11 checks.

Documentation:

- [Week 4 Experiments](Week4/README.md)

## Project Built in This Repository

The project developed during Weeks 3 and 4 is a **RISC-V-Based Latched Enclosure Tamper and SOS Controller**.

It monitors a generic protected enclosure using GPIO inputs.

Its behaviour includes:

- Opening the enclosure while disarmed does not trigger the alarm.
- Opening it while armed activates the alert.
- A tamper event activates the alert even while disarmed.
- The alert remains latched after the trigger disappears.
- Reset is accepted only after the unsafe sensor condition is removed.
- The controller drives an alarm, alert LED and SOS-request output.

### Custom x30 GPIO Allocation

| Bits | Direction | Purpose |
|---|---|---|
| `31` | Input | System armed |
| `30` | Input | Enclosure open |
| `29` | Input | Tamper detected |
| `28` | Input | Alarm reset |
| `27:4` | Reserved | Future expansion |
| `3` | Output | SOS request |
| `2` | Output | Local alarm |
| `1` | Output | Alert LED |
| `0` | Output | Armed LED |

The project includes:

- A tested C controller
- RV32I `-O0` and `-O2` disassembly
- Custom `x30` inline assembly
- A synthesizable Verilog GPIO bridge
- A self-checking SystemVerilog testbench
- RTL waveforms
- A Yosys-generated netlist
- Gate-level simulation and comparison results

## Repository Structure

```text
riscv-hdp/
├── Week1/    Binary arithmetic, ALU and ISA comparison
├── Week2/    RV32IM program and instruction analysis
├── Week3/    Application-specific GPIO architecture
└── Week4/    Security controller, RTL and synthesis
```

## Tools Used

- GCC and the RISC-V GNU toolchain
- GNU `objdump`
- Spike
- Icarus Verilog and `vvp`
- GTKWave
- Yosys
- Git and GitHub

## Acknowledgement

This work was completed while following the **RISC-V Hardware Design Program by VLSI System Design**. The course lectures and referenced projects provided the learning context, while the project implementation and verification in this repository were created as my own practical application of those concepts.