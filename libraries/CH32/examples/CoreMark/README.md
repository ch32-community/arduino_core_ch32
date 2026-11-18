# CoreMark - CPU Performance Benchmark
Source: [CoreMark](https://github.com/PaulStoffregen/CoreMark) by Paul Stoffregen

## Background info
CoreMark was designed by EEMBC to replace Dhrystone by measuring basic processor pipeline efficiency using common embedded algorithms. 
Paul Stoffregen’s version is an Arduino-friendly port of the [official EEMBC benchmark](https://www.eembc.org/coremark/) that runs the exact same core algorithms while wrapping them in Arduino environment setup and timing functions.

## Functional description 
Measures the number for times per second your processor can perform a
variety of common tasks: linked list management, matrix multiply, and
executing state machines.

## CH32 configuration and compilation
This release has been configured to run on the smallest CH32 processor: the CH32V003. To fit it into 16kB Flash and 2kB RAM, the following configuration applies:
- Use `TOTAL_DATA_SIZE 2*500` instead of `TOTAL_DATA_SIZE 2*1000`
- Select `MEM_METHOD MEM_STATIC` instead of `MEM_METHOD MEM_STACK`. This will produce link errors when data size is too large.
- Use these options:
    - `HAS_FLOAT 0`
    - `HAS_STDIO 0`
    - `HAS_PRINTF 0`
- Compile using -Os LTO and only have the UART peripheral enabled.

With `HAS_FLOAT 0`, elapsed time retains millisecond precision and iterations per second are calculated from raw ticks using integer arithmetic, then rounded to two decimal places. Earlier versions divided by elapsed time truncated to whole seconds, which inflated the reported score. The CH32 scores below predate this correction and have not been remeasured.

#### Incompatible scores
Please note that the effect of these configuration changes means that the resulting score for CH32 is not 1:1 comparable to the scores of other boards. The numbers listed for those boards in the section below are the original scores, as published by Paul Stoffregen in 2019. They are provided for reference purposes only.
 - Under EEMBC rules, changing TOTAL_DATA_SIZE from 2000 bytes to 1000 bytes invalidates the score for official comparison against other MCUs. Using a smaller dataset means linked lists have fewer nodes and matrices are smaller.
 - While CoreMark normalizes scores by iterations per second, smaller loops reduce pointer-chasing overhead and branch mispredictions, often resulting in a slightly higher (optimistic) CoreMark/MHz score.
 - Using the same configuration,  we can compare scores with other cH32 processors but they are not a fair comparison against benchmark scores running the 2000-byte standard.


## Scores for CH32 boards

- If not mentioned explicitely, clockspeed is the the default clock frequency as set in the IDE.
- CoreMark Size    : 333, 
- Memory location  : Static
- VM002 tested both with<sup>+</sup> and without<sup>-</sup> _zmmul compilation option for hardware multiplication.


| Board                  | CoreMark |  Duration |
| ---------------------- | :------: | :----: |
| CH32V003 @ 48 MHz HSI  | 111      | 18s, 2000 it   |
| CH32V002 @ 48 MHz HSI<sup>+</sup> | 142    | 14s, 2000 it   | 
| CH32V002 @ 48 MHz HSI<sup>-</sup> | 84     | 13s, 1100 it   | 
| CH32V002 @ 24 MHz HSI<sup>+</sup> | 100    | 11s, 1100 it   | 
| CH32V002 @ 8 MHz HSI<sup>+</sup>  | 40     | 15s, 600 it    | 
| CH32V002 @ 8 MHz HSI<sup>-</sup>  | 14     | 14s, 200 it    | 
| CH32VM00X->V006        | ???      |        |
| CH32X03x               | ???      |        |
| CH32V103 @ 72MHz HSI   | 352      | 17s, 6000 it   |
| CH32V203 @ 144MHz HSI    | 846      | 13s, 11000 it  |
| CH32V307 @ 144 MHz HSI | 1000     | 11s, 11000 it  |
| CH32V4x7               | ???      |        |
| CH32L10x               | ???      |        |

(larger numbers are better)




## Scores for other Arduino boards

| Board                  | CoreMark |
| ---------------------- | :------: |
| Teensy 4.0             | 2313.57  |
| Adafruit Metro M4 (200MHz overclock, 'dragons' optimization) | 536.35   |
| Adafruit Metro M4 (180MHz overclock, faster optimizations) | 458.19   |
| Teensy 3.6             | 440.72   |
| Sparkfun ESP32 Thing   | 351.33   |
| Adafruit HUZZAH 32     | 351.35   |
| Teensy 3.5             | 265.50   |
| Teensy 3.2 (96MHz overclock, faster optimizations)            | 218.26   |
| Adafruit Metro M4 (120MHz, smaller code) | 214.85   |
| Teensy 3.2 (72MHz)            | 168.62   |
| Teensy 3.2 (72MHz, smaller code)            | 126.76   |
| Arduino Due            | 94.95    |
| Arduino Zero           | 56.86    |
| Arduino Nano Every     | 8.20     |
| Arduino Mega           | 7.03     |

(larger numbers are better)

