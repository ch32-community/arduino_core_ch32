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
#ifndef IDE_MENU_PERIPHERALS   // MxRD: defined when peripherals are enabled/disabled via the IDE menu
#define                         ADC_MODULE_ENABLED
#define                         UART_MODULE_ENABLED
#define                         SPI_MODULE_ENABLED
#define                         I2C_MODULE_ENABLED
#define                         TIM_MODULE_ENABLED
#endif

/* CH32V003F4 Pins */
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

// Alternate pins number
#define PD5_ALT1                (PD5  | ALT1)
#define PD6_ALT1                (PD6  | ALT1)

// Alternate pin numbers
#define PC0_ALT1                (PC0  | ALT1)
#define PC1_ALT1                (PC1  | ALT1)
#define PC2_ALT1                (PC2  | ALT1)
#define PC3_ALT1                (PC3  | ALT1)
#define PC4_ALT1                (PC4  | ALT1)
#define PC6_ALT1                (PC6  | ALT1)
#define PC7_ALT1                (PC7  | ALT1)
#define PD1_ALT1                (PD1  | ALT1)

#define PA1_ALT2                (PA1  | ALT2)
#define PA2_ALT2                (PA2  | ALT2)
#define PC0_ALT2                (PC0  | ALT2)
#define PC1_ALT2                (PC1  | ALT2)
#define PC3_ALT2                (PC3  | ALT2)
#define PC4_ALT2                (PC4  | ALT2)
#define PD1_ALT2                (PD1  | ALT2)
#define PD7_ALT2                (PD7  | ALT2)

#define PC1_ALT3                (PC1  | ALT3)
#define PC3_ALT3                (PC3  | ALT3)
#define PC4_ALT3                (PC4  | ALT3)
#define PC6_ALT3                (PC6  | ALT3)
#define PC7_ALT3                (PC7  | ALT3)
#define PD4_ALT3                (PD4  | ALT3)
#define PD5_ALT3                (PD5  | ALT3)
#define PD6_ALT3                (PD6  | ALT3)

#define PC7_ALT4                (PC7  | ALT4)




#define NUM_DIGITAL_PINS        18
#define NUM_ANALOG_INPUTS       8

// #define ADC_CTLR_ADCAL          
#define ADC_RESOLUTION          10



// On-board LED pin number
#ifndef LED_BUILTIN
  #define LED_BUILTIN           PNUM_NOT_DEFINED
#endif



// On-board user button
#ifndef USER_BTN
  #define USER_BTN              PNUM_NOT_DEFINED
#endif



// UART Definitions
#ifndef BOARD_DEFAULT_SERIAL
  #define BOARD_DEFAULT_SERIAL 1
#endif

#if defined(I2C_MODULE_ENABLED) && defined(SERIAL_UART_PINMAP) && SERIAL_UART_PINMAP==3
  #error "CH32V003F4 Serial1_3 requires I2C to be disabled"
#endif

// Default pins for hardware serial port 1.
#ifndef PIN_SERIAL1_RX
  #define PIN_SERIAL1_RX         PD6
#endif
#ifndef PIN_SERIAL1_TX
  #define PIN_SERIAL1_TX         PD5
#endif

// Useful mapped pins for each hardware serial port.
#ifndef PIN_SERIAL1_2_RX
  #define PIN_SERIAL1_2_RX     PD5  /* UART1_2 pinmap RX pin */
#endif
#ifndef PIN_SERIAL1_2_TX
  #define PIN_SERIAL1_2_TX     PD6  /* UART1_2 pinmap TX pin */
#endif

#ifndef PIN_SERIAL1_3_RX
  #define PIN_SERIAL1_3_RX     PC1  /* UART1_3 pinmap RX pin */
#endif
#ifndef PIN_SERIAL1_3_TX
  #define PIN_SERIAL1_3_TX     PC0  /* UART1_3 pinmap TX pin */
#endif



// SPI definitions
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

// I2C definitions
#ifndef PIN_WIRE_SDA
  #define PIN_WIRE_SDA          PC1
#endif
#ifndef PIN_WIRE_SCL
  #define PIN_WIRE_SCL          PC2
#endif

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

