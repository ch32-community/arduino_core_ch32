#include "Arduino.h"
#include "HardwareSerial_private.h"

#if defined(UART_MODULE_ENABLED) && !defined(UART_MODULE_ONLY)
#if defined(HAVE_HWSERIAL1) && defined(USART1)
DEFINE_HARDWARE_SERIAL_PORT(1, USART1, USART1_IRQHandler, USART1_IRQn)
#endif
#endif
