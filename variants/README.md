# Variant pin mappings

Each variant's existing `.h` file defines `VARIANT_DIGITAL_PIN_MAP` and
`VARIANT_ANALOG_PIN_MAP`. Update these initializer macros when changing a board's
pin numbering. The variant `.cpp` uses them to define the existing C runtime
tables. The C++ UART constructor uses the same initializers for `static constexpr`
members, so the two mappings stay in sync without an additional file per variant.

Continue each macro line except the final closing brace with a backslash. Use
block comments (`/* ... */`) inside these macros: line comments (`// ...`) would
consume subsequent entries after the preprocessor joins the continued lines.

Keep public aliases such as `PA6` as Arduino pin numbers. UART defaults can use
these aliases, including analog aliases and alternate routes. In optimized
builds, the inline constructor converts constant arguments directly to
`PinName` values without runtime table lookups. Variable arguments reuse the
existing runtime tables. The explicit `PA_6` constructor remains available.

Run `python tools/test_serial_pin_constants.py --compiler <path-to-riscv-none-embed-g++>`
from the core's root directory to cross-compile every variant's pin conversions,
UART defaults, and C runtime mapping API. The script also accepts `--variant`,
`--define`, and an optional `--baseline` snapshot containing `cores/` and
`variants/` to compare startup code and pin tables with a previous implementation.
