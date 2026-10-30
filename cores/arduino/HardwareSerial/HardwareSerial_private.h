#ifndef HARDWARE_SERIAL_PRIVATE_H
#define HARDWARE_SERIAL_PRIVATE_H

#include "../HardwareSerial.h"
#include "../pins_arduino_analog.h"

struct HardwareSerialIrq {
  HardwareSerial * volatile instance;
  IRQn_Type irq;
};

#if defined(UART_MODULE_ENABLED) && !defined(UART_MODULE_ONLY)

// Only definitions used by a sketch are instantiated. The vector alias belongs
// to the template ISR's COMDAT section, allowing duplicate use from multiple TUs.
// RISC-V prints immediate symbols without a prefix; host tests use %c0 instead.
#ifdef __riscv
  #define HARDWARE_SERIAL_ISR_ADDRESS "%0"
#else
  #define HARDWARE_SERIAL_ISR_ADDRESS "%c0"
#endif

template<unsigned Port> struct HardwareSerialPortTraits;

#define DEFINE_HARDWARE_SERIAL_TRAITS(N, UART, IRQ_HANDLER, IRQ_NUMBER)       \
  template<> struct HardwareSerialPortTraits<N> {                            \
    static USART_TypeDef *peripheral() { return UART; }                       \
    static constexpr IRQn_Type irq = IRQ_NUMBER;                              \
    static auto rx() -> decltype(PIN_SERIAL##N##_RX) {                         \
      return PIN_SERIAL##N##_RX;                                             \
    }                                                                        \
    static auto tx() -> decltype(PIN_SERIAL##N##_TX) {                         \
      return PIN_SERIAL##N##_TX;                                             \
    }                                                                        \
    template<class Backend>                                                  \
    __attribute__((always_inline)) static inline void bindVector() {          \
      __asm__ volatile(".global " #IRQ_HANDLER "\n.set " #IRQ_HANDLER ","       \
                       HARDWARE_SERIAL_ISR_ADDRESS "\n"                      \
                       : : "i"(&Backend::receiveInterrupt));                 \
    }                                                                        \
  };

#if defined(HAVE_HWSERIAL1) && defined(USART1)
DEFINE_HARDWARE_SERIAL_TRAITS(1, USART1, USART1_IRQHandler, USART1_IRQn)
#endif
#if defined(HAVE_HWSERIAL2) && defined(USART2)
DEFINE_HARDWARE_SERIAL_TRAITS(2, USART2, USART2_IRQHandler, USART2_IRQn)
#endif
#if defined(HAVE_HWSERIAL3) && defined(USART3)
DEFINE_HARDWARE_SERIAL_TRAITS(3, USART3, USART3_IRQHandler, USART3_IRQn)
#endif
#if defined(HAVE_HWSERIAL4) && defined(USART4)
DEFINE_HARDWARE_SERIAL_TRAITS(4, USART4, USART4_IRQHandler, USART4_IRQn)
#elif defined(HAVE_HWSERIAL4) && defined(UART4)
DEFINE_HARDWARE_SERIAL_TRAITS(4, UART4, UART4_IRQHandler, UART4_IRQn)
#endif
#if defined(HAVE_HWSERIAL5) && defined(UART5)
DEFINE_HARDWARE_SERIAL_TRAITS(5, UART5, UART5_IRQHandler, UART5_IRQn)
#endif
#if defined(HAVE_HWSERIAL6) && defined(UART6)
DEFINE_HARDWARE_SERIAL_TRAITS(6, UART6, UART6_IRQHandler, UART6_IRQn)
#endif
#if defined(HAVE_HWSERIAL7) && defined(UART7)
DEFINE_HARDWARE_SERIAL_TRAITS(7, UART7, UART7_IRQHandler, UART7_IRQn)
#endif
#if defined(HAVE_HWSERIAL8) && defined(UART8)
DEFINE_HARDWARE_SERIAL_TRAITS(8, UART8, UART8_IRQHandler, UART8_IRQn)
#endif

#undef DEFINE_HARDWARE_SERIAL_TRAITS
#undef HARDWARE_SERIAL_ISR_ADDRESS

#if ENABLE_RX_BUFFER
using HardwareSerialRxHandler = void (*)(USART_TypeDef *, HardwareSerial *);
extern HardwareSerialRxHandler volatile hardware_serial_rx_handler;
#endif

