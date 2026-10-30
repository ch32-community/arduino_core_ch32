#include "Arduino.h"
#include "HardwareSerial_private.h"

#if defined(UART_MODULE_ENABLED) && !defined(UART_MODULE_ONLY)
HardwareSerial::HardwareSerial(void *peripheral)
  : HardwareSerial(peripheral, pinmap_pin(peripheral, PinMap_UART_RX),
                   pinmap_pin(peripheral, PinMap_UART_TX))
{
}

HardwareSerial::HardwareSerial(void *peripheral, uint32_t rx, uint32_t tx)
  : HardwareSerial(peripheral, rx, tx, hardware_serial_irq_for(peripheral))
{
}

HardwareSerial::HardwareSerial(void *peripheral, PinName rx, PinName tx)
  : HardwareSerial(peripheral, rx, tx, hardware_serial_irq_for(peripheral))
{
}

void HardwareSerial::setHandler(void *handler)
{
  _serial.uart = static_cast<USART_TypeDef *>(handler);
#if ENABLE_RX_BUFFER
  _rx_irq = hardware_serial_irq_for(handler);
#endif
}

// Only direct peripheral constructors or setHandler() reference this resolver.
// Named SerialN factories bind their own IRQ and never pull in this object.
HardwareSerialIrq *hardware_serial_irq_for(void *peripheral)
{
#if defined(HAVE_HWSERIAL1) && defined(USART1)
  extern HardwareSerialIrq *hardware_serial_irq1();
  if (peripheral == USART1) { return hardware_serial_irq1(); }
#endif
#if defined(HAVE_HWSERIAL2) && defined(USART2)
  extern HardwareSerialIrq *hardware_serial_irq2();
  if (peripheral == USART2) { return hardware_serial_irq2(); }
#endif
#if defined(HAVE_HWSERIAL3) && defined(USART3)
  extern HardwareSerialIrq *hardware_serial_irq3();
  if (peripheral == USART3) { return hardware_serial_irq3(); }
#endif
#if defined(HAVE_HWSERIAL4) && defined(USART4)
  extern HardwareSerialIrq *hardware_serial_irq4();
  if (peripheral == USART4) { return hardware_serial_irq4(); }
#elif defined(HAVE_HWSERIAL4) && defined(UART4)
  extern HardwareSerialIrq *hardware_serial_irq4();
  if (peripheral == UART4) { return hardware_serial_irq4(); }
#endif
#if defined(HAVE_HWSERIAL5) && defined(UART5)
  extern HardwareSerialIrq *hardware_serial_irq5();
  if (peripheral == UART5) { return hardware_serial_irq5(); }
#endif
#if defined(HAVE_HWSERIAL6) && defined(UART6)
  extern HardwareSerialIrq *hardware_serial_irq6();
  if (peripheral == UART6) { return hardware_serial_irq6(); }
#endif
#if defined(HAVE_HWSERIAL7) && defined(UART7)
  extern HardwareSerialIrq *hardware_serial_irq7();
  if (peripheral == UART7) { return hardware_serial_irq7(); }
#endif
#if defined(HAVE_HWSERIAL8) && defined(UART8)
  extern HardwareSerialIrq *hardware_serial_irq8();
  if (peripheral == UART8) { return hardware_serial_irq8(); }
#endif
  return nullptr;
}
#endif
