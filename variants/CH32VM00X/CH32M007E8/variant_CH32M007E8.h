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

/* CH32M007E Pins */
#define PA0                     7
#define PA1                     PIN_A1
#define PA2                     PIN_A0
#define PA3                     6
#define PA4                     4
#define PA5                     5
#define PB0                     8
#define PB1                     10
#define PB3                     12
//#define PC0                     19
#define PC1                     13
#define PC2                     14
#define PC4                     PIN_A2
#define PC5                     16
#define PD0                     11
#define PD1                     18
#define PD2                     PIN_A3
#define PD3                     PIN_A4 
#define PD4                     PIN_A7
#define PD5                     PIN_A5
#define PD6                     PIN_A6 
#define PD7                     3


// Alternate pins number
//#define PD5_ALT1                (PD5  | ALT1)
//#define PD6_ALT1                (PD6  | ALT1)


#define NUM_DIGITAL_PINS        21
#define NUM_ANALOG_INPUTS       8

// Shared initializers for runtime and compile-time pin conversion.
#define VARIANT_DIGITAL_PIN_MAP { \
  PD_4, /* D0/A7 */               \
  PD_5, /* D1/A5/TX */            \
  PD_6, /* D2/A6/RX */            \
  PD_7, /* D3/ */                 \
  PA_4, /* D4/TX2 */              \
  PA_5, /* D5/RX2 */              \
  PA_3, /* D6/HO1 */              \
  PA_0, /* D7/LO1 */              \
  PB_0, /* D8/HO2 */              \
  PA_2, /* D9/LO2 */              \
  PB_1, /* D10/HO3 */             \
  PD_0, /* D11/LO3 */             \
  PB_3, /* D12/MISO */            \
  PC_1, /* D13/SDA */             \
  PC_2, /* D14/SCL */             \
  PC_4, /* D15/SS/A2 */           \
  PC_5, /* D16/RST */             \
  PA_1, /* D17/A1 */              \
  PD_1, /* D18/SWIO */            \
  PD_2, /* D19/A3/SCK */          \
  PD_3  /* D20/A4/MOSI */         \
}

#define VARIANT_ANALOG_PIN_MAP { \
  NC,  /* A0/PA2 (was 9, but A0 cannot be used, PA2 is occupied by LO2) */  \
  17,  /* A1/PA1 */                                                         \
  15,  /* A2/PC4 */                                                         \
  19,  /* A3/PD2 */                                                         \
  20,  /* A4/PD3 */                                                         \
  1,   /* A5/PD5 */                                                         \
  2,   /* A6/PD6 */                                                         \
  0    /* A7/PD4  */                                                        \
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
// TODO: M007E8R6 QSOP24 UART2 has no PA7 pin. It has RX/TX as alternative functions UART2_1 U2RX_/U2TX_ on pins PA4/PA5)
// CH32V007:
//    RX: PD6=RX1, PB3=RX2, PA5=RX2_1
//    TX: PD5=TX1, PA7=TX2, PA4=TX2_1

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


