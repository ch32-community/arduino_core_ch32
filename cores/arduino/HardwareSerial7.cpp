/*
  HardwareSerial7.cpp - Serial7, on UART7.

  Its own file so that a sketch which never mentions Serial7 does not link
  it. The object, its receive buffer, its constructor and the interrupt
  handler below all come in together or not at all. This works because:
    - the core archive is linked without --whole-archive (see platform.txt),
      so an archive member is only linked when something references it, and
    - the startup file declares UART7_IRQHandler weak, so the vector table
      alone does not pull this file in.

  Default pins: the port/pinmap chosen in the Tools > Serial menu when this
  is the selected port, otherwise the variant's PIN_SERIAL7_RX/TX. Move them with
  Serial7.setRx()/setTx() before begin().
*/

#include "Arduino.h"

#if defined(HAVE_HWSERIAL7) && defined(UART7)

#if defined(SERIAL_UART_INSTANCE) && SERIAL_UART_INSTANCE == 7
    HardwareSerial Serial7(UART7, PIN_SERIAL_TX, PIN_SERIAL_RX);
#else
    HardwareSerial Serial7(UART7, PIN_SERIAL7_TX, PIN_SERIAL7_RX);
#endif

#if ENABLE_RX_BUFFER
extern "C" void UART7_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
extern "C" void UART7_IRQHandler(void)
{
    Serial7._rx_irq(UART7);
}
#endif

#endif // HAVE_HWSERIAL7
