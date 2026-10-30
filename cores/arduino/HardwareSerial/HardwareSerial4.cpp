#include "Arduino.h"
#include "HardwareSerial_private.h"

#if defined(UART_MODULE_ENABLED) && !defined(UART_MODULE_ONLY)
#if defined(HAVE_HWSERIAL4) && defined(USART4)
DEFINE_HARDWARE_SERIAL_PORT(4, USART4, USART4_IRQHandler, USART4_IRQn)
#elif defined(HAVE_HWSERIAL4) && defined(UART4)
DEFINE_HARDWARE_SERIAL_PORT(4, UART4, UART4_IRQHandler, UART4_IRQn)
#endif
#endif
