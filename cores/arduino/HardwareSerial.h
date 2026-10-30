/*
  HardwareSerial.h - Hardware serial library for Wiring
  Copyright (c) 2006 Nicholas Zambetti.  All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

  Modified 28 September 2010 by Mark Sproul
  Modified 14 August 2012 by Alarus
  Modified 3 December 2013 by Matthijs Kooijman
  Modified 1 may 2023 by TempersLee
  Modified 8 September 2025 by Jobit Joseph (Universal CH32 RX Buffer Support)
  Modified 13 September 2026 by MxRD (compile-time guards for Serial selection)
*/

#ifndef HardwareSerial_h
#define HardwareSerial_h

#include <inttypes.h>
#include "Stream.h"
#include "uart.h"

// Per-port pin overrides are only valid for UARTs exposed by the selected
// variant. Catch misspelled/nonexistent port selections at compile time.
#if (defined(PIN_SERIAL1_RX) || defined(PIN_SERIAL1_TX)) && !defined(HAVE_HWSERIAL1)
  #error "PIN_SERIAL1_RX/TX configured, but this board does not expose Serial1"
#endif
#if (defined(PIN_SERIAL2_RX) || defined(PIN_SERIAL2_TX)) && !defined(HAVE_HWSERIAL2)
  #error "PIN_SERIAL2_RX/TX configured, but this board does not expose Serial2"
#endif
#if (defined(PIN_SERIAL3_RX) || defined(PIN_SERIAL3_TX)) && !defined(HAVE_HWSERIAL3)
  #error "PIN_SERIAL3_RX/TX configured, but this board does not expose Serial3"
#endif
#if (defined(PIN_SERIAL4_RX) || defined(PIN_SERIAL4_TX)) && !defined(HAVE_HWSERIAL4)
  #error "PIN_SERIAL4_RX/TX configured, but this board does not expose Serial4"
#endif
#if (defined(PIN_SERIAL5_RX) || defined(PIN_SERIAL5_TX)) && !defined(HAVE_HWSERIAL5)
  #error "PIN_SERIAL5_RX/TX configured, but this board does not expose Serial5"
#endif
#if (defined(PIN_SERIAL6_RX) || defined(PIN_SERIAL6_TX)) && !defined(HAVE_HWSERIAL6)
  #error "PIN_SERIAL6_RX/TX configured, but this board does not expose Serial6"
#endif
#if (defined(PIN_SERIAL7_RX) || defined(PIN_SERIAL7_TX)) && !defined(HAVE_HWSERIAL7)
  #error "PIN_SERIAL7_RX/TX configured, but this board does not expose Serial7"
#endif
#if (defined(PIN_SERIAL8_RX) || defined(PIN_SERIAL8_TX)) && !defined(HAVE_HWSERIAL8)
  #error "PIN_SERIAL8_RX/TX configured, but this board does not expose Serial8"
#endif
#if (defined(PIN_SERIAL1_RX) != defined(PIN_SERIAL1_TX)) || \
    (defined(PIN_SERIAL2_RX) != defined(PIN_SERIAL2_TX)) || \
    (defined(PIN_SERIAL3_RX) != defined(PIN_SERIAL3_TX)) || \
    (defined(PIN_SERIAL4_RX) != defined(PIN_SERIAL4_TX)) || \
    (defined(PIN_SERIAL5_RX) != defined(PIN_SERIAL5_TX)) || \
    (defined(PIN_SERIAL6_RX) != defined(PIN_SERIAL6_TX)) || \
    (defined(PIN_SERIAL7_RX) != defined(PIN_SERIAL7_TX)) || \
    (defined(PIN_SERIAL8_RX) != defined(PIN_SERIAL8_TX))
  #error "Define both PIN_SERIALn_RX and PIN_SERIALn_TX for a serial pin override"
#endif

// Define constants and variables for buffering incoming serial data.  We're
// using a ring buffer (I think), in which head is the index of the location
// to which to write the next incoming character and tail is the index of the
// location from which to read.
// NOTE: a "power of 2" buffer size is recommended to dramatically
//       optimize all the modulo operations for ring buffers.
// WARNING: When buffer sizes are increased to > 256, the buffer index
// variables are automatically increased in size, but the extra
// atomicity guards needed for that are not implemented. This will
// often work, but occasionally a race condition can occur that makes
// Serial behave erratically. See https://github.com/arduino/Arduino/issues/2405
#if !defined(SERIAL_TX_BUFFER_SIZE)
  #define SERIAL_TX_BUFFER_SIZE 64
#endif
#if !defined(SERIAL_RX_BUFFER_SIZE)
  #define SERIAL_RX_BUFFER_SIZE 64
#endif
#if (SERIAL_TX_BUFFER_SIZE>256)
  typedef uint16_t tx_buffer_index_t;
#else
  typedef uint8_t tx_buffer_index_t;
#endif
#if  (SERIAL_RX_BUFFER_SIZE>256)
  typedef uint16_t rx_buffer_index_t;
#else
  typedef uint8_t rx_buffer_index_t;
#endif

// A bool should be enough for this
// But it brings an build error due to ambiguous
// call of overloaded HardwareSerial(int, int)
// So defining a dedicated type
typedef enum {
  HALF_DUPLEX_DISABLED,
  HALF_DUPLEX_ENABLED
} HalfDuplexMode_t;

