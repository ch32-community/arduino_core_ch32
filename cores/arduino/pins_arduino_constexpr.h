/* Compile-time Arduino pin conversion for C++ peripheral constructors. */
#ifndef _PINS_ARDUINO_CONSTEXPR_H_
#define _PINS_ARDUINO_CONSTEXPR_H_

#include "pins_arduino.h"

namespace ch32 {
namespace detail {

// A template keeps the header-defined tables valid across C++14 translation
// units. Constant conversions do not emit either table.
// The variant .cpp uses the same initializers to define the C runtime tables.
template <typename = void> struct VariantPinMap {
  static constexpr PinName digitalPin[] = VARIANT_DIGITAL_PIN_MAP;
  static constexpr uint32_t analogInputPin[] = VARIANT_ANALOG_PIN_MAP;
};

template <typename T> constexpr PinName VariantPinMap<T>::digitalPin[];
template <typename T> constexpr uint32_t VariantPinMap<T>::analogInputPin[];

// Some variants retain trailing table entries outside their exposed pin count.
static_assert(sizeof(VariantPinMap<>::digitalPin) / sizeof(PinName) >= NUM_DIGITAL_PINS,
              "Variant digital pin mapping is too short");

} // namespace detail

// Match digitalPinToPinName(), including analog aliases and alternate routes.
__attribute__((always_inline)) constexpr PinName digitalPinToPinNameConstexpr(uint32_t pin)
{
  if ((pin & PNUM_MASK) < NUM_DIGITAL_PINS) {
    return static_cast<PinName>(detail::VariantPinMap<>::digitalPin[pin & PNUM_MASK] |
                                (pin & ALTX_MASK));
  }
#if NUM_ANALOG_INPUTS > 0
  if ((pin & PNUM_ANALOG_BASE) == PNUM_ANALOG_BASE &&
      (pin & PNUM_MASK) < NUM_ANALOG_INTERNAL_FIRST &&
      (pin & PNUM_ANALOG_INDEX) < sizeof(detail::VariantPinMap<>::analogInputPin) / sizeof(uint32_t)) {
    const uint32_t digital = detail::VariantPinMap<>::analogInputPin[pin & PNUM_ANALOG_INDEX];
    // Some packages leave holes (NC) or omit trailing analog channels.
    if (digital < sizeof(detail::VariantPinMap<>::digitalPin) / sizeof(PinName)) {
      return static_cast<PinName>(detail::VariantPinMap<>::digitalPin[digital] |
                                  (pin & ALTX_MASK));
    }
  }
#endif
  return NC;
}

// Runtime constructor arguments reuse the existing C tables instead of emitting
// a second copy of the constexpr tables. GCC folds constant aliases after inlining.
__attribute__((always_inline)) inline PinName serialPinToPinName(uint32_t pin)
{
  return __builtin_constant_p(pin) ? digitalPinToPinNameConstexpr(pin)
                                   : digitalPinToPinName(pin);
}

} // namespace ch32

#endif // _PINS_ARDUINO_CONSTEXPR_H_
