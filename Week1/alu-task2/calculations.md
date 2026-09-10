# Binary Arithmetic Calculations

## Representation

Eight-bit values are used for these calculations because the given decimal values cannot all be represented using four bits.

The word “signed” in the assignment is interpreted as sign-and-magnitude because the accompanying course material compares sign-and-magnitude, one’s complement, and two’s complement.

## 1. Calculate 10 − 19

The expected decimal result is:

```text
10 − 19 = −9
```

Positive operands:

```text
10 = 00001010
19 = 00010011
 9 = 00001001
```

### Sign-and-magnitude

Since 19 is greater than 10, the result is negative with magnitude 9.

```text
Magnitude of 9: 0001001
Sign bit:        1
Result:          10001001
```

Therefore:

```text
10001001 = −9 in eight-bit sign-and-magnitude
```

### One’s complement

First form negative 19 by inverting every bit of positive 19:

```text
+19:             00010011
Invert:          11101100
−19:             11101100
```

Add positive 10:

```text
  00001010
+ 11101100
----------
  11110110
```

Check the negative result by inverting it:

```text
11110110 → 00001001 = 9
```

Therefore:

```text
11110110 = −9 in eight-bit one’s complement
```

### Two’s complement

Form negative 19 by inverting positive 19 and adding one:

```text
+19:             00010011
One’s complement:11101100
Add one:         00000001
                  --------
−19:             11101101
```

Add positive 10:

```text
  00001010
+ 11101101
----------
  11110111
```

Check the magnitude:

```text
11110111 → invert → 00001000 → add 1 → 00001001
```

Therefore:

```text
11110111 = −9 in eight-bit two’s complement
```

## 2. Calculate 20 + 30

The expected decimal result is:

```text
20 + 30 = 50
```

Binary values:

```text
20 = 00010100
30 = 00011110
```

Addition:

```text
  00010100
+ 00011110
----------
  00110010
```

Both operands and the result are positive. Therefore, the representation is identical in all three systems:

```text
Sign-and-magnitude: 00110010
One’s complement:   00110010
Two’s complement:   00110010
```

Thus:

```text
00110010 = +50
```

## 3. Calculate 36 − 12

The expected decimal result is:

```text
36 − 12 = 24
```

Positive operands:

```text
36 = 00100100
12 = 00001100
24 = 00011000
```

### Sign-and-magnitude

Both operands are positive and 36 is greater than 12. Subtracting their magnitudes gives:

```text
36 − 12 = 24
```

Therefore:

```text
00011000 = +24 in eight-bit sign-and-magnitude
```

### One’s complement

Form negative 12 by inverting positive 12:

```text
+12:             00001100
−12:             11110011
```

Add positive 36:

```text
  00100100
+ 11110011
----------
1 00010111
```

One’s-complement arithmetic uses an end-around carry. Add the carry to the least-significant bit:

```text
  00010111
+        1
----------
  00011000
```

Therefore:

```text
00011000 = +24 in eight-bit one’s complement
```

### Two’s complement

Form negative 12:

```text
+12:             00001100
One’s complement:11110011
Add one:         00000001
                  --------
−12:             11110100
```

Add positive 36:

```text
  00100100
+ 11110100
----------
1 00011000
```

Discard the carry outside the eight-bit result:

```text
00011000 = +24 in eight-bit two’s complement
```

## Calculation summary

| Expression | Decimal result | Sign-and-magnitude | One’s complement | Two’s complement |
| ---------- | -------------: | ------------------ | ---------------- | ---------------- |
| `10 − 19`  |             −9 | `10001001`         | `11110110`       | `11110111`       |
| `20 + 30`  |            +50 | `00110010`         | `00110010`       | `00110010`       |
| `36 − 12`  |            +24 | `00011000`         | `00011000`       | `00011000`       |
