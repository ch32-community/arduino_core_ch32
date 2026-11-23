/*
  HardwareSerial5.cpp - Serial5, on UART5.

  Its own file so that a sketch which never mentions Serial5 does not link
  it. The object, its receive buffer, its constructor and the interrupt
  handler below all come in together or not at all. This works because:
    - the core archive is linked without --whole-archive (see platform.txt),
      so an archive member is only linked when something references it, and
    - the startup file declares UART5_IRQHandler weak, so the vector table
      alone does not pull this file in.

  Default pins: the port/pinmap chosen in the Tools > Serial menu when this
  is the selected port, otherwise the variant's PIN_SERIAL5_RX/TX. Move them with
  Serial5.setRx()/setTx() before begin().
*/

#include "Arduino.h"

#if defined(HAVE_HWSERIAL5) && defined(UART5)

#if defined(SERIAL_UART_INSTANCE) && SERIAL_UART_INSTANCE == 5
    HardwareSerial Serial5(UART5, PIN_SERIAL_TX, PIN_SERIAL_RX);
#else
    HardwareSerial Serial5(UART5, PIN_SERIAL5_TX, PIN_SERIAL5_RX);
#endif

#if ENABLE_RX_BUFFER
extern "C" void UART5_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
extern "C" void UART5_IRQHandler(void)
{
    Serial5._rx_irq(UART5);
}
#endif

#endif // HAVE_HWSERIAL5
