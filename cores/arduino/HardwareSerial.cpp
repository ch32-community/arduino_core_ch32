/*
  HardwareSerial.cpp - Hardware serial library for Wiring
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

  Modified 23 November 2006 by David A. Mellis
  Modified 28 September 2010 by Mark Sproul
  Modified 14 August 2012 by Alarus
  Modified 3  December 2013 by Matthijs Kooijman
  Modified 1 may 2023 by TempersLee
  Modified 8 September 2025 by Jobit Joseph (Universal CH32 RX Buffer Support)
*/

#include <stdio.h>
#include "Arduino.h"
#include "HardwareSerial.h"
#include "HardwareSerial/HardwareSerial_private.h"

#if defined(UART_MODULE_ENABLED) && !defined(UART_MODULE_ONLY)

#if ENABLE_RX_BUFFER
// IRQ wrappers do not reference receive processing until begin() installs it.
HardwareSerialRxHandler volatile hardware_serial_rx_handler;

static void uart_rx_isr(USART_TypeDef *uart, HardwareSerial *obj) {
  if (USART_GetITStatus(uart, USART_IT_RXNE) != RESET) {
    USART_ClearITPendingBit(uart, USART_IT_RXNE);
    
    unsigned char c = USART_ReceiveData(uart);
    rx_buffer_index_t next_head = (obj->_rx_buffer_head + 1) % SERIAL_RX_BUFFER_SIZE;

    if (next_head != obj->_rx_buffer_tail) {
      obj->_rx_buffer[obj->_rx_buffer_head] = c;
      obj->_rx_buffer_head = next_head;
    }
  }
}

#endif // ENABLE_RX_BUFFER

HardwareSerial::HardwareSerial(void *peripheral, uint32_t rx, uint32_t tx,
                               HardwareSerialIrq *irq)
  : HardwareSerial(peripheral, digitalPinToPinName(rx), digitalPinToPinName(tx), irq)
{
}

HardwareSerial::HardwareSerial(void *peripheral, PinName rx, PinName tx,
                               HardwareSerialIrq *irq)
{
  _serial.uart = static_cast<USART_TypeDef *>(peripheral);
#if ENABLE_RX_BUFFER
  _rx_irq = irq;
  _rx_buffer_head = 0;
  _rx_buffer_tail = 0;
#else
  (void)irq;
#endif
  init(rx, tx);
}

void HardwareSerial::init(PinName _rx, PinName _tx, PinName _rts, PinName _cts)
{
  if (_rx == _tx) {
    _serial.pin_rx = NC;
  } else {
    _serial.pin_rx = _rx;
  }
  _serial.pin_tx = _tx;
  _serial.pin_rts = _rts;
  _serial.pin_cts = _cts;
}

// Public Methods //////////////////////////////////////////////////////////////
void HardwareSerial::begin(unsigned long baud, byte config)
{
  uint32_t databits = 0;
  uint32_t stopbits = 0;
  uint32_t parity = 0;

#if ENABLE_RX_BUFFER
  // Clear buffers for all CH32 series MCUs
  _rx_buffer_head = 0;
  _rx_buffer_tail = 0;
#endif

  // Manage databits
  switch (config & 0x03) {
    case 0x00:
      databits = 6;
      break;
    case 0x01:
      databits = 7;
      break;
    case 0x02:
      databits = 8;
      break;
    case 0x03:
      databits = 9;
      break;
    default:
      databits = 8;
      break;
  }

  if ((config & 0x30) == 0x30) {
    parity = USART_Parity_Odd;
  } else if ((config & 0x20) == 0x20) {
    parity = USART_Parity_Even;
  } else {
    parity = USART_Parity_No;
  }

  switch ( (config & 0x0C) >> 2 ) {
    case 0x00:
      stopbits = USART_StopBits_1;
      break;
    case 0x01:
      stopbits = USART_StopBits_0_5;
      break;
    case 0x02:
      stopbits = USART_StopBits_2;
      break;
    case 0x03:
      stopbits = USART_StopBits_1_5;
      break;
    default:
      stopbits = USART_StopBits_1;
      break;
  }

  switch (databits) 
  {
#ifdef USART_WordLength_6b
    case 6: 
      databits = USART_WordLength_6b;
      break;
#endif 
#ifdef USART_WordLength_7b
    case 7: 
      databits = USART_WordLength_7b;
      break;
#endif
    case 8:
      databits = USART_WordLength_8b;
      break;
    case 9:
      databits = USART_WordLength_9b;
      break;
    default:
    case 0:
      Error_Handler();
      break;
  }
  
  uart_init(&_serial, (uint32_t)baud, databits, parity, stopbits);

#if ENABLE_RX_BUFFER
  if (_rx_irq) {
    // Publish the callback and owner before enabling receive interrupts.
    hardware_serial_rx_handler = uart_rx_isr;
    _rx_irq->instance = this;
    USART_ITConfig(_serial.uart, USART_IT_RXNE, ENABLE);
    NVIC_SetPriority(_rx_irq->irq, 2);
    NVIC_EnableIRQ(_rx_irq->irq);
  }
#endif
}

