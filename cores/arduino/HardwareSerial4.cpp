/*
  HardwareSerial4.cpp - Serial4, on USART4 or UART4.

  Its own file so that a sketch which never mentions Serial4 does not link
  it. The object, its receive buffer, its constructor and the interrupt
  handler below all come in together or not at all. This works because:
    - the core archive is linked without --whole-archive (see platform.txt),
      so an archive member is only linked when something references it, and
    - the startup file declares USART4_IRQHandler/UART4_IRQHandler weak,
      so the vector table alone does not pull this file in.

  Default pins: the port/pinmap chosen in the Tools > Serial menu when this
  is the selected port, otherwise the variant's PIN_SERIAL4_RX/TX. Move them with
  Serial4.setRx()/setTx() before begin().
*/

#include "Arduino.h"

// X035/X033 and L103 have USART4; V203RB, V208 and V30x have UART4.
#if defined(USART4)
  #define SERIAL4_PERIPH      USART4
  #define SERIAL4_IRQHandler  USART4_IRQHandler
#elif defined(UART4)
  #define SERIAL4_PERIPH      UART4
  #define SERIAL4_IRQHandler  UART4_IRQHandler
#endif

#if defined(HAVE_HWSERIAL4) && defined(SERIAL4_PERIPH)

#if defined(SERIAL_UART_INSTANCE) && SERIAL_UART_INSTANCE == 4
    HardwareSerial Serial4(SERIAL4_PERIPH, PIN_SERIAL_TX, PIN_SERIAL_RX);
#else
    HardwareSerial Serial4(SERIAL4_PERIPH, PIN_SERIAL4_TX, PIN_SERIAL4_RX);
#endif

#if ENABLE_RX_BUFFER
extern "C" void SERIAL4_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
extern "C" void SERIAL4_IRQHandler(void)
{
    Serial4._rx_irq(SERIAL4_PERIPH);
}
#endif

#endif // HAVE_HWSERIAL4
