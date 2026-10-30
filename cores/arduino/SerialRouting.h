#ifndef CH32_SERIAL_ROUTING_H
#define CH32_SERIAL_ROUTING_H

// Applied after the variant defaults, in both C and C++ translation units.
// 0: board default, -1: USB CDC, 1..8: hardware UART.
#if defined(SERIAL_UART_INSTANCE) && SERIAL_UART_INSTANCE != 0
  #undef Serial
  #if SERIAL_UART_INSTANCE == -1
    #if !defined(USE_TINYUSB)
      #error "Serial routing to USB CDC requires enabling TinyUSB in the USB menu"
    #endif
    #define Serial SerialTinyUSB
  #elif SERIAL_UART_INSTANCE == 1
    #if !defined(HAVE_HWSERIAL1)
      #error "Selected Serial1 is not supported by this board variant"
    #endif
    #define Serial Serial1
  #elif SERIAL_UART_INSTANCE == 2
    #if !defined(HAVE_HWSERIAL2)
      #error "Selected Serial2 is not supported by this board variant"
    #endif
    #define Serial Serial2
  #elif SERIAL_UART_INSTANCE == 3
    #if !defined(HAVE_HWSERIAL3)
      #error "Selected Serial3 is not supported by this board variant"
    #endif
    #define Serial Serial3
  #elif SERIAL_UART_INSTANCE == 4
    #if !defined(HAVE_HWSERIAL4)
      #error "Selected Serial4 is not supported by this board variant"
    #endif
    #define Serial Serial4
  #elif SERIAL_UART_INSTANCE == 5
    #if !defined(HAVE_HWSERIAL5)
      #error "Selected Serial5 is not supported by this board variant"
    #endif
    #define Serial Serial5
  #elif SERIAL_UART_INSTANCE == 6
    #if !defined(HAVE_HWSERIAL6)
      #error "Selected Serial6 is not supported by this board variant"
    #endif
    #define Serial Serial6
  #elif SERIAL_UART_INSTANCE == 7
    #if !defined(HAVE_HWSERIAL7)
      #error "Selected Serial7 is not supported by this board variant"
    #endif
    #define Serial Serial7
  #elif SERIAL_UART_INSTANCE == 8
    #if !defined(HAVE_HWSERIAL8)
      #error "Selected Serial8 is not supported by this board variant"
    #endif
    #define Serial Serial8
  #else
    #error "Invalid SERIAL_UART_INSTANCE: use -1, 0, or a supported UART number"
  #endif
#endif

#endif // CH32_SERIAL_ROUTING_H
