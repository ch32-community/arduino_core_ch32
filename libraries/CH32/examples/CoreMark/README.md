# CoreMark - CPU Performance Benchmark
Source: [CoreMark](https://github.com/PaulStoffregen/CoreMark) by Paul Stoffregen

## Background info
CoreMark was designed by EEMBC to replace Dhrystone by measuring basic processor pipeline efficiency using common embedded algorithms. 
Paul Stoffregen’s version is an Arduino-friendly port of the [official EEMBC benchmark](https://www.eembc.org/coremark/) that runs the
exact same core algorithms while wrapping them in Arduino environment setup and timing functions. For CH32 minor changes were made in
configuration and showing results.

## Functional description 
Measures the number for times per second your processor can perform a variety of common tasks: linked list management, 
matrix multiply, and executing state machines.

## CH32 configuration and compilation
This release has been configured to run on the smallest CH32 processor: the CH32V003. To fit it into 16kB Flash and 2kB RAM,
the following configuration applies:
- Use `TOTAL_DATA_SIZE 2*500` instead of `TOTAL_DATA_SIZE 2*1000`
- Select `MEM_METHOD MEM_STATIC` instead of `MEM_METHOD MEM_STACK`. This will produce link errors when data size is too large.
- Use these options:
    - `HAS_FLOAT 0`
    - `HAS_STDIO 0`
    - `HAS_PRINTF 0`
- Compile using -Os LTO and only have the UART peripheral enabled.


#### Incompatible scores
Please note that the effect of these configuration changes means that the resulting score for CH32 is not 1:1 comparable to
the scores of other boards. The numbers listed for those boards in the section below are the original scores, as published
by Paul Stoffregen in 2019. They are provided for reference purposes only.
 - Under EEMBC rules, changing TOTAL_DATA_SIZE from 2000 bytes to 1000 bytes invalidates the score for official comparison against other MCUs. Using a smaller dataset means linked lists have fewer nodes and matrices are smaller.
 - While CoreMark normalizes scores by iterations per second, smaller loops reduce pointer-chasing overhead and branch mispredictions, often resulting in a slightly higher (optimistic) CoreMark/MHz score.
 - Using the same configuration,  we can compare scores with other CH32 processors but they are not a fair comparison against benchmark scores running the 2000-byte standard.
 - Optimization matters a lot. We've chosen to use `-Os LTO` to allow comparing V003 and V002.
 - In Application Note AN03002 WCH lists guidelines and some CoreMark scores: [CN original](https://github.com/openwch/ch32_application_notes/blob/main/zh/AN03002/AN03002.md) - [EN translated](https://github-com.translate.goog/openwch/ch32_application_notes/blob/main/zh/AN03002/AN03002.md?_x_tr_sl=auto&_x_tr_tl=en&_x_tr_hl=nl&_x_tr_pto=wapp).


## Scores for CH32 boards

- If not mentioned explicitely, clockspeed is the the default clock frequency as set in the IDE.
- CoreMark Size    : 333, 
- Memory location  : Static
- VM002 tested both with<sup>+</sup> and without<sup>-</sup> _zmmul compilation option for hardware multiplication.
- With `HAS_FLOAT 0`, elapsed time retains millisecond precision and iterations per second are calculated from raw ticks using integer arithmetic, then rounded to two decimal places. Earlier versions divided by elapsed time truncated to whole seconds, which inflated the reported score. The CH32 scores below listed in **_italics_** predate this correction but have been recalculated using stored measurements for ticks and iterations.
- Using the same configuration and compilation settings, V002 and V006 scored the same. X033 and X035 also scored the same. Using optimization `-O3 (fastest)` gave a much larger binary, but also gave a much better score. See row `CH32V006 -O3` in the table below.


| Board + speed option   | CoreMark |  Duration |
| ---------------------- | :------: | :----: |
| CH32V003 @ 48 MHz HSI  | _110.85_ | 18.1s, 2000 it  |
| CH32V002 @ 48 MHz HSI<sup>+</sup> | 136.41 | 14.7s, 2000 it | 
| CH32V002 @ 48 MHz HSI<sup>-</sup> | _80.95_| 13.6s, 1100 it | 
| CH32V002 @ 24 MHz HSI<sup>+</sup> | 92.62  | 11.9s, 1100 it | 
| CH32V002 @ 8 MHz HSI<sup>+</sup>  | 39.13  | 15.3s, 600 it  | 
| CH32V002 @ 8 MHz HSI<sup>-</sup>  | _13.44_| 14.9s, 200 it  | 
| CH32V006 -O3 (fastest)<sup>+</sup>| 192.15 | 15.6s 3000 it  |
| CH32X033 @ 48 MHz HSI  | 106.19   | 18.8s, 2000 it  |
| CH32V103 @ 72 MHz HSI  | _339.54_ | 17.7s, 6000 it  |
| CH32V203 @ 144 MHz HSI | _792.45_ | 13.9s, 11000 it |
| CH32V307 @ 144 MHz HSI | _933,31_ | 11.8s, 11000 it |
| CH32V4x7               | ???      |        |
| Metro CH32V467 @100 MHz| 659.04   | 16.7s, 11000 it |
| Metro CH32V467 @200 MHz| 1318.22  | 15.2s, 20000 it |
| CH32L10x               | ???      |        |

(larger numbers are better)


## Scores for other Arduino boards

- These scores are as mentioned in the README.md by Paul Stoffregen; dating from 2019.

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

