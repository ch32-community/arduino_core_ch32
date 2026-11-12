# CH32PSRAM

Access the CH32V467RET6's 4 MiB of in-package PSRAM. A 200 MHz system PLL
with either 100 or 200 MHz HCLK is required; both Metro CH32V467 clock
options provide this configuration.

Create one `CH32PSRAM` instance and call `begin()` before using the memory.
`begin()` returns false on unsupported chips or clocks, or if initialization
times out. CH32V407 has no supported in-package PSRAM and returns false without
accessing the PSRAM controller.

- `size()` returns the capacity in bytes after successful initialization, or zero.
- `data()` returns a `volatile uint32_t *` for aligned 32-bit access, or `nullptr`
  before successful initialization.
- `readModeRegister(address)` reads a raw mode register at address 0–15. It
  returns -1 on invalid input, before initialization, or on a command timeout.

The memory is separate from the ordinary heap: `malloc()` and `new` continue
to use internal RAM. The example uses aligned 32-bit accesses. Byte and aligned
16-bit accesses are also supported by casting the returned pointer to
`volatile uint8_t *` or `volatile uint16_t *`. Do not use unaligned 16/32-bit
accesses or packed structures that require them. Contents are lost when power
is removed. Calling `begin()` again on the same initialized
instance preserves its contents. Do not change the system clock or reconfigure
the PSRAM controller while using the library.

See `PSRAMBuffer` for a simple buffer example. A Metro board definition with
the clock options above is needed to run it on the Adafruit Metro CH32V467.

Initialization is adapted from WCH's CH32V4x7 EVT PSRAM example in the
[SDK mirror at afee2d1](https://github.com/ch32-riscv-ug/CH32V407/tree/afee2d144b184176e5ca21202714ca9add1dd46f).
The implementation retains WCH's
restriction to WCH microcontrollers; it is not relicensed under the Arduino
core's license.
