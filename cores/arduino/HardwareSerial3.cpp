/*
  HardwareSerial3.cpp - Serial3, on USART3.

  Its own file so that a sketch which never mentions Serial3 does not link
  it. The object, its receive buffer, its constructor and the interrupt
  handler below all come in together or not at all. This works because:
    - the core archive is linked without --whole-archive (see platform.txt),
      so an archive member is only linked when something references it, and
    - the startup file declares USART3_IRQHandler weak, so the vector table
      alone does not pull this file in.

  Default pins: the port/pinmap chosen in the Tools > Serial menu when this
  is the selected port, otherwise the variant's PIN_SERIAL3_RX/TX. Move them with
  Serial3.setRx()/setTx() before begin().
*/

#include "Arduino.h"

#if defined(HAVE_HWSERIAL3) && defined(USART3)

#if defined(SERIAL_UART_INSTANCE) && SERIAL_UART_INSTANCE == 3
    HardwareSerial Serial3(USART3, PIN_SERIAL_TX, PIN_SERIAL_RX);
#else
    HardwareSerial Serial3(USART3, PIN_SERIAL3_TX, PIN_SERIAL3_RX);
#endif

#if ENABLE_RX_BUFFER
extern "C" void USART3_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
extern "C" void USART3_IRQHandler(void)
{
    Serial3._rx_irq(USART3);
}
#endif

#endif // HAVE_HWSERIAL3