template<unsigned Port> struct HardwareSerialBackend {
#if ENABLE_RX_BUFFER
  static HardwareSerialIrq state;
  static void receiveInterrupt() __attribute__((interrupt("WCH-Interrupt-fast"))) {
    HardwareSerial *obj = state.instance;
    if (obj) {
      hardware_serial_rx_handler(HardwareSerialPortTraits<Port>::peripheral(), obj);
    }
  }
#endif
  static HardwareSerialIrq *binding() {
#if ENABLE_RX_BUFFER
    HardwareSerialPortTraits<Port>::template bindVector<HardwareSerialBackend<Port>>();
    return &state;
#else
    return nullptr;
#endif
  }
};

#if ENABLE_RX_BUFFER
template<unsigned Port>
HardwareSerialIrq HardwareSerialBackend<Port>::state = {
  nullptr, HardwareSerialPortTraits<Port>::irq
};
#endif

// Optional typed construction preserves compatibility with HardwareSerial&.
// The port number selects both the peripheral and its variant pin defaults.
template<unsigned Port> class HardwareSerialPort : public HardwareSerial {
public:
  HardwareSerialPort()
    : HardwareSerial(HardwareSerialPortTraits<Port>::peripheral(),
                     HardwareSerialPortTraits<Port>::rx(),
                     HardwareSerialPortTraits<Port>::tx(),
                     HardwareSerialBackend<Port>::binding())
  {
  }
};

template<unsigned Port> HardwareSerial &hardware_serial_instance() {
  static HardwareSerial serial(HardwareSerialPortTraits<Port>::peripheral(),
                               HardwareSerialPortTraits<Port>::rx(),
                               HardwareSerialPortTraits<Port>::tx(),
                               HardwareSerialBackend<Port>::binding());
  return serial;
}

// Runtime peripheral constructors necessarily instantiate every candidate port.
// Keep this resolver and those constructors inline so unused APIs emit no ISRs.
inline HardwareSerialIrq *hardware_serial_irq_for(void *peripheral) {
#if defined(HAVE_HWSERIAL1) && defined(USART1)
  if (peripheral == HardwareSerialPortTraits<1>::peripheral()) {
    return HardwareSerialBackend<1>::binding();
  }
#endif
#if defined(HAVE_HWSERIAL2) && defined(USART2)
  if (peripheral == HardwareSerialPortTraits<2>::peripheral()) {
    return HardwareSerialBackend<2>::binding();
  }
#endif
#if defined(HAVE_HWSERIAL3) && defined(USART3)
  if (peripheral == HardwareSerialPortTraits<3>::peripheral()) {
    return HardwareSerialBackend<3>::binding();
  }
#endif
#if defined(HAVE_HWSERIAL4) && (defined(USART4) || defined(UART4))
  if (peripheral == HardwareSerialPortTraits<4>::peripheral()) {
    return HardwareSerialBackend<4>::binding();
  }
#endif
#if defined(HAVE_HWSERIAL5) && defined(UART5)
  if (peripheral == HardwareSerialPortTraits<5>::peripheral()) {
    return HardwareSerialBackend<5>::binding();
  }
#endif
#if defined(HAVE_HWSERIAL6) && defined(UART6)
  if (peripheral == HardwareSerialPortTraits<6>::peripheral()) {
    return HardwareSerialBackend<6>::binding();
  }
#endif
#if defined(HAVE_HWSERIAL7) && defined(UART7)
  if (peripheral == HardwareSerialPortTraits<7>::peripheral()) {
    return HardwareSerialBackend<7>::binding();
  }
#endif
#if defined(HAVE_HWSERIAL8) && defined(UART8)
  if (peripheral == HardwareSerialPortTraits<8>::peripheral()) {
    return HardwareSerialBackend<8>::binding();
  }
#endif
  return nullptr;
}

PinName hardware_serial_pin_for(void *peripheral, bool tx);

inline HardwareSerial::HardwareSerial(void *peripheral)
  : HardwareSerial(peripheral, hardware_serial_pin_for(peripheral, false),
                   hardware_serial_pin_for(peripheral, true))
{
}

inline HardwareSerial::HardwareSerial(void *peripheral, uint32_t rx, uint32_t tx)
  : HardwareSerial(peripheral, rx, tx, hardware_serial_irq_for(peripheral))
{
}

inline HardwareSerial::HardwareSerial(void *peripheral, PinName rx, PinName tx)
  : HardwareSerial(peripheral, rx, tx, hardware_serial_irq_for(peripheral))
{
}

inline void HardwareSerial::setHandler(void *handler)
{
  _serial.uart = static_cast<USART_TypeDef *>(handler);
#if ENABLE_RX_BUFFER
  _rx_irq = hardware_serial_irq_for(handler);
#endif
}

#endif // UART_MODULE_ENABLED && !UART_MODULE_ONLY
#endif // HARDWARE_SERIAL_PRIVATE_H
