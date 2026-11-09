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
#define                         ADC_MODULE_ENABLED
#define                         UART_MODULE_ENABLED
#define                         SPI_MODULE_ENABLED
#define                         I2C_MODULE_ENABLED
#define                         TIM_MODULE_ENABLED

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


#define NUM_DIGITAL_PINS        18
#define NUM_ANALOG_INPUTS       8

// Shared initializers for runtime and compile-time pin conversion.
#define VARIANT_DIGITAL_PIN_MAP { \
  PA_1,   /* D0/A1    */          \
  PA_2,   /* D1/A0 */             \
  PC_0,   /* D2 */                \
  PC_1,   /* D3 */                \
  PC_2,   /* D4 */                \
  PC_3,   /* D5  */               \
  PC_4,   /* D6/A2 */             \
  PC_5,   /* D7 */                \
  PC_6,   /* D8 */                \
  PC_7,   /* D9 */                \
  PD_0,   /* D10 */               \
  PD_1,   /* D11 */               \
  PD_2,   /* D12/A3 */            \
  PD_3,   /* D13/A4 */            \
  PD_4,   /* D14/A7 */            \
  PD_5,   /* D15/A5 */            \
  PD_6,   /* D16/A6   */          \
  PD_7    /* D17 */               \
}

#define VARIANT_ANALOG_PIN_MAP { \
  1,    /* A0/PA2 */              \
  0,    /* A1/PA1 */              \
  6,    /* A2/PC4 */              \
  12,   /* A3/PD2 */              \
  13,   /* A4/PD3 */              \
  15,   /* A5/PD5 */              \
  16,   /* A6/PD6 */              \
  14    /* A7/PD4  */             \
}

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
  #define PIN_SERIAL1_2_TX     PD6  /* UART1_3 pinmap TX pin */
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