void HardwareSerial::end()
{
#if ENABLE_RX_BUFFER
  if (_rx_irq && _rx_irq->instance == this) {
    NVIC_DisableIRQ(_rx_irq->irq);
    USART_ITConfig(_serial.uart, USART_IT_RXNE, DISABLE);
    _rx_irq->instance = nullptr;
  }
  _rx_buffer_head = _rx_buffer_tail;
#endif

  uart_deinit(&_serial);
}

int HardwareSerial::available(void)
{
#if ENABLE_RX_BUFFER
  // For all CH32 series MCUs: return proper buffer count
  return ((unsigned int)(SERIAL_RX_BUFFER_SIZE + _rx_buffer_head - _rx_buffer_tail)) % SERIAL_RX_BUFFER_SIZE;
#else
  // For other MCUs: keep original behavior
  return -1;
#endif
}

int HardwareSerial::peek(void)
{
#if ENABLE_RX_BUFFER
  // For all CH32 series MCUs: return next byte without removing it
  if (_rx_buffer_head == _rx_buffer_tail) {
    return -1;
  } else {
    return _rx_buffer[_rx_buffer_tail];
  }
#else
  // For other MCUs: keep original behavior
  return -1;
#endif
}

int HardwareSerial::read(void)
{
#if ENABLE_RX_BUFFER
  // For all CH32 series MCUs: read from buffer
  if (_rx_buffer_head == _rx_buffer_tail) {
    return -1;
  } else {
    unsigned char c = _rx_buffer[_rx_buffer_tail];
    _rx_buffer_tail = (rx_buffer_index_t)(_rx_buffer_tail + 1) % SERIAL_RX_BUFFER_SIZE;
    return c;
  }
#else
  // For other MCUs: keep original behavior
  unsigned char c;
  if(uart_getc(&_serial, &c) == 0){
    return c;
  }else{
    return -1;
  }
#endif
}

size_t HardwareSerial::write(const uint8_t *buffer, size_t size)
{

    for (size_t i = 0; i < size; i++) {
      write(buffer[i]);
    }

    return size;
}

size_t HardwareSerial::write(uint8_t c)
{
  return uart_putc(&_serial, c);
}

void HardwareSerial::setRx(uint32_t _rx)
{
  _serial.pin_rx = digitalPinToPinName(_rx);
}

void HardwareSerial::setTx(uint32_t _tx)
{
  _serial.pin_tx = digitalPinToPinName(_tx);
}

void HardwareSerial::setRx(PinName _rx)
{
  _serial.pin_rx = _rx;
}

void HardwareSerial::setTx(PinName _tx)
{
  _serial.pin_tx = _tx;
}

void HardwareSerial::setRts(uint32_t _rts)
{
  _serial.pin_rts = digitalPinToPinName(_rts);
}

void HardwareSerial::setCts(uint32_t _cts)
{
  _serial.pin_cts = digitalPinToPinName(_cts);
}

void HardwareSerial::setRtsCts(uint32_t _rts, uint32_t _cts)
{
  _serial.pin_rts = digitalPinToPinName(_rts);
  _serial.pin_cts = digitalPinToPinName(_cts);
}

void HardwareSerial::setRts(PinName _rts)
{
  _serial.pin_rts = _rts;
}

void HardwareSerial::setCts(PinName _cts)
{
  _serial.pin_cts = _cts;
}

void HardwareSerial::setRtsCts(PinName _rts, PinName _cts)
{
  _serial.pin_rts = _rts;
  _serial.pin_cts = _cts;
}

#endif // UART_MODULE_ENABLED && !UART_MODULE_ONLY
