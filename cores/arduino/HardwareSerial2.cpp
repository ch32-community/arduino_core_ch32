/*
  HardwareSerial2.cpp - Serial2, on USART2.

  Its own file so that a sketch which never mentions Serial2 does not link
  it. The object, its receive buffer, its constructor and the interrupt
  handler below all come in together or not at all. This works because:
    - the core archive is linked without --whole-archive (see platform.txt),
      so an archive member is only linked when something references it, and
    - the startup file declares USART2_IRQHandler weak, so the vector table
      alone does not pull this file in.

  Default pins: the port/pinmap chosen in the Tools > Serial menu when this
  is the selected port, otherwise the variant's PIN_SERIAL2_RX/TX. Move them with
  Serial2.setRx()/setTx() before begin().
*/

#include "Arduino.h"

#if defined(HAVE_HWSERIAL2) && defined(USART2)

#if defined(SERIAL_UART_INSTANCE) && SERIAL_UART_INSTANCE == 2
	HardwareSerial Serial2(USART2, PIN_SERIAL_TX, PIN_SERIAL_RX);
#else
    HardwareSerial Serial2(USART2, PIN_SERIAL2_TX, PIN_SERIAL2_RX);
#endif

#if ENABLE_RX_BUFFER
extern "C" void USART2_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
extern "C" void USART2_IRQHandler(void)
{
    Serial2._rx_irq(USART2);
}
#endif

#endif // HAVE_HWSERIAL2
