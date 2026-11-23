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

/* CH32V002A4M. Used for compile-time package selection. */
#define WCH_MCU_CHIP_ID            0x00220600U

/* ENABLE Peripherals */
#ifndef IDE_MENU_PERIPHERALS   // defined when peripherals are enabled/disabled via the IDE menu
#define                         ADC_MODULE_ENABLED
#define                         UART_MODULE_ENABLED
//#define                         SPI_MODULE_ENABLED  // Board default is SPI disabled
#define                         I2C_MODULE_ENABLED
#define                         TIM_MODULE_ENABLED
#endif  // IDE_MENU_PERIPHERALS  

// CH32V002A4 Pins. Same as CH32V003A4
#define PA1                     0 //PIN_A1
#define PA2                     1 //PIN_A0
#define PC0                     2
#define PC1                     3
#define PC2                     4
#define PC3                     5
#define PC4                     6 //PIN_A2
#define PC6                     7
#define PC7                     8
#define PD1                     9
#define PD4                     10 //PIN_A7
#define PD5                     11 //PIN_A5
#define PD6                     12 //PIN_A6
#define PD7                     13

// Make sure A3/A4 are unknown enums, to generate errors when used.
// (see analogInputPin[] in variant_...cpp and pins_arduino_analog.h)
#define VARIANT_NO_ANALOG_A3
#define VARIANT_NO_ANALOG_A4

/* TODO: Define alternate pins like PC_0_ALT1 in PinNamesVar.h
         Then add pinmap entries for alternate pins.
         See V003 PinNamesVar.h and PeripheraPins.c for TIM mappings.
   Note: Having the additional definitions listed below, for notations like PC0_ALT1 require matching mappings and may be unwanted.

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
*/

#define NUM_DIGITAL_PINS        14
#define NUM_ANALOG_INPUTS       8


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


// Peripheral Selection: 
// SPI cannot be used with both I2C and UART. Board default is that SPI is not enabled. 
// When SPI is enabled, UART1_3 is the only usable pinout
// When I2C is enabled, UART1_3 cannot be used
#if defined(SPI_MODULE_ENABLED) && defined(I2C_MODULE_ENABLED) && defined(UART_MODULE_ENABLED) 
  #error "CH32V002A4 cannot have SPI enabled with both I2C and UART."
#elif (defined(SPI_MODULE_ENABLED) && defined(UART_MODULE_ENABLED) && defined(SERIAL_UART_PINMAP) && SERIAL_UART_PINMAP!=3)
  #error "CH32V002A4 select Serial1_3 when having SPI enabled."
  #define BOARD_DEFAULT_SERIALMAP 3   
  // Defined BOARD_DEFAULT_SERIALMAP to prevent further errors. User still needs to choose.
#elif defined(I2C_MODULE_ENABLED) && defined(UART_MODULE_ENABLED) && defined(SERIAL_UART_PINMAP) && SERIAL_UART_PINMAP==3
  #error "CH32V002A4 Serial1_3 requires I2C to be disabled"
#endif


// UART Definitions
// CH32V002 only has USART1. CH32V002A4M6 pins:
//    RX - TX : PD6/MOSI - PD5/MISO
//    RX'- TX': PC1/SDA - PC0
// CH32V002A4M6 SOP16 has RX1/TX1, but it is shared with SPI. 
// When SPI is enabled alternative pins UART_3 (RX'/TX') are used, but these conflict with I2C
// If I2C is enabled the default UART pins are used as board default, but user can override this. 
#if defined(SERIAL_UART_ENABLE_ALL)
  #error "CH32V002A4 only supports Serial1; Dual Serial is unavailable"
#endif
#if (defined(SERIAL_UART_INSTANCE) && SERIAL_UART_INSTANCE>1)
  #error "CH32V002A4 only supports Serial1; Serial2 cannot be selected"
#endif

#ifndef BOARD_DEFAULT_SERIAL
  #define BOARD_DEFAULT_SERIAL 1
#endif

// When I2C and SPI are disabled board default is no serial map.
// When I2C is enabled board default is no serial map.
// When SPI is enabled board default is serial map 3.
#ifndef BOARD_DEFAULT_SERIALMAP
  #if defined(SPI_MODULE_ENABLED)
    #define BOARD_DEFAULT_SERIALMAP 3   // use pin-remap UART_3
  #else
    #define BOARD_DEFAULT_SERIALMAP 0
  #endif
#endif

// Default pins for each hardware serial port.
#ifndef PIN_SERIAL1_RX
  #define PIN_SERIAL1_RX         PD6
#endif
#ifndef PIN_SERIAL1_TX
  #define PIN_SERIAL1_TX         PD5
#endif

// Mapped serial pins
// The VM00X datasheet shows 9 mappings for USART1 and 6 for USART2. To keep is short only a few are offered by the menu.
#ifndef PIN_SERIAL1_1_RX
  #define PIN_SERIAL1_1_RX     PD5  // UART1_1 pinmap RX pin
#endif
#ifndef PIN_SERIAL1_1_TX
  #define PIN_SERIAL1_1_TX     PD6  // UART1_1 pinmap TX pin 
#endif
#ifndef PIN_SERIAL1_3_RX
  #define PIN_SERIAL1_3_RX     PC1  // UART1_3 pinmap RX pin
#endif
#ifndef PIN_SERIAL1_3_TX
  #define PIN_SERIAL1_3_TX     PC0  // UART1_3 pinmap TX pin 
#endif


// SPI definitions
#ifndef PIN_SPI_SS
  //#define PIN_SPI_SS            PC4
  #define PIN_SPI_SS            PC7   // use pin-remap SPI_4 with software SS=PC7
#endif
#ifndef PIN_SPI_MOSI
  //#define PIN_SPI_MOSI          PC6
  #define PIN_SPI_MOSI          PD6   // use pin-remap SPI_4
#endif
#ifndef PIN_SPI_MISO
  //#define PIN_SPI_MISO          PC7
  #define PIN_SPI_MISO          PD5   // use pin-remap SPI_4
#endif
#ifndef PIN_SPI_SCK
  //#define PIN_SPI_SCK           PC5
  #define PIN_SPI_SCK           PD4   // use pin-remap SPI_4
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
