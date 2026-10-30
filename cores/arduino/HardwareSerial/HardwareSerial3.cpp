#include "Arduino.h"
#include "HardwareSerial_private.h"

#if defined(UART_MODULE_ENABLED) && !defined(UART_MODULE_ONLY)
#if defined(HAVE_HWSERIAL3) && defined(USART3)
DEFINE_HARDWARE_SERIAL_PORT(3, USART3, USART3_IRQHandler, USART3_IRQn)
#endif
#endif
