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
#endif

#if defined(SPI_MODULE_ENABLED)
  #define SPI_INTERFACES_COUNT 1  // MMOLE: used by libraries such as Adafruit BusIO. Should be defined in some variants.h file somewhere
#endif

/* CH32V006F Pins */
#define PA0                     11
#define PA1                     PIN_A1
#define PA2                     PIN_A0
#define PA3                     14
#define PA4                     8
#define PA5                     9
#define PB0                     15
#define PB1                     16
#define PB3                     17
#define PC0                     19
#define PC1                     20
#define PC2                     0
#define PC4                     PIN_A2
#define PC5                     21
#define PD0                     13
#define PD1                     18
#define PD2                     PIN_A3
#define PD3                     PIN_A4 
#define PD4                     PIN_A7
#define PD5                     PIN_A5
#define PD6                     PIN_A6 
#define PD7                     7


// Alternate pins number
//#define PD5_ALT1                (PD5  | ALT1)
//#define PD6_ALT1                (PD6  | ALT1)


#define NUM_DIGITAL_PINS        22
#define NUM_ANALOG_INPUTS       8

// Shared initializers for runtime and compile-time pin conversion.
#define VARIANT_DIGITAL_PIN_MAP {        \
  PC_2,   /* D0/SCL */                   \
  PC_4,   /* D1/A2/SS */                 \
  PD_2,   /* D2/A3/SCK */                \
  PD_3,   /* D3/A4/MOSI */               \
  PD_4,   /* D4/A7 */                    \
  PD_5,   /* D5/A5/TX */                 \
  PD_6,   /* D6/A6/RX */                 \
  PD_7,   /* D7 */                       \
  PA_4,   /* D8/TX2 */                   \
  PA_5,   /* D9/RX2 */                   \
  PA_1,   /* D10 (shared with PA6)/A1 */ \
  PA_0,   /* D11 */                      \
  PA_2,   /* D12/A0 */                   \
  PD_0,   /* D13/ */                     \
  PA_3,   /* D14/ */                     \
  PB_0,   /* D15/ */                     \
  PB_1,   /* D16/ */                     \
  PB_3,   /* D17/MISO */                 \
  PD_1,   /* D18/SWIO */                 \
  PC_0,   /* D19 */                      \
  PC_1,   /* D20/SDA */                  \
  PC_5    /* D21/RST */                  \
/*                                       \
  PA_1,   // D0/A1                       \
  PA_2,   // D1/A0                       \
  PC_0,   // D2                          \
  PC_1,   // D3                          \
  PC_2,   // D4                          \
  PC_3,   // D5                          \
  PC_4,   // D6/A2                       \
  PC_5,   // D7                          \
  PC_6,   // D8                          \
  PC_7,   // D9                          \
  PD_0,   // D10                         \
  PD_1,   // D11/SWIO                    \
  PD_2,   // D12/A3                      \
  PD_3,   // D13/A4                      \
  PD_4,   // D14/A7                      \
  PD_5,   // D15/A5/TX                   \
  PD_6,   // D16/A6/RX                   \
  PD_7    // D17/RST                     \
*/                                       \
}

#define VARIANT_ANALOG_PIN_MAP { \
  12,    /* A0/PA2 */ \
  10,    /* A1/PA1 */ \
  1,    /* A2/PC4 */ \
  2,   /* A3/PD2 */ \
  3,   /* A4/PD3 */ \
  5,   /* A5/PD5 */ \
  6,   /* A6/PD6 */ \
  4    /* A7/PD4  */ \
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
// CH32V006:
//    RX: PD6=RX1, PA5=RX2_1
//    TX: PD5=TX1, PA4=TX2_1
// CH32V006E8 QSOP24 USART2 has RX/TX as alternative functions U2RX_1/U2TX_1 on PA5/PA4 (pins 16/15).
#ifndef BOARD_DEFAULT_SERIAL
  #define BOARD_DEFAULT_SERIAL 1
#endif

// Default pins for each hardware serial port.
#ifndef PIN_SERIAL1_RX
  #define PIN_SERIAL1_RX     PD6  /* UART1 default RX pin */
#endif
#ifndef PIN_SERIAL1_TX
  #define PIN_SERIAL1_TX     PD5  /* UART1 default TX pin */
#endif

#ifndef PIN_SERIAL2_RX
  #define PIN_SERIAL2_RX     PA5  /* UART2 default RX pin */
#endif
#ifndef PIN_SERIAL2_TX
  #define PIN_SERIAL2_TX     PA4  /* UART2 default TX pin */
#endif



// SPI definitions
#ifndef PIN_SPI_SS
  #define PIN_SPI_SS            PC4
#endif
#ifndef PIN_SPI_MOSI
  #define PIN_SPI_MOSI          PD3
#endif
#ifndef PIN_SPI_MISO
  #define PIN_SPI_MISO          PB3
#endif
#ifndef PIN_SPI_SCK
  #define PIN_SPI_SCK           PD2
#endif

// I2C definitions
#ifndef PIN_WIRE_SDA
  #define PIN_WIRE_SDA          PC1
#endif
#ifndef PIN_WIRE_SCL
  #define PIN_WIRE_SCL          PC2
#endif

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


