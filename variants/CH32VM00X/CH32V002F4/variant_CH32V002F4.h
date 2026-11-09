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
#define VARIANT_DIGITAL_PIN_MAP { \
  PA_1,   /* D0/A1 */             \
  PA_2,   /* D1/A0 */             \
  PC_0,   /* D2 */                \
  PC_1,   /* D3 */                \
  PC_2,   /* D4 */                \
  PC_3,   /* D5 */                \
  PC_4,   /* D6/A2 */             \
  PC_5,   /* D7 */                \
  PC_6,   /* D8 */                \
  PC_7,   /* D9 */                \
  PD_0,   /* D10 */               \
  PD_1,   /* D11/SWIO */          \
  PD_2,   /* D12/A3 */            \
  PD_3,   /* D13/A4 */            \
  PD_4,   /* D14/A7 */            \
  PD_5,   /* D15/A5/TX */         \
  PD_6,   /* D16/A6/RX */         \
  PD_7    /* D17/RST */           \
                                  \
/* WCH V006K8 pins                \
  PA_0,   //D0                    \
  PA_1,   //D1/A1                 \
  PA_2,   //D2/A0                 \
  PA_3,   //D3                    \
  PA_4,   //D4                    \
  PA_5,   //D5                    \
  PA_6,   //D6                    \
  PA_7,   //D7                    \
  PB_0,   //D8                    \
  PB_1,   //D9                    \
  PB_2,   //D10                   \
  PB_3,   //D11                   \
  PB_4,   //D12                   \
  PB_5,   //D13                   \
  PB_6,   //D14                   \
  PC_0,   //D15                   \
  PC_1,   //D16                   \
  PC_2,   //D17                   \
  PC_3,   //D18                   \
  PC_4,   //D19/A2                \
  PC_5,   //D20                   \
  PC_6,   //D21                   \
  PC_7,   //D22                   \
  PD_0,   //D23                   \
  PD_1,   //D24                   \
  PD_2,   //D25/A3                \
  PD_3,   //D26/A4                \
  PD_4,   //D27/A7                \
  PD_5,   //D28/A5                \
  PD_6,   //D29/A6                \
  PD_7    //D30                   \
*/                                \
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
// CH32V002 only has USART1. CH32V002F4 pins:
//    RX: PD6=RX1
//    TX: PD5=TX1
// CH32V002F4 TSSOP20/QFN20 has RX1/TX1
#if defined(SERIAL_UART_ENABLE_ALL)
  #error "CH32V002A4 only supports Serial1; Dual Serial is unavailable"
#endif

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


