#ifndef WIRING_SERIAL_H
#define WIRING_SERIAL_H

#include "variant.h"
//#include "HardwareSerial.h"   // MxRD 261013: moved to below to allow guards for HAVE_HWSERIALx

#if defined(USE_TINYUSB)
// Discover the library for ordinary Serial sketches. Core compilation uses
// the internal header because Arduino omits library include paths for the core.
#if CH32_LIB_DISCOVERY_PHASE
#include <Adafruit_TinyUSB.h>
#else
#include "Adafruit_USBD_CDC.h"
#endif
#if defined(Serial)
  #undef Serial   // MxRD: undef Serial defined when Multiple Serial was selected
#endif
#define Serial SerialTinyUSB
#endif

#if defined(UART_MODULE_ENABLED) && !defined(UART_MODULE_ONLY)   // MxRD: UART_MODULE_ONLY seems never used

  #if !defined(HWSERIAL_NONE) && defined(SERIAL_UART_INSTANCE)

    #if SERIAL_UART_INSTANCE == 1
      #define ENABLE_HWSERIAL1
      #if !defined(Serial)
        #define Serial Serial1
        // #define serialEvent serialEvent1  //reserved
      #endif  
    #elif SERIAL_UART_INSTANCE == 2
      #define ENABLE_HWSERIAL2
      #if !defined(Serial)
        #define Serial Serial2
        // #define serialEvent serialEvent2
      #endif
    #elif SERIAL_UART_INSTANCE == 3
      #define ENABLE_HWSERIAL3
      #if !defined(Serial)
        #define Serial Serial3
        // #define serialEvent serialEvent3
      #endif
    #elif SERIAL_UART_INSTANCE == 4
      #define ENABLE_HWSERIAL4
      #if !defined(Serial)
        #define Serial Serial4
        // #define serialEvent serialEvent4
      #endif
    #elif SERIAL_UART_INSTANCE == 5
      #define ENABLE_HWSERIAL5
      #if !defined(Serial)
        #define Serial Serial5
        // #define serialEvent serialEvent5
      #endif
    #elif SERIAL_UART_INSTANCE == 6
      #define ENABLE_HWSERIAL6
      #if !defined(Serial)
        #define Serial Serial6
        // #define serialEvent serialEvent6
      #endif
    #elif SERIAL_UART_INSTANCE == 7
      #define ENABLE_HWSERIAL7
      #if !defined(Serial)
        #define Serial Serial7
        // #define serialEvent serialEvent7
      #endif
    #elif SERIAL_UART_INSTANCE == 8
      #define ENABLE_HWSERIAL8
      #if !defined(Serial)
        #define Serial Serial8
        // #define serialEvent serialEvent8
      #endif
    #else
      #error "Unsupported SERIAL_UART_INSTANCE: select a UART from 1 through 8, or disable Serial."

    #endif /* SERIAL_UART_INSTANCE == x */

  #endif /* !HWSERIAL_NONE && SERIAL_UART_INSTANCE */


  #if defined(ENABLE_HWSERIAL1)
    #if !defined(PIN_SERIAL1_RX) || !defined(PIN_SERIAL1_TX)
      #error "Serial1 is unsupported by this board: PIN_SERIAL1_RX and PIN_SERIAL1_TX must be defined."
    #elif !(defined(USART1_BASE))
      #error "Serial1 is unsupported by this MCU: UART peripheral is unavailable."
    #else
      #define HAVE_HWSERIAL1
    #endif
  #endif
  #if defined(ENABLE_HWSERIAL2)
    #if !defined(PIN_SERIAL2_RX) || !defined(PIN_SERIAL2_TX)
      #error "Serial2 is unsupported by this board: PIN_SERIAL2_RX and PIN_SERIAL2_TX must be defined."
    #elif !(defined(USART2_BASE))
      #error "Serial2 is unsupported by this MCU: UART peripheral is unavailable."
    #else
      #define HAVE_HWSERIAL2
    #endif
  #endif
  #if defined(ENABLE_HWSERIAL3)
    #if !defined(PIN_SERIAL3_RX) || !defined(PIN_SERIAL3_TX)
      #error "Serial3 is unsupported by this board: PIN_SERIAL3_RX and PIN_SERIAL3_TX must be defined."
    #elif !(defined(USART3_BASE))
      #error "Serial3 is unsupported by this MCU: UART peripheral is unavailable."
    #else
      #define HAVE_HWSERIAL3
    #endif
  #endif
  #if defined(ENABLE_HWSERIAL4)
    #if !defined(PIN_SERIAL4_RX) || !defined(PIN_SERIAL4_TX)
      #error "Serial4 is unsupported by this board: PIN_SERIAL4_RX and PIN_SERIAL4_TX must be defined."
    #elif !(defined(USART4_BASE) || defined(UART4_BASE))
      #error "Serial4 is unsupported by this MCU: UART peripheral is unavailable."
    #else
      #define HAVE_HWSERIAL4
    #endif
  #endif
  #if defined(ENABLE_HWSERIAL5)
    #if !defined(PIN_SERIAL5_RX) || !defined(PIN_SERIAL5_TX)
      #error "Serial5 is unsupported by this board: PIN_SERIAL5_RX and PIN_SERIAL5_TX must be defined."
    #elif !(defined(UART5_BASE))
      #error "Serial5 is unsupported by this MCU: UART peripheral is unavailable."
    #else
      #define HAVE_HWSERIAL5
    #endif
  #endif
  #if defined(ENABLE_HWSERIAL6)
    #if !defined(PIN_SERIAL6_RX) || !defined(PIN_SERIAL6_TX)
      #error "Serial6 is unsupported by this board: PIN_SERIAL6_RX and PIN_SERIAL6_TX must be defined."
    #elif !(defined(UART6_BASE))
      #error "Serial6 is unsupported by this MCU: UART peripheral is unavailable."
    #else
      #define HAVE_HWSERIAL6
    #endif
  #endif
  #if defined(ENABLE_HWSERIAL7)
    #if !defined(PIN_SERIAL7_RX) || !defined(PIN_SERIAL7_TX)
      #error "Serial7 is unsupported by this board: PIN_SERIAL7_RX and PIN_SERIAL7_TX must be defined."
    #elif !(defined(UART7_BASE))
      #error "Serial7 is unsupported by this MCU: UART peripheral is unavailable."
    #else
      #define HAVE_HWSERIAL7
    #endif
  #endif
  #if defined(ENABLE_HWSERIAL8)
    #if !defined(PIN_SERIAL8_RX) || !defined(PIN_SERIAL8_TX)
      #error "Serial8 is unsupported by this board: PIN_SERIAL8_RX and PIN_SERIAL8_TX must be defined."
    #elif !(defined(UART8_BASE))
      #error "Serial8 is unsupported by this MCU: UART peripheral is unavailable."
    #else
      #define HAVE_HWSERIAL8
    #endif
  #endif

#include "HardwareSerial.h"   // MxRD 261013: moved from above to allow guards for HAVE_HWSERIALx

#endif  // defined(UART_MODULE_ENABLED) && !defined(UART_MODULE_ONLY)

#endif /* WIRING_SERIAL_H */
