/*
  pins_arduino_constexpr.h - Compile-time Arduino pin number -> PinName.

  digitalPinToPinName() reads the variant's digitalPin[]/analogInputPin[]
  tables, which are defined in the variant .cpp and so cannot be read by the
  compiler. A constructor such as Serial1(USART1, PIN_SERIAL1_TX,
  PIN_SERIAL1_RX) therefore did a table lookup at startup, and linked the
  tables, even though its pins never change.

  The variant header provides the same initializers as VARIANT_DIGITAL_PIN_MAP
  and VARIANT_ANALOG_PIN_MAP. The constexpr copies below are built from them,
  so a constant pin number converts to a constant PinName and the lookup
  disappears. The copies are C++17 inline variables: one definition across
  translation units, emitted only if something reads them at run time.
*/
#ifndef _PINS_ARDUINO_CONSTEXPR_H_
#define _PINS_ARDUINO_CONSTEXPR_H_

#include "pins_arduino.h"

#ifdef __cplusplus

namespace ch32 {
namespace detail {

inline constexpr PinName digitalPin[] = VARIANT_DIGITAL_PIN_MAP;
inline constexpr uint32_t analogInputPin[] = VARIANT_ANALOG_PIN_MAP;

// digitalPinToPinName() indexes these tables up to these counts unchecked.
static_assert(sizeof(digitalPin) / sizeof(digitalPin[0]) >= NUM_DIGITAL_PINS,
              "VARIANT_DIGITAL_PIN_MAP has fewer than NUM_DIGITAL_PINS entries");
static_assert(sizeof(analogInputPin) / sizeof(analogInputPin[0]) >= NUM_ANALOG_INPUTS,
              "VARIANT_ANALOG_PIN_MAP has fewer than NUM_ANALOG_INPUTS entries");

} // namespace detail

// The same conversion as digitalPinToPinNameImpl() in pins_arduino.h, step for
// step, so a pin converts identically at compile time and at run time.
constexpr PinName digitalPinToPinNameConstexpr(uint32_t pin)
{
  uint32_t index = pin & PNUM_MASK;

#if NUM_ANALOG_INPUTS > 0
  if (index >= NUM_DIGITAL_PINS &&
      (index & PNUM_ANALOG_BASE) == PNUM_ANALOG_BASE &&
      index < NUM_ANALOG_INTERNAL_FIRST) {
    index = detail::analogInputPin[index & PNUM_ANALOG_INDEX];
  }
#endif

  if (index >= NUM_DIGITAL_PINS) {
    return NC;
  }

  return static_cast<PinName>(detail::digitalPin[index] | (pin & ALTX_MASK));
}

// Converts at compile time when the pin is a constant once inlined, and falls
// back to the runtime tables otherwise, so a variable pin does not pull the
// constexpr copies into the image.
__attribute__((always_inline)) inline PinName pinToPinName(uint32_t pin)
{
  return __builtin_constant_p(pin) ? digitalPinToPinNameConstexpr(pin)
                                   : digitalPinToPinName(pin);
}

} // namespace ch32

#endif // __cplusplus

#endif // _PINS_ARDUINO_CONSTEXPR_H_
