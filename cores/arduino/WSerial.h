#ifndef WIRING_SERIAL_H
#define WIRING_SERIAL_H

#include "variant.h"

#if defined(USE_TINYUSB)
#include "Adafruit_USBD_CDC.h"
#endif

#if defined(UART_MODULE_ENABLED) && !defined(UART_MODULE_ONLY)
// The variant declares its supported HAVE_HWSERIALx ports; SerialRouting.h
// selects the Serial alias. Ports are constructed only when referenced.
#include "HardwareSerial.h"
#endif

#endif /* WIRING_SERIAL_H */
