
#pragma once

#include VARIANT_H

// Preserve the board menu value; resolve Board Default to a hardware port.
#if defined(SERIAL_UART_INSTANCE)
  #if SERIAL_UART_INSTANCE == 0
    #undef SERIAL_UART_INSTANCE
    #if defined(BOARD_DEFAULT_SERIAL)
      #define SERIAL_UART_INSTANCE BOARD_DEFAULT_SERIAL
    #else
      #define SERIAL_UART_INSTANCE 1
    #endif
  #endif
#endif

// Compatibility pin aliases use the same port as WSerial.h.
#define SERIAL_PIN_NAME_EXPAND(port, direction) PIN_SERIAL##port##_##direction
#define SERIAL_PIN_NAME(port, direction) SERIAL_PIN_NAME_EXPAND(port, direction)
#if defined(SERIAL_UART_INSTANCE) && SERIAL_UART_INSTANCE > 0
    #define PIN_SERIAL_RX SERIAL_PIN_NAME(SERIAL_UART_INSTANCE, RX)
    #define PIN_SERIAL_TX SERIAL_PIN_NAME(SERIAL_UART_INSTANCE, TX)
#endif
