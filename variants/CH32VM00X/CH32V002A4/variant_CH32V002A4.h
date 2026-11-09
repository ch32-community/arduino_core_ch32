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
#define                         SPI_MODULE_ENABLED
#define                         I2C_MODULE_ENABLED
#define                         TIM_MODULE_ENABLED
#endif  // IDE_MENU_PERIPHERALS  

/* CH32V002F4 Pins - same as CH32V003F4 TSSOP20 (18x GPIO, 8+3 ADC, 1xSER, no OPA */
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

/*
#define PA0                     0         // N/A
#define PA1                     PIN_A1
#define PA2                     PIN_A0
#define PA3                     3         // N/A
#define PA4                     4          // N/A
#define PA5                     5         // N/A
#define PA6                     6         // N/A
#define PA7                     7          // N/A
#define PB0                     8             // N/A
#define PB1                     9         // N/A
#define PB2                     10         // N/A
#define PB3                     11          // N/A
#define PB4                     12          // N/A
#define PB5                     13         // N/A
#define PB6                     14          // N/A
#define PC0                     15
#define PC1                     16
#define PC2                     17
#define PC3                     18
#define PC4                     PIN_A2
#define PC5                     20
#define PC6                     21
#define PC7                     22
#define PD0                     23
#define PD1                     24
#define PD2                     PIN_A3
#define PD3                     PIN_A4 
#define PD4                     PIN_A7
#define PD5                     PIN_A5
#define PD6                     PIN_A6 
#define PD7                     30
*/

// Alternate pins number
#define PD5_ALT1                (PD5  | ALT1)
#define PD6_ALT1                (PD6  | ALT1)


#define NUM_DIGITAL_PINS        18
#define NUM_ANALOG_INPUTS       8

// Shared initializers for runtime and compile-time pin conversion.
#define VARIANT_DIGITAL_PIN_MAP {               \
  PA_1,   /* D0/A1 */                           \
  PA_2,   /* D1/A0 */                           \
  PC_0,   /* D2 */                              \
  PC_1,   /* D3 */                              \
  PC_2,   /* D4 */                              \
  PC_3,   /* D5 */                              \
  PC_4,   /* D6/A2 */                           \
  NC,     /* D7        No PC_5 on A4M6 */       \
  PC_6,   /* D8 */                              \
  PC_7,   /* D9 (recommended software CS/SS) */ \
  NC,     /* D10       No PD_0 on A4M6 */       \
  PD_1,   /* D11/SWIO */                        \
  NC,     /* D12/A3    No PD_2 on A4M6 */       \
  NC,     /* D13/A4    No PD_3 on A4M6 */       \
  PD_4,   /* D14/A7/SCK */                      \
  PD_5,   /* D15/A5/TX/MISO */                  \
  PD_6,   /* D16/A6/RX/MOSI */                  \
  PD_7    /* D17/RST */                         \
}

#define VARIANT_ANALOG_PIN_MAP { \
  1,    /* A0/PA2 */                            \
  0,    /* A1/PA1 */                            \
  6,    /* A2/PC4 */                            \
  12,   /* A3/PD2    No PD2 on A4M6 */          \
  13,   /* A4/PD3    No PD3 on A4M6 */          \
  15,   /* A5/PD5 */                            \
  16,   /* A6/PD6 */                            \
  14    /* A7/PD4  */                           \
}

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
// CH32V002 only has USART1. CH32V002A4M6 pins:
//    RX: PD6=RX1=MOSI
//    TX: PD5=TX1=MISO
// CH32V002A4M6 SOP16 has RX1/TX1, but it is shared with SPI. 
// When SPI is enabled alternative pins UART_3 (RX'/TX'=PC1/PC0) are used, but these conflict with I2C
// If I2C is enabled we cannot use these alternatives but there are no other combinations available.
// Using both I2C/SPI and UART is too much (but flash gets limited then also)
#if defined(SERIAL_UART_ENABLE_ALL)
  #error "CH32V002A4 only supports Serial1; Dual Serial is unavailable"
#endif

#ifndef BOARD_DEFAULT_SERIAL
  #define BOARD_DEFAULT_SERIAL 1
#endif

// Default pins for each hardware serial port.
#ifndef PIN_SERIAL1_RX
  #if defined(I2C_MODULE_ENABLED)
    #define PIN_SERIAL1_RX         PD6
  #else  
    #define PIN_SERIAL1_RX         PC1   // use pin-remap UART_3
  #endif // I2C_MODULE_ENABLED
  #endif
  #ifndef PIN_SERIAL1_TX
  #if defined(I2C_MODULE_ENABLED)
    #define PIN_SERIAL1_TX         PD5
  #else  
    #define PIN_SERIAL1_TX         PC0   // use pin-remap UART_3
  #endif // I2C_MODULE_ENABLED
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


