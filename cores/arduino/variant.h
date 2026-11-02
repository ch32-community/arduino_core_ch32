
#pragma once

#include VARIANT_H

// Compatibility aliases. Board/USB defaults use the Serial1 hardware pins.
#define SERIAL_PIN_NAME_EXPAND(port, direction) PIN_SERIAL##port##_##direction
#define SERIAL_PIN_NAME(port, direction) SERIAL_PIN_NAME_EXPAND(port, direction)

#ifndef SERIAL_UART_INSTANCE
    #error SERIAL_UART_INSTANCE is required to be defined.
#else
    #if defined(BOARD_DEFAULT_SERIAL) && SERIAL_UART_INSTANCE == 0
        #define PIN_SERIAL_RX SERIAL_PIN_NAME(BOARD_DEFAULT_SERIAL, RX)
        #define PIN_SERIAL_TX SERIAL_PIN_NAME(BOARD_DEFAULT_SERIAL, TX)
    #elif SERIAL_UART_INSTANCE > 0
        #define PIN_SERIAL_RX SERIAL_PIN_NAME(SERIAL_UART_INSTANCE, RX)
        #define PIN_SERIAL_TX SERIAL_PIN_NAME(SERIAL_UART_INSTANCE, TX)
    #endif
#endif
