#ifndef CH32_SERIAL_PINS_H
#define CH32_SERIAL_PINS_H

// Included immediately after SerialRouting.h, before SerialN becomes an accessor
// macro. Resolve the final alias, including variant defaults and build overrides.
#define CH32_SERIAL_PORT_Serial1 1
#define CH32_SERIAL_PORT_Serial2 2
#define CH32_SERIAL_PORT_Serial3 3
#define CH32_SERIAL_PORT_Serial4 4
#define CH32_SERIAL_PORT_Serial5 5
#define CH32_SERIAL_PORT_Serial6 6
#define CH32_SERIAL_PORT_Serial7 7
#define CH32_SERIAL_PORT_Serial8 8
#define CH32_SERIAL_PORT_INNER(alias) CH32_SERIAL_PORT_##alias
#define CH32_SERIAL_PORT(alias) CH32_SERIAL_PORT_INNER(alias)

// USB CDC, a custom serial object, or no Serial alias has no inferred UART pins.
#if CH32_SERIAL_PORT(Serial) == 1
  #ifndef PIN_SERIAL_RX
    #define PIN_SERIAL_RX PIN_SERIAL1_RX
  #endif
  #ifndef PIN_SERIAL_TX
    #define PIN_SERIAL_TX PIN_SERIAL1_TX
  #endif
#elif CH32_SERIAL_PORT(Serial) == 2
  #ifndef PIN_SERIAL_RX
    #define PIN_SERIAL_RX PIN_SERIAL2_RX
  #endif
  #ifndef PIN_SERIAL_TX
    #define PIN_SERIAL_TX PIN_SERIAL2_TX
  #endif
#elif CH32_SERIAL_PORT(Serial) == 3
  #ifndef PIN_SERIAL_RX
    #define PIN_SERIAL_RX PIN_SERIAL3_RX
  #endif
  #ifndef PIN_SERIAL_TX
    #define PIN_SERIAL_TX PIN_SERIAL3_TX
  #endif
#elif CH32_SERIAL_PORT(Serial) == 4
  #ifndef PIN_SERIAL_RX
    #define PIN_SERIAL_RX PIN_SERIAL4_RX
  #endif
  #ifndef PIN_SERIAL_TX
    #define PIN_SERIAL_TX PIN_SERIAL4_TX
  #endif
#elif CH32_SERIAL_PORT(Serial) == 5
  #ifndef PIN_SERIAL_RX
    #define PIN_SERIAL_RX PIN_SERIAL5_RX
  #endif
  #ifndef PIN_SERIAL_TX
    #define PIN_SERIAL_TX PIN_SERIAL5_TX
  #endif
#elif CH32_SERIAL_PORT(Serial) == 6
  #ifndef PIN_SERIAL_RX
    #define PIN_SERIAL_RX PIN_SERIAL6_RX
  #endif
  #ifndef PIN_SERIAL_TX
    #define PIN_SERIAL_TX PIN_SERIAL6_TX
  #endif
#elif CH32_SERIAL_PORT(Serial) == 7
  #ifndef PIN_SERIAL_RX
    #define PIN_SERIAL_RX PIN_SERIAL7_RX
  #endif
  #ifndef PIN_SERIAL_TX
    #define PIN_SERIAL_TX PIN_SERIAL7_TX
  #endif
#elif CH32_SERIAL_PORT(Serial) == 8
  #ifndef PIN_SERIAL_RX
    #define PIN_SERIAL_RX PIN_SERIAL8_RX
  #endif
  #ifndef PIN_SERIAL_TX
    #define PIN_SERIAL_TX PIN_SERIAL8_TX
  #endif
#endif

#undef CH32_SERIAL_PORT
#undef CH32_SERIAL_PORT_INNER
#undef CH32_SERIAL_PORT_Serial1
#undef CH32_SERIAL_PORT_Serial2
#undef CH32_SERIAL_PORT_Serial3
#undef CH32_SERIAL_PORT_Serial4
#undef CH32_SERIAL_PORT_Serial5
#undef CH32_SERIAL_PORT_Serial6
#undef CH32_SERIAL_PORT_Serial7
#undef CH32_SERIAL_PORT_Serial8

#endif // CH32_SERIAL_PINS_H
