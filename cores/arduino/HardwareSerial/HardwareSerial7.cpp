#include "Arduino.h"
#include "HardwareSerial_private.h"

#if defined(UART_MODULE_ENABLED) && !defined(UART_MODULE_ONLY)
#if defined(HAVE_HWSERIAL7) && defined(UART7)
DEFINE_HARDWARE_SERIAL_PORT(7, UART7, UART7_IRQHandler, UART7_IRQn)
#endif
#endif
