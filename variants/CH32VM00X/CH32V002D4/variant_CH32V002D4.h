/*
 *******************************************************************************
 * Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
 * All rights reserved.
 *
 * This software component is licensed by WCH under BSD 3-Clause license,
 * the "License"; You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 *                        opensource.org/licenses/BSD-3-Clause
 *
 *******************************************************************************
 */
#pragma once

/* ENABLE Peripherals */
#ifndef IDE_MENU_PERIPHERALS   // defined when peripherals are enabled/disabled via the IDE menu
#define                         ADC_MODULE_ENABLED
#define                         UART_MODULE_ENABLED
//#define                         SPI_MODULE_ENABLED  // SPI not supported on QFN12
#define                         I2C_MODULE_ENABLED
#define                         TIM_MODULE_ENABLED
#endif

/* CH32V002D4 Pins - subset of CH32V002F4 TSSOP20 (18x GPIO, 8+3 ADC, 1xSER, no OPA */
#define PA1                     PIN_A1
#define PA2                     PIN_A0
#define PC0                     2
#define PC3                     3
#define PC4                     PIN_A2
#define PC6                     5
#define PC7                     6
#define PD0                     7
#define PD1                     8
#define PD4                     PIN_A3
#define PD7                     10

/* CH32V002F4:
#define PA1                     PIN_A1
#define PA2                     PIN_A0
#define PC0                     2
#define PC1                     3
#define PC2                     4
#define PC3                     5 
#define PC4                     PIN_A2
#define PC5                     7
#define PC6                     8
#define PC7                     9
#define PD0                     10
#define PD1                     11
#define PD2                     PIN_A3
#define PD3                     PIN_A4 
#define PD4                     PIN_A7
#define PD5                     PIN_A5
#define PD6                     PIN_A6 
#define PD7                     17
*/

#define NUM_DIGITAL_PINS        11 // 18
#define NUM_ANALOG_INPUTS       4  // 8

// #define ADC_CTLR_ADCAL          
#define ADC_RESOLUTION          12



// On-board LED pin number
#ifndef LED_BUILTIN
  #define LED_BUILTIN           PNUM_NOT_DEFINED
#endif



// On-board user button
#ifndef USER_BTN
  #define USER_BTN              PNUM_NOT_DEFINED
#endif



// UART Definitions
// CH32V002 only has USART1. CH32V002D4U6 pins:
//    RX: PD1=RX1=SCL=SWIO (!)
//    TX: PD0=TX1=SDA
// CH32V002D4U6 QFN12 has RX1/TX1, but beware: RX is shared with SWIO! Use "power-off clear" in Link Utility to program after bricking
#ifndef SERIAL_UART_INSTANCES
  #define SERIAL_UART_INSTANCES 1
#endif

#if (SERIAL_UART_INSTANCES == 1)
  //  Using only one Serial port
  #ifndef SERIAL_UART_INSTANCE
    #define SERIAL_UART_INSTANCE 1   // use Serial1 as default for Serial
  #endif
#else
  // Using multple Serial ports
  // When having multiple Serial instances SERIAL_UART_INSTANCE cannot be defined as it will skip defining the other ports.
  #undef SERIAL_UART_INSTANCE
  #define ENABLE_HWSERIAL1 1
  //#define ENABLE_HWSERIAL2 1
  #define Serial Serial1
  #error No Serial2 on CH32V002 (do not select Dual Serial)
  // Note: cannot use single quote ' in #error
#endif

// Default pin used for generic 'Serial' instance
// Mandatory for Firmata, no longer used by HardwareSerial.
#if SERIAL_UART_INSTANCE==1 || SERIAL_UART_INSTANCES>1
    #ifndef PIN_SERIAL_RX
        #define PIN_SERIAL_RX     PD1
    #endif
    #ifndef PIN_SERIAL_TX
        #define PIN_SERIAL_TX     PD0
    #endif
#elif  SERIAL_UART_INSTANCE==2
    #error No Serial2 on CH32V002 (select Serial1 instead)
#endif



// SPI definitions
#if defined(SPI_MODULE_ENABLED)
#error CH32V002D4 QFN12 has no pins to support SPI
/*
#ifndef PIN_SPI_SS
  #define PIN_SPI_SS            PC4
#endif
#ifndef PIN_SPI_MOSI
  #define PIN_SPI_MOSI          PC6
#endif
#ifndef PIN_SPI_MISO
  #define PIN_SPI_MISO          PC7
#endif
#ifndef PIN_SPI_SCK
  #define PIN_SPI_SCK           PC5
#endif
*/
#endif // SPI_MODULE_ENABLED

// I2C definitions
#if defined(I2C_MODULE_ENABLED)
#ifndef PIN_WIRE_SDA
  #define PIN_WIRE_SDA          PD0
#endif
#ifndef PIN_WIRE_SCL
  #define PIN_WIRE_SCL          PD1
#endif
#endif // I2C_MODULE_ENABLED

// Timer Definitions
#ifndef TIMER_TONE
  #define TIMER_TONE            TIM2
#endif
#ifndef TIMER_SERVO
  #define TIMER_SERVO           TIM1
#endif

/*----------------------------------------------------------------------------
 *        Arduino objects - C++ only
 *----------------------------------------------------------------------------*/

#ifdef __cplusplus
  // These serial port names are intended to allow libraries and architecture-neutral
  // sketches to automatically default to the correct port name for a particular type
  // of use.  For example, a GPS module would normally connect to SERIAL_PORT_HARDWARE_OPEN,
  // the first hardware serial port whose RX/TX pins are not dedicated to another use.
  //
  // SERIAL_PORT_MONITOR        Port which normally prints to the Arduino Serial Monitor
  //
  // SERIAL_PORT_USBVIRTUAL     Port which is USB virtual serial
  //
  // SERIAL_PORT_LINUXBRIDGE    Port which connects to a Linux system via Bridge library
  //
  // SERIAL_PORT_HARDWARE       Hardware serial port, physical RX & TX pins.
  //
  // SERIAL_PORT_HARDWARE_OPEN  Hardware serial ports which are open for use.  Their RX & TX
  //                            pins are NOT connected to anything by default.
  #ifndef SERIAL_PORT_MONITOR
    #define SERIAL_PORT_MONITOR   Serial
  #endif
  #ifndef SERIAL_PORT_HARDWARE
    #define SERIAL_PORT_HARDWARE  Serial
  #endif
#endif


