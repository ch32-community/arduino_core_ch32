# CH32 PSRAM allocation

The CH32V467RET6 provides 4 MiB of in-package PSRAM. The core initializes it
before `setup()` and also on the first allocation, so global constructors can
use it without an explicit `begin()` call. Both Metro CH32V467 clock options
are supported: a 200 MHz system PLL with 100 or 200 MHz HCLK.

The allocation interface follows Arduino-Pico:

```cpp
uint8_t *samples = (uint8_t *)pmalloc(65536);
if (samples) {
  samples[0] = 42;
  free(samples);
}
```

- `pmalloc(bytes)` allocates PSRAM aligned for ordinary C/C++ types.
- `pcalloc(count, bytes)` allocates zero-filled PSRAM and checks multiplication
  overflow. Both allocation functions return `nullptr` for zero-sized requests,
  unavailable PSRAM, or insufficient space.
- Ordinary `free(ptr)` and `realloc(ptr, bytes)` recognize PSRAM pointers.
  A successful resize preserves the existing contents up to the smaller size;
  a failed resize leaves the original allocation valid. Resizing a PSRAM
  allocation to zero frees it and returns `nullptr`. Growing a buffer uses an
  aligned replacement, so space for both buffers is needed during the copy.
- Ordinary `malloc()`, `calloc()`, `new`, and `realloc(nullptr, bytes)` use
  internal RAM. Explicit PSRAM allocation never falls back to internal RAM.
- `getPSRAMSize()` reports physical capacity; `getTotalPSRAMHeap()` reports
  initial allocatable space after allocator metadata; `getFreePSRAMHeap()` and
  `getUsedPSRAMHeap()` report current space in bytes. Free space may be fragmented.
  These global functions correspond to Pico's similarly named helper methods.

These functions are included by `Arduino.h`; no special include is required
in a sketch. Use them from foreground code, not interrupt handlers. The core
preserves the caller's interrupt state while updating allocator metadata and
keeps interrupts available during large buffer copies and zero-fills.

CH32V407 and other unsupported chips return `nullptr` for PSRAM allocations
and zero for capacity queries. Calling `pmalloc()` or `pcalloc()` on these
builds emits a compiler warning. This lets shared code detect missing PSRAM
without accessing its hardware. Unsupported clocks and initialization failures
also leave PSRAM unavailable.

Allocated buffers are ordinary memory and support byte, aligned halfword, and
aligned word accesses. Avoid unaligned 16/32-bit accesses and packed structures
requiring them. Contents are lost when power is removed. Do not change the
system clock or reconfigure the PSRAM controller while allocations exist.
The raw mapping belongs to the allocator; do not write directly to it.
Pico's `PSRAM` global-variable attribute is not implemented in this change.

See `PSRAMBuffer` for allocation, resizing, and release. Its `.skip.txt` excludes
families without PSRAM from the generic example sweep; build it explicitly for
a CH32V467 board.

The hardware setup is adapted from WCH's CH32V4x7 EVT example at
[afee2d1](https://github.com/ch32-riscv-ug/CH32V407/tree/afee2d144b184176e5ca21202714ca9add1dd46f)
and retains WCH's restriction to WCH microcontrollers. The heap uses the same
BSD-licensed TLSF implementation as Arduino-Pico; its source and provenance
are in `system/Drivers/tlsf`.
