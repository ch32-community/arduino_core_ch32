#include "Arduino.h"

// Preserve the distinction between Arduino pin numbers and native PinName
// constants. C consumers cannot select these overloads themselves.
static inline PinName serialPinName(uint32_t pin)
{
  return digitalPinToPinName(pin);
}

static inline PinName serialPinName(PinName pin)
{
  return pin;
}

extern "C" PinName serialRxPinName(void)
{
#if defined(PIN_SERIAL_RX)
  return serialPinName(PIN_SERIAL_RX);
#else
  return NC;
#endif
}

extern "C" PinName serialTxPinName(void)
{
#if defined(PIN_SERIAL_TX)
  return serialPinName(PIN_SERIAL_TX);
#else
  return NC;
#endif
}
