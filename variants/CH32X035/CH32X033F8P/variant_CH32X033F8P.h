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

/* CH32VX033F8P6 Pins */
#define PA0                     PIN_A0
#define PA1                     PIN_A1
#define PA2                     PIN_A2
#define PA3                     PIN_A3
#define PA4                     PIN_A4
#define PA5                     PIN_A5
#define PA6                     PIN_A6
#define PA7                     PIN_A8
#define PB0                     PIN_A8
#define PB1                     PIN_A9
#define PB7                     9       // can be configured as RST
#define PC16                    10
#define PC17                    11
#define PA9                     12
#define PA11                    13
#define PA10                    14
#define PC3                     15      // PIN_A13
#define PC18                    16
#define PC19                    17

// Alternate pins number TODO
#define PA0_ALT1                (PA0  | ALT1)
#define PA1_ALT1                (PA1  | ALT1)
#define PA2_ALT1                (PA2  | ALT1)
#define PA3_ALT1                (PA3  | ALT1)
#define PA4_ALT1                (PA4  | ALT1)
#define PA5_ALT1                (PA5  | ALT1)
#define PA6_ALT1                (PA6  | ALT1)
#define PA7_ALT1                (PA7  | ALT1)
#define PB0_ALT1                (PB0  | ALT1)
#define PB1_ALT1                (PB1  | ALT1)
#define PC0_ALT1                (PC0  | ALT1)
#define PC3_ALT1                (PC3  | ALT1)



#define NUM_DIGITAL_PINS        18      // 27
#define NUM_ANALOG_INPUTS       14      // 14 to allow PIN_13/A13 definition for PC3 (no A10, A11, A12 on X033F8P6)

// Shared initializers for runtime and compile-time pin conversion.
#define VARIANT_DIGITAL_PIN_MAP {                              \
  PA_0,   /* D0  A0 */                                         \
  PA_1,   /* D1  A1 */                                         \
  PA_2,   /* D2  TX2/A2 */                                     \
  PA_3,   /* D3  RX2/A3 */                                     \
  PA_4,   /* D4  CS/A4 */                                      \
  PA_5,   /* D5  SCK/A5 */                                     \
  PA_6,   /* D6  MISO/A6 */                                    \
  PB_0,   /* D7  MOSI/A8  PA7 is tied to PB0 => input only  */ \
  PB_1,   /* D8  A9 */                                         \
  PB_7,   /* D9  RST */                                        \
  PC_16,  /* D10 USBDM  tied to PC11=input only  */            \
  PC_17,  /* D11 USBPD  tied to PC10=input only  */            \
  PA_9,   /* D12 */                                            \
  PA_11,  /* D13 SDA (not in 0-series) */                      \
  PA_10,  /* D14 SCL (not in 0-series) */                      \
  PC_3,   /* D15 A13 */                                        \
  PC_18,  /* D16 SWDIO */                                      \
  PC_19,  /* D17 SWCLK */                                      \
}

#define VARIANT_ANALOG_PIN_MAP {                               \
  0,  /* A0,  PA0  */                                          \
  1,  /* A1,  PA1 */                                           \
  2,  /* A2,  PA2 */                                           \
  3,  /* A3,  PA3 (no ADC Ch3 in 0-series!) */                 \
  4,  /* A4,  PA4 */                                           \
  5,  /* A5,  PA5 */                                           \
  6,  /* A6,  PA6 */                                           \
  7,  /* A7,  PA7=PB0, input only */                           \
  7,  /* A8,  PB0=PA7, input only */                           \
  8,  /* A9,  PB1 */                                           \
  10, /* A10, none */                                          \
  11, /* A11, none */                                          \
  12, /* A12, none */                                          \
  15, /* A13, PC3 */                                           \
}
#define ADC_RESOLUTION          12


// On-board LED pin number
#ifndef LED_BUILTIN
  #define LED_BUILTIN           PNUM_NOT_DEFINED
#endif



// On-board user button
#ifndef USER_BTN
  #define USER_BTN              PNUM_NOT_DEFINED
#endif

// SPI definitions
#ifndef PIN_SPI_SS
  #define PIN_SPI_SS            PA4
#endif
#ifndef PIN_SPI_MOSI
  #define PIN_SPI_MOSI          PA7
#endif
#ifndef PIN_SPI_MISO
  #define PIN_SPI_MISO          PA6
#endif
#ifndef PIN_SPI_SCK
  #define PIN_SPI_SCK           PA5
#endif

// I2C definitions
#ifndef PIN_WIRE_SDA
  #define PIN_WIRE_SDA          PC17
#endif
#ifndef PIN_WIRE_SCL
  #define PIN_WIRE_SCL          PC16
#endif

// Timer Definitions
#ifndef TIMER_TONE
  #define TIMER_TONE            TIM3
#endif
#ifndef TIMER_SERVO
  #define TIMER_SERVO           TIM2
#endif


// UART Definitions
// CH32X033 (TSSOP20):
//    RX: PA11=RX1=SDA, PA3=RX2, PC19=RX3=SWCLK, PB1=RX4
//    TX: PA10=TX1=SCL, PA2=TX2, PC18=TX3=SWDIO, PB0=TX4=MOSI=PA7
// For CH32X033F8P6 serial pins RX2=PA3/TX2=PA2 or alternative RX1_1=PA11/TX1_1=PA10
#ifndef BOARD_DEFAULT_SERIAL
  #define BOARD_DEFAULT_SERIAL 2
#endif
#if (defined(SERIAL_UART_INSTANCE) && SERIAL_UART_INSTANCE==3)
  #error "CH32X033 Serial3 cannot be selected (pins are used by SWDIO/SWCLK)"
#endif

// Default pins for each hardware serial port.
#ifndef PIN_SERIAL1_RX
  #define PIN_SERIAL1_RX     PA11  /* UART1 default RX pin */
#endif
#ifndef PIN_SERIAL1_TX
  #define PIN_SERIAL1_TX     PA10  /* UART1 default TX pin */
#endif

#ifndef PIN_SERIAL2_RX
  #define PIN_SERIAL2_RX     PA3  /* UART2 default RX pin */
#endif
#ifndef PIN_SERIAL2_TX
  #define PIN_SERIAL2_TX     PA2  /* UART2 default TX pin */
#endif

#ifndef PIN_SERIAL4_RX
  #define PIN_SERIAL4_RX     PB1  /* UART4 default RX pin */
#endif
#ifndef PIN_SERIAL4_TX
  #define PIN_SERIAL4_TX     PB0  /* UART4 default TX pin */
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
