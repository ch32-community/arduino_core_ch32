#ifndef HARDWARE_SERIAL_PRIVATE_H
#define HARDWARE_SERIAL_PRIVATE_H

#include "../HardwareSerial.h"

struct HardwareSerialIrq {
  HardwareSerial * volatile instance;
  IRQn_Type irq;
};

HardwareSerialIrq *hardware_serial_irq_for(void *peripheral);

#if ENABLE_RX_BUFFER
// These helpers must expand in the per-port translation unit: its accessor
// extracts that archive member, and only then may its vector override startup.
// The assembly alias keeps vector resolution outside LTO weak-symbol folding.
// Its unique C entry point remains emitted with the target interrupt ABI.
using HardwareSerialRxHandler = void (*)(USART_TypeDef *, HardwareSerial *);
extern HardwareSerialRxHandler volatile hardware_serial_rx_handler;

#define DEFINE_HARDWARE_SERIAL_PORT(N, UART, IRQ_HANDLER, IRQ_NUMBER)    \
  static HardwareSerialIrq serial_irq = {nullptr, IRQ_NUMBER};           \
  HardwareSerialIrq *hardware_serial_irq##N() { return &serial_irq; }    \
  extern "C" void hardware_serial_rx_irq##N(void)                        \
    __attribute__((used, externally_visible))                            \
    __attribute__((interrupt("WCH-Interrupt-fast")));                    \
  extern "C" void hardware_serial_rx_irq##N(void) {                      \
    HardwareSerial *obj = serial_irq.instance;                           \
    if (obj) { hardware_serial_rx_handler(UART, obj); }                  \
  }                                                                      \
  __asm__(".global " #IRQ_HANDLER "\n.set " #IRQ_HANDLER                 \
          ", hardware_serial_rx_irq" #N "\n");                           \
  HardwareSerial &getSerial##N() {                                       \
    static HardwareSerial serial(UART, PIN_SERIAL##N##_RX,               \
                                 PIN_SERIAL##N##_TX, &serial_irq);       \
    return serial;                                                       \
  }
#else
#define DEFINE_HARDWARE_SERIAL_PORT(N, UART, IRQ_HANDLER, IRQ_NUMBER)    \
  HardwareSerialIrq *hardware_serial_irq##N() { return nullptr; }        \
  HardwareSerial &getSerial##N() {                                       \
    static HardwareSerial serial(UART, PIN_SERIAL##N##_RX,               \
                                 PIN_SERIAL##N##_TX, nullptr);           \
    return serial;                                                       \
  }
#endif

#endif // HARDWARE_SERIAL_PRIVATE_H
