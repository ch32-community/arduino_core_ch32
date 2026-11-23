/*
  HardwareSerial6.cpp - Serial6, on UART6.

  Its own file so that a sketch which never mentions Serial6 does not link
  it. The object, its receive buffer, its constructor and the interrupt
  handler below all come in together or not at all. This works because:
    - the core archive is linked without --whole-archive (see platform.txt),
      so an archive member is only linked when something references it, and
    - the startup file declares UART6_IRQHandler weak, so the vector table
      alone does not pull this file in.

  Default pins: the port/pinmap chosen in the Tools > Serial menu when this
  is the selected port, otherwise the variant's PIN_SERIAL6_RX/TX. Move them with
  Serial6.setRx()/setTx() before begin().
*/

#include "Arduino.h"

#if defined(HAVE_HWSERIAL6) && defined(UART6)

#if defined(SERIAL_UART_INSTANCE) && SERIAL_UART_INSTANCE == 6
    HardwareSerial Serial6(UART6, digitalPinToPinName(PIN_SERIAL_RX), digitalPinToPinName(PIN_SERIAL_TX));
#else
    HardwareSerial Serial6(UART6, digitalPinToPinName(PIN_SERIAL6_RX), digitalPinToPinName(PIN_SERIAL6_TX));
#endif

#if ENABLE_RX_BUFFER
extern "C" void UART6_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
extern "C" void UART6_IRQHandler(void)
{
    Serial6._rx_irq(UART6);
}
#endif

#endif // HAVE_HWSERIAL6
