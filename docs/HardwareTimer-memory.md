# HardwareTimer memory and flash configuration

`HardwareTimer_Handle[TIMER_NUM]` was an array of pointers, not an array of timer
objects. On these 32-bit chips it reserved `4 * TIMER_NUM` bytes of zero-initialized
RAM; it did not store that many objects or that many initialized bytes in flash.
Objects are constructed only when a sketch declares/creates them, or when PWM
first needs a timer.

The default implementation now registers constructed timers in a linked list.
The registry uses one global pointer (4 bytes) and one link per object (4 bytes).
Channel modes use 16-bit storage rather than four full enums, saving 8 bytes per
object. Together, these changes reduce each object by 4 bytes as well as replacing
the fixed registry with a single pointer. Lookup takes O(number of active timers);
the compatibility registry provides constant-time indexed lookup.

Only one object may own a hardware timer. Copying timer objects is disabled.
Use `HardwareTimer::getInstance(TIM1)` to find an existing object, or get `nullptr`
if none has been constructed. The public `HardwareTimer_Handle` array is available
only in compatibility builds. Create and destroy timer objects in foreground code,
not in interrupt callbacks.

## Build options

These defines must be applied to the **entire core and sketch build**, not just
placed above an include in a sketch. They affect class layout and callback types.
Rebuild the core when changing them.

| Define | Effect |
| --- | --- |
| `ENSURE_COMPATIBILITY` | Uses the pointer array, full enum storage, `std::function` callbacks, and the original channel/interrupt dispatch paths. Overrides the two options below. Correctness fixes apply in both modes. |
| `HARDWARE_TIMER_ENABLED_MASK` | Selects which hardware timers HardwareTimer supports. Bit 0 is TIM1, bit 1 TIM2, through bit 9 TIM10. Defaults to all supported timers (`0x3FF`). |
| `HARDWARE_TIMER_SIMPLE_CALLBACKS` | Uses `void (*)(void)` callbacks. Supports ordinary functions and lambdas without captures; capturing lambdas and other stateful callables require the default `std::function` mode. |

For example, TIM1 only with simple callbacks:

```sh
arduino-cli compile --fqbn ch32-community:ch32:CH32V30x_EVT \
  --build-property "build.extra_flags=-DHARDWARE_TIMER_ENABLED_MASK=1 -DHARDWARE_TIMER_SIMPLE_CALLBACKS" \
  TimerSize
```

Use your installed platform's FQBN. The measurements below used the local checkout
registered as `local:ch32:CH32V30x_EVT`.

Mask examples: TIM1 = `1`, TIM2 = `2`, TIM1 + TIM2 = `3`, TIM1 + TIM8 = `129`.
Include every timer needed by the sketch, libraries, and `analogWrite()` pin mappings.
Excluded timers are rejected by `setup()`; their HardwareTimer interrupt handlers
are omitted. A runtime `TIM_TypeDef *` constructor does not let the linker reliably
infer which timer vectors can be removed, so automatic object registration alone
does not eliminate all unused interrupt handlers.

The default callback API remains unchanged. With simple callbacks, five callback
slots take 20 bytes rather than the 80 bytes used by this toolchain's
`std::function<void()>` representation. No registry allocation is performed on the
heap. `analogWrite()` can still allocate a timer on first use. `pwm_stop()` pauses
only its channel and retains the object for reuse; it does not delete a timer that
may be shared with other pins or owned by the sketch.

## Measurements and validation

CH32V307VCT6, GCC 8.2.0, board default `-Os`, no LTO. These are complete linked
sketch totals, including other Arduino core overhead, rather than just timer code.
RAM is the linker-reported global allocation, excluding stack and runtime heap.

| One TIM1 update callback | Flash (bytes) | Global RAM (bytes) |
| --- | ---: | ---: |
| Compatibility mode | 8,588 | 776 |
| Default optimized mode, all timers available | 8,428 | 712 |
| TIM1 mask + simple callbacks | 6,440 | 544 |

The small build contains only TIM1's two HardwareTimer interrupt handlers. Object
sizes on this target are 132 bytes (compatibility), 128 bytes (default), and 68 bytes
(simple callbacks).

The benchmark sketch was:

```cpp
#include <HardwareTimer.h>

HardwareTimer timer;
volatile unsigned ticks;

void onTick() { ++ticks; }

void setup() {
  timer.setup(TIM1);
  timer.setOverflow(1000, HERTZ_FORMAT);
  timer.attachInterrupt(onTick);
  timer.resume();
}

void loop() {}
```

Compilation was checked for CH32V00x, CH32VM00X, CH32X035, CH32V10x, CH32V20x,
CH32V30x, and CH32L10x. This does not replace testing PWM waveforms, capture edges,
and interrupt timing on the physical boards. The vendor output-compare/input-capture
initializers remain responsible for family-specific register behavior.