// Define config for Serial.begin(baud, config);
/* databits 8 */
#define SERIAL_8N1      0x02     
#define SERIAL_8N2      0x0A
#define SERIAL_8N0_5    0x06
#define SERIAL_8N1_5    0x0E
#define SERIAL_8E1      0x22
#define SERIAL_8E2      0x2A
#define SERIAL_8E0_5    0x26
#define SERIAL_8E1_5    0x2E
#define SERIAL_8O1      0x32
#define SERIAL_8O2      0x3A
#define SERIAL_8O0_5    0x36
#define SERIAL_8O1_5    0x3E
/* databits 9 */
#define SERIAL_9N1      0x03 
#define SERIAL_9N2      0x0B
#define SERIAL_9N0_5    0x07
#define SERIAL_9N1_5    0x0F
#define SERIAL_9E1      0x23
#define SERIAL_9E2      0x2B
#define SERIAL_9E0_5    0x27 
#define SERIAL_9E1_5    0x2F
#define SERIAL_9O1      0x33 
#define SERIAL_9O2      0x3B
#define SERIAL_9O0_5    0x37
#define SERIAL_9O1_5    0x3F  

// Enable RX buffering for all CH32 series MCUs
#if 1 // in future releases this may become conditional for processors with large enough hardware buffer
// was #if defined(CH32V003) || defined(CH32V203) || defined(CH32X035) || defined(CH32V103) || defined(CH32V307)
  #define ENABLE_RX_BUFFER 1
#else
  #define ENABLE_RX_BUFFER 0
#endif

struct HardwareSerialIrq;

class HardwareSerial : public Stream {

  serial_t _serial;
#if ENABLE_RX_BUFFER
  HardwareSerialIrq *_rx_irq;
#endif
public:
    HardwareSerial(void *peripheral);
    HardwareSerial(void *peripheral, uint32_t rx, uint32_t tx);
    HardwareSerial(void *peripheral, PinName rx, PinName tx);
    // Internal port factories bind their IRQ without referencing other ports.
    HardwareSerial(void *peripheral, uint32_t rx, uint32_t tx, HardwareSerialIrq *irq);
    HardwareSerial(void *peripheral, PinName rx, PinName tx, HardwareSerialIrq *irq);
    
    void begin(unsigned long baud)
    {
      begin(baud, SERIAL_8N1);     //SERIAL_9E1_5  SERIAL_8N1
    }
    void begin(unsigned long, uint8_t);
    void end();

    virtual int available(void);
    virtual int peek(void);
    virtual int read(void);

    virtual size_t write(uint8_t);
    inline size_t write(unsigned long n)
    {
      return write((uint8_t)n);
    }
    inline size_t write(long n)
    {
      return write((uint8_t)n);
    }
    inline size_t write(unsigned int n)
    {
      return write((uint8_t)n);
    }
    inline size_t write(int n)
    {
      return write((uint8_t)n);
    }
    size_t write(const uint8_t *buffer, size_t size);
    using Print::write; // pull in write(str) from Print
    operator bool()
    {
      return true;
    }

    void setRx(uint32_t _rx);
    void setTx(uint32_t _tx);
    void setRx(PinName _rx);
    void setTx(PinName _tx);

    // Enable HW flow control on RTS, CTS or both
    void setRts(uint32_t _rts);
    void setCts(uint32_t _cts);
    void setRtsCts(uint32_t _rts, uint32_t _cts);
    void setRts(PinName _rts);
    void setCts(PinName _cts);
    void setRtsCts(PinName _rts, PinName _cts);
    void setHandler(void *handler);

#if ENABLE_RX_BUFFER
public:
    // RX Buffer variables - for all CH32 series MCUs
    volatile rx_buffer_index_t _rx_buffer_head;
    volatile rx_buffer_index_t _rx_buffer_tail;
    unsigned char _rx_buffer[SERIAL_RX_BUFFER_SIZE];
#endif

  private:
    void init(PinName _rx, PinName _tx, PinName _rts = NC, PinName _cts = NC);
};

// Preserve SerialN.method(), &SerialN and Stream/Print references while creating
// each port on first use. These are expressions, not external object symbols.
#if defined(HAVE_HWSERIAL1) && defined(USART1)
  HardwareSerial &getSerial1();
  #define Serial1 getSerial1()
#endif
#if defined(HAVE_HWSERIAL2) && defined(USART2)
  HardwareSerial &getSerial2();
  #define Serial2 getSerial2()
#endif
#if defined(HAVE_HWSERIAL3) && defined(USART3)
  HardwareSerial &getSerial3();
  #define Serial3 getSerial3()
#endif
#if defined(HAVE_HWSERIAL4) && (defined(UART4) || defined(USART4))   // Note: some have USART4, V30x has UART4
  HardwareSerial &getSerial4();
  #define Serial4 getSerial4()
#endif
#if defined(HAVE_HWSERIAL5) && (defined(UART5) || defined(USART5))   // Note: V30x has UART 4-8, no USART5-8 in supported series (even though mentioned in various GPIO/remap sources)
  HardwareSerial &getSerial5();
  #define Serial5 getSerial5()
#endif
#if defined(HAVE_HWSERIAL6) && (defined(UART6) || defined(USART6))
  HardwareSerial &getSerial6();
  #define Serial6 getSerial6()
#endif
#if defined(HAVE_HWSERIAL7) && (defined(UART7) || defined(USART7))
  HardwareSerial &getSerial7();
  #define Serial7 getSerial7()
#endif
#if defined(HAVE_HWSERIAL8) && (defined(UART8) || defined(USART8))
  HardwareSerial &getSerial8();
  #define Serial8 getSerial8()
#endif

#endif   // HardwareSerial_h
