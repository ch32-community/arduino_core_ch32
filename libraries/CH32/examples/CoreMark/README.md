# CoreMark - CPU Performance Benchmark
source: [CoreMark](https://github.com/PaulStoffregen/CoreMark) by Paul Stoffregen

## Background info
CoreMark was designed by EEMBC to replace Dhrystone by measuring basic processor pipeline efficiency using common embedded algorithms. 
Paul Stoffregen’s version is an Arduino-friendly port of the [official EEMBC benchmark](https://www.eembc.org/coremark/) that runs the exact same core algorithms while wrapping them in Arduino environment setup and timing functions.

## Functional description 
Measures the number for times per second your processor can perform a
variety of common tasks: linked list management, matrix multiply, and
executing state machines.

## CH32 configuration and compilation
This release has been configured to run on the smallest CH32 processor: the CH32V003. To fit it into 16kB Flash and 1kB RAM, the following configuration applies:
- Use `TOTAL_DATA_SIZE 2*500` instead of `TOTAL_DATA_SIZE 2*1000`
- Select `MEM_METHOD MEM_STATIC` instead of `MEM_METHOD MEM_STACK`. This will produce link errors when data size is too large.
- Use these options:
    - `HAS_FLOAT 0`
    - `HAS_STDIO 0`
    - `HAS_PRINTF 0`
- Compile using -Os LTO and only have the UART peripheral enabled.
- Clockspeed is the the default clockspeed as set in the IDE, unless noted otherwise.

Please note that the effect of these configuration changes may very well mean that the resulting score for CH32 is not 1:1 comparable to the scores of other boards. The numbers listed for those boards are the original scores, as published by Paul Stoffregen in 2019.

## Scores for CH32 boards

CoreMark Size    : 333, 
Memory location  : Static

| Board                  | CoreMark |  Notes |
| ---------------------- | :------: | :----: |
| CH32V003               | 111      | 18s, 2000 it    |
| CH32VM00X->V002        | 142      | 14s, 2000 it     | 
| V002 @ 24 MHz HSI      | 100      | 11s, 1100 it    | 
| CH32VM00X->V006        | ?.??     |        |
| CH32V10x               | ?.??     |        |
| CH32V20x               | ?.??     |        |
| CH32V30x               | ?.??     |        |
| CH32V4x7               | ?.??     |        |
| CH32L10x               | ?.??     |        |

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

