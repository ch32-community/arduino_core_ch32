#include "Arduino.h"
#include "HardwareSerial_private.h"

#if defined(UART_MODULE_ENABLED) && !defined(UART_MODULE_ONLY)
#if defined(HAVE_HWSERIAL6) && defined(UART6)
DEFINE_HARDWARE_SERIAL_PORT(6, UART6, UART6_IRQHandler, UART6_IRQn)
#endif
#endif
