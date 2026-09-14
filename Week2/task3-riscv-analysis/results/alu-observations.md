# Four-Bit ALU Observations

The O0 and O2 executables both exited with status 0. All five test cases passed, and the visible O0 and O2 outputs matched.

The function receives input A in x10/a0, input B in x11/a1, and the operation selector in x12/a2. Its result is returned in x10/a0. A bltu instruction sends operation values greater than four to the default case.

Both versions implement the switch using a jump table. The operation selector is shifted left by two with slli because each table entry occupies four bytes. The selected target address is loaded with lw and reached using jalr. This slli performs jump-table address calculation; it is not an ALU test operation.

At O0, the arguments and result were repeatedly stored in stack memory. Each case performed add, sub, and, or, or xor and then used jal to reach one shared result-masking path. The function contained 15 static lw instructions and 10 static sw instructions.

At O2, the operands remained in registers. Each operation wrote directly into x10, applied andi with 15, and returned. This duplicated the andi and return instructions in each case, explaining why static jalr occurrences increased even though the total function became smaller.

The static instruction count fell from 49 at O0 to 25 at O2, approximately 49.0%. Static lw occurrences fell from 15 to 1, and static sw occurrences fell from 10 to 0.

Masking with 0xF retains only the lowest four bits. Thus 9 + 8 produces 17 before masking and 1 after masking. The four-bit subtraction 3 - 5 produces 1110, interpreted as unsigned 14 or signed two's-complement -2. Similarly, 1000 represents unsigned 8 or signed -8, and 1111 represents unsigned 15 or signed -1.

These observations establish matching behaviour for the selected tests under Spike. They do not exhaustively verify every input combination, verify Verilog hardware, or establish clock frequency, CPI, area, power, timing, or performance.
