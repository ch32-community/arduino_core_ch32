#include "Arduino.h"
#include "HardwareSerial_private.h"

#if defined(UART_MODULE_ENABLED) && !defined(UART_MODULE_ONLY)
#if defined(HAVE_HWSERIAL5) && defined(UART5)
DEFINE_HARDWARE_SERIAL_PORT(5, UART5, UART5_IRQHandler, UART5_IRQn)
#endif
#endif
