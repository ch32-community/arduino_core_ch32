/*
  HardwareSerial1.cpp - Serial1, on USART1.

  Its own file so that a sketch which never mentions Serial1 does not link
  it. The object, its receive buffer, its constructor and the interrupt
  handler below all come in together or not at all. This works because:
    - the core archive is linked without --whole-archive (see platform.txt),
      so an archive member is only linked when something references it, and
    - the startup file declares USART1_IRQHandler weak, so the vector table
      alone does not pull this file in.

  Default pins: the port/pinmap chosen in the Tools > Serial menu when this
  is the selected port, otherwise the variant's PIN_SERIAL1_RX/TX. Move them with
  Serial1.setRx()/setTx() before begin().
*/

#include "Arduino.h"

#if defined(HAVE_HWSERIAL1) && defined(USART1)

#if defined(SERIAL_UART_INSTANCE) && SERIAL_UART_INSTANCE == 1
	HardwareSerial Serial1(USART1, digitalPinToPinName(PIN_SERIAL_RX), digitalPinToPinName(PIN_SERIAL_TX));
#else
	HardwareSerial Serial1(USART1, digitalPinToPinName(PIN_SERIAL1_RX), digitalPinToPinName(PIN_SERIAL1_TX));
#endif

#if ENABLE_RX_BUFFER
extern "C" void USART1_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
extern "C" void USART1_IRQHandler(void)
{
	Serial1._rx_irq(USART1);
}
#endif

#endif // HAVE_HWSERIAL1
