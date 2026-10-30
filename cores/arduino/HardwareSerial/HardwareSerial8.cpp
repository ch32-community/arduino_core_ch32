#include "Arduino.h"
#include "HardwareSerial_private.h"

#if defined(UART_MODULE_ENABLED) && !defined(UART_MODULE_ONLY)
#if defined(HAVE_HWSERIAL8) && defined(UART8)
DEFINE_HARDWARE_SERIAL_PORT(8, UART8, UART8_IRQHandler, UART8_IRQn)
#endif
#endif
