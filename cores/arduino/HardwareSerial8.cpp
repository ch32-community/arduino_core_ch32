/*
  HardwareSerial8.cpp - Serial8, on UART8.

  Its own file so that a sketch which never mentions Serial8 does not link
  it. The object, its receive buffer, its constructor and the interrupt
  handler below all come in together or not at all. This works because:
    - the core archive is linked without --whole-archive (see platform.txt),
      so an archive member is only linked when something references it, and
    - the startup file declares UART8_IRQHandler weak, so the vector table
      alone does not pull this file in.

  Default pins: the port/pinmap chosen in the Tools > Serial menu when this
  is the selected port, otherwise the variant's PIN_SERIAL8_RX/TX. Move them with
  Serial8.setRx()/setTx() before begin().
*/

#include "Arduino.h"

#if defined(HAVE_HWSERIAL8) && defined(UART8)

#if defined(SERIAL_UART_INSTANCE) && SERIAL_UART_INSTANCE == 8
    HardwareSerial Serial8(UART8, PIN_SERIAL_TX, PIN_SERIAL_RX);
#else
    HardwareSerial Serial8(UART8, PIN_SERIAL8_TX, PIN_SERIAL8_RX);
#endif

#if ENABLE_RX_BUFFER
extern "C" void UART8_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
extern "C" void UART8_IRQHandler(void)
{
    Serial8._rx_irq(UART8);
}
#endif

#endif // HAVE_HWSERIAL8
