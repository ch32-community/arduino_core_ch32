#include "Arduino.h"
#include "HardwareSerial_private.h"

#if defined(UART_MODULE_ENABLED) && !defined(UART_MODULE_ONLY)
#if defined(HAVE_HWSERIAL2) && defined(USART2)
DEFINE_HARDWARE_SERIAL_PORT(2, USART2, USART2_IRQHandler, USART2_IRQn)
#endif
#endif
