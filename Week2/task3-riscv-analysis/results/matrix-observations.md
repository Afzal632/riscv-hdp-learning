# Matrix Multiplication Observations

Both O0 and O2 executions exited with status 0 and produced the expected matrix: 58, 64, 139, and 154. Their visible outputs matched.

At O0, matrix_multiply closely retained the three nested loops. Loop variables, pointers, and the running sum were repeatedly accessed through memory, producing 20 static lw instructions and 13 static sw instructions. Seven slli instructions and several add instructions calculated array addresses. Shifting an index left by two multiplies it by four, which converts an integer index into a byte offset.

O0 contained one static mul instruction. That instruction was reused by the i, j, and k loops. From the loop bounds, the multiplication executes 2 × 2 × 3 = 12 times.

At O2, the compiler used fixed load offsets and pointer increments. It unrolled the two output columns and the three terms in each dot product, leaving one loop over the two rows. Each row-loop iteration contains six static mul instructions, so the six instructions execute twice and still produce 12 dynamic multiplications.

The static instruction count fell from 69 at O0 to 29 at O2, a reduction of approximately 58.0%. Static lw occurrences fell from 20 to 12, static sw occurrences fell from 13 to 2, and the three loop branches were reduced to one.

These results show functional agreement for the tested matrices under Spike. Static instruction counts alone do not establish execution time. No Verilog processor, clock period, hardware CPI, cache, physical timing, power, or area was measured.
