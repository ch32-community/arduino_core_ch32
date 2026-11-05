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
#define                         DAC_MODULE_ENABLED
#define                         SPI_MODULE_ENABLED
#define                         I2C_MODULE_ENABLED
#define                         TIM_MODULE_ENABLED

/* CH32V307VCT6 Pins (LQFP100, 80 GPIO)
 * Format: Dx is the digital pin number, Ay is the analog input (if available),
 * Alternate functions are listed in parentheses (see CH32V307DS0.PDF, Table 3-1).
 */

/* Port A */
#define PA0                     0   // D0 / A0  (ADC1_IN0, ADC2_IN0, TIM2_CH1, TIM5_CH1, TIM8_ETR, USART2_CTS, OPA4_OUT0)
#define PA1                     1   // D1 / A1  (ADC1_IN1, ADC2_IN1, TIM2_CH2, TIM5_CH2, USART2_RTS, OPA3_OUT0)
#define PA2                     2   // D2 / A2  (ADC1_IN2, ADC2_IN2, TIM2_CH3, TIM5_CH3, TIM9_CH1, USART2_TX, OPA2_OUT0)
#define PA3                     3   // D3 / A3  (ADC1_IN3, ADC2_IN3, TIM2_CH4, TIM5_CH4, TIM9_CH2, USART2_RX, OPA1_OUT0)
#define PA4                     4   // D4 / A4  (ADC1_IN4, ADC2_IN4, SPI1_NSS, USART2_CK, TIM9_CH3, DAC1_OUT, DVP_HSYNC)
#define PA5                     5   // D5 / A5  (ADC1_IN5, ADC2_IN5, SPI1_SCK, TIM10_CH1N, DAC2_OUT, DVP_VSYNC, OPA2_CH1N)
#define PA6                     6   // D6 / A6  (ADC1_IN6, ADC2_IN6, SPI1_MISO, TIM3_CH1, TIM8_BKIN, TIM10_CH2N, USART1_TX_3, UART7_TX_1, DVP_PCLK)
#define PA7                     7   // D7 / A7  (ADC1_IN7, ADC2_IN7, SPI1_MOSI, TIM3_CH2, TIM8_CH1N, TIM10_CH3N, USART1_RX_3, UART7_RX_1, ETH_MII_RX_DV)
#define PA8                     8   // D8       (TIM1_CH1, MCO, USART1_CK)
#define PA9                     9   // D9       (TIM1_CH2, USART1_TX, OTG_FS_VBUS, DVP_D0)
#define PA10                    10  // D10      (TIM1_CH3, USART1_RX, OTG_FS_ID, DVP_D1)
#define PA11                    11  // D11      (TIM1_CH4, USART1_CTS, OTG_FS_DM, CAN1_RX)
#define PA12                    12  // D12      (TIM1_ETR, USART1_RTS, OTG_FS_DP, CAN1_TX)
#define PA13                    13  // D13      (SWDIO, TIM10_CH2N, TIM8_CH1N_1, USART3_TX_2)
#define PA14                    14  // D14      (SWCLK, TIM10_CH3N, TIM8_CH2N_1, UART8_TX_1, USART3_RX_2)
#define PA15                    15  // D15      (SPI3_NSS, TIM2_CH1_1, TIM2_ETR_1, SPI1_NSS_1, TIM8_CH3N_1, UART8_RX_1)

/* Port B */
#define PB0                     16  // D16 / A8  (ADC1_IN8, ADC2_IN8, TIM3_CH3, TIM8_CH2N, TIM1_CH2N_1, TIM9_CH1N_1, UART4_TX_1)
#define PB1                     17  // D17 / A9  (ADC1_IN9, ADC2_IN9, TIM3_CH4, TIM8_CH3N, TIM1_CH3N_1, TIM9_CH2N_1, UART4_RX_1)
#define PB2                     18  // D18        (BOOT1, OPA3_CH0N)
#define PB3                     19  // D19        (SPI3_SCK, TIM2_CH2_1, TIM10_CH1_1, DVP_D5)
#define PB4                     20  // D20        (SPI3_MISO, TIM3_CH1_2, SPI1_MISO_1, UART5_TX_1, TIM10_CH2_1)
#define PB5                     21  // D21        (SPI3_MOSI, I2C1_SMBA, ETH_MII_PPS_OUT, TIM3_CH2_2, SPI1_MOSI_1, CAN2_RX_1, UART5_RX_1)
#define PB6                     22  // D22        (I2C1_SCL, TIM4_CH1, USART1_TX_1, USBHS_DM, CAN2_TX_1, TIM8_CH1_1, DVP_D5)
#define PB7                     23  // D23        (I2C1_SDA, TIM4_CH2, USART1_RX_1, USBHS_DP, TIM8_CH2_1, FSMC_NADV)
#define PB8                     24  // D24        (I2C1_SCL_1, TIM4_CH3, SDIO_D4, TIM10_CH1, ETH_MII_TXD3, DVP_D6)
#define PB9                     25  // D25        (I2C1_SDA_1, TIM4_CH4, SDIO_D5, TIM10_CH2, ETH_MII_TXD3, DVP_D7)
#define PB10                    26  // D26        (I2C2_SCL, USART3_TX, TIM2_CH3_2, OPA2_CH0N, ETH_MII_RX_ER)
#define PB11                    27  // D27        (I2C2_SDA, USART3_RX, TIM2_CH4_2, OPA1_CH0N, ETH_MII_TX_EN)
#define PB12                    28  // D28        (SPI2_NSS, I2C2_SMBA, USART3_CK, TIM1_BKIN, CAN2_RX, ETH_MII_TXD0)
#define PB13                    29  // D29        (SPI2_SCK, USART3_CTS, TIM1_CH1N, CAN2_TX, ETH_MII_TXD1, OPA3_CH0P)
#define PB14                    30  // D30        (SPI2_MISO, USART3_RTS, TIM1_CH2N, SDIO_D0, OPA2_CH0P)
#define PB15                    31  // D31        (SPI2_MOSI, TIM1_CH3N, SDIO_D1, OPA1_CH0P, USART1_TX_2)

/* Port C */
#define PC0                     32  // D32 / A10 (ADC1_IN10, ADC2_IN10, TIM9_CH1N, UART6_TX)
#define PC1                     33  // D33 / A11 (ADC1_IN11, ADC2_IN11, TIM9_CH2N, UART6_RX)
#define PC2                     34  // D34 / A12 (ADC1_IN12, ADC2_IN12, TIM9_CH3N, UART7_TX, OPA3_CH1N)
#define PC3                     35  // D35 / A13 (ADC1_IN13, ADC2_IN13, TIM10_CH3, UART7_RX, OPA4_CH1N)
#define PC4                     36  // D36 / A14 (ADC1_IN14, ADC2_IN14, TIM9_CH4, UART8_TX, OPA4_CH1P, ETH_MII_RXD0)
#define PC5                     37  // D37 / A15 (ADC1_IN15, ADC2_IN15, TIM9_BKIN, UART8_RX, OPA3_CH1P, ETH_MII_RXD1)
#define PC6                     38  // D38        (I2S2_MCK, TIM8_CH1, SDIO_D6, TIM3_CH1_3, ETH_RXP)
#define PC7                     39  // D39        (I2S3_MCK, TIM8_CH2, SDIO_D7, TIM3_CH2_3, ETH_RXN)
#define PC8                     40  // D40        (TIM8_CH3, SDIO_D0, TIM3_CH3_3, DVP_D2)
#define PC9                     41  // D41        (TIM8_CH4, SDIO_D1, TIM3_CH4_3, DVP_D3)
#define PC10                    42  // D42        (UART4_TX, SDIO_D2, TIM10_ETR, SPI3_SCK_1, DVP_D8)
#define PC11                    43  // D43        (UART4_RX, SDIO_D3, TIM10_CH4, SPI3_MISO_1, DVP_D4)
#define PC12                    44  // D44        (UART5_TX, SDIO_CK, TIM10_BKIN, SPI3_MOSI_1, DVP_D9)
#define PC13                    45  // D45        (TAMPER-RTC, TIM8_CH4_1)
#define PC14                    46  // D46        (OSC32_IN, TIM9_CH4_1)
#define PC15                    47  // D47        (OSC32_OUT, TIM10_CH4_1)

/* Port D */
#define PD0                     48  // D48        (OSC_IN, FSMC_D2, CAN1_RX_3, TIM10_ETR_2)
#define PD1                     49  // D49        (OSC_OUT, FSMC_D3, CAN1_TX_3, TIM10_CH1_2)
#define PD2                     50  // D50        (TIM3_ETR, UART5_RX, SDIO_CMD, FSMC_NADV, DVP_D11)
#define PD3                     51  // D51        (FSMC_CLK, USART2_CTS_1, TIM10_CH2_2)
#define PD4                     52  // D52        (FSMC_NOE, USART2_RTS_1)
#define PD5                     53  // D53        (FSMC_NWE, USART2_TX_1, TIM10_CH3_2)
#define PD6                     54  // D54        (FSMC_NWAIT, USART2_RX_1, DVP_D10)
#define PD7                     55  // D55        (FSMC_NE1, FSMC_NCE2, USART2_CK_1, TIM10_CH4_2)

/* Port E */
#define PE0                     56  // D56        (TIM4_ETR, FSMC_NBL0, UART4_TX_2)
#define PE1                     57  // D57        (FSMC_NBL1, UART4_RX_2)
#define PE2                     58  // D58        (TIM10_BKIN_2, FSMC_A23)
#define PE3                     59  // D59        (TIM10_CH1N_2, FSMC_A19)
#define PE4                     60  // D60        (TIM10_CH2N_2, FSMC_A20)
#define PE5                     61  // D61        (TIM10_CH3N_2, FSMC_A21)
#define PE6                     62  // D62        (TIM10_CH4N_2, FSMC_A22)
#define PE7                     63  // D63        (TIM1_ETR_3, FSMC_D4, OPA3_OUT1)
#define PE8                     64  // D64        (TIM1_CH1N_3, FSMC_D5, OPA4_OUT1, UART5_TX_2)
#define PE9                     65  // D65        (TIM1_CH1_3, FSMC_D6, UART5_RX_2)
#define PE10                    66  // D66        (TIM1_CH2N_3, FSMC_D7, UART6_TX_2)
#define PE11                    67  // D67        (TIM1_CH2_3, FSMC_D8, UART6_RX_2)
#define PE12                    68  // D68        (TIM1_CH3N_3, FSMC_D9, UART7_TX_2)
#define PE13                    69  // D69        (TIM1_CH3_3, FSMC_D10, UART7_RX_2)
#define PE14                    70  // D70        (TIM1_CH4_3, FSMC_D11, OPA2_OUT1, UART8_TX_2)
#define PE15                    71  // D71        (TIM1_BKIN_3, FSMC_D12, OPA1_OUT1, UART8_RX_2)

/* Alternate names for remapping (used in PeripheralPins.c) */
#define PA0_ALT1                (PA0  | ALT1)
#define PA1_ALT1                (PA1  | ALT1)
#define PA2_ALT1                (PA2  | ALT1)
#define PA3_ALT1                (PA3  | ALT1)
#define PA4_ALT1                (PA4  | ALT1)
#define PA5_ALT1                (PA5  | ALT1)
#define PA4_ALT2                (PA4  | ALT2) // DAC_Channel1
#define PA5_ALT2                (PA5  | ALT2) // DAC_Channel2

/* Alternate names for USART1 (PB6/PB7 and PA6/PA7) */
#define PA9_ALT1                (PA9  | ALT1)  // USART1_TX remap to PB6
#define PA10_ALT1               (PA10 | ALT1)  // USART1_RX remap to PB7
#define PA9_ALT2                (PA9  | ALT2)  // USART1_TX remap to PA6
#define PA10_ALT2               (PA10 | ALT2)  // USART1_RX remap to PA7

/* Alternate names for USART2 (PD5/PD6) */
#define PA2_ALT1                (PA2  | ALT1)  // USART2_TX remap to PD5
#define PA3_ALT1                (PA3  | ALT1)  // USART2_RX remap to PD6

/* Alternate names for USART3 (PC10/PC11) */
#define PB10_ALT1               (PB10 | ALT1)  // USART3_TX remap to PC10
#define PB11_ALT1               (PB11 | ALT1)  // USART3_RX remap to PC11

#define NUM_DIGITAL_PINS        72
#define NUM_ANALOG_INPUTS       16
#define ADC_RESOLUTION          12

// On-board LED pin number
#ifndef LED_BUILTIN
  #define LED_BUILTIN           PNUM_NOT_DEFINED
#endif

// On-board user button
#ifndef USER_BTN
  #define USER_BTN              PNUM_NOT_DEFINED
#endif

// SPI definitions (SPI1 by default, PA4..PA7)
#ifndef PIN_SPI_SS
  #define PIN_SPI_SS            PA4
#endif
#ifndef PIN_SPI_SS1
  #define PIN_SPI_SS1           PNUM_NOT_DEFINED
#endif
#ifndef PIN_SPI_SS2
  #define PIN_SPI_SS2           PNUM_NOT_DEFINED
#endif
#ifndef PIN_SPI_SS3
  #define PIN_SPI_SS3           PNUM_NOT_DEFINED
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

// I2C definitions (I2C2 by default, PB10/PB11)
#define PIN_WIRE_SDA          PB11
#define PIN_WIRE_SCL          PB10

// Timer Definitions
// Use TIM6/TIM7 when possible as servo and tone don't need GPIO output pin
#ifndef TIMER_TONE
  #define TIMER_TONE            TIM3
#endif
#ifndef TIMER_SERVO
  #define TIMER_SERVO           TIM2
#endif

// UART Definitions
// CH32V307: USART1-3, UART4-8
//    RX: PA10=RX1, PA3=RX2, PB11=RX3, PC11=RX4, PD2=RX5, PC1=RX6, PC3=RX7, PC5=RX8
//    TX: PA9=TX1, PA2=TX2, PB10=TX3, PC10=TX4, PC12=TX5, PC0=TX6, PC2=TX7, PC4=TX8
#ifndef BOARD_DEFAULT_SERIAL
  #define BOARD_DEFAULT_SERIAL 1
#endif

// Default pins for each hardware serial port.
#ifndef PIN_SERIAL1_RX
  #define PIN_SERIAL1_RX     PA10 /* UART1 default RX pin */
#endif
#ifndef PIN_SERIAL1_TX
  #define PIN_SERIAL1_TX     PA9  /* UART1 default TX pin */
#endif

#ifndef PIN_SERIAL2_RX
  #define PIN_SERIAL2_RX     PA3  /* UART2 default RX pin */
#endif
#ifndef PIN_SERIAL2_TX
  #define PIN_SERIAL2_TX     PA2  /* UART2 default TX pin */
#endif

#ifndef PIN_SERIAL3_RX
  #define PIN_SERIAL3_RX     PB11 /* UART3 default RX pin */
#endif
#ifndef PIN_SERIAL3_TX
  #define PIN_SERIAL3_TX     PB10 /* UART3 default TX pin */
#endif

#ifndef PIN_SERIAL4_RX
  #define PIN_SERIAL4_RX     PC11 /* UART4 default RX pin */
#endif
#ifndef PIN_SERIAL4_TX
  #define PIN_SERIAL4_TX     PC10 /* UART4 default TX pin */
#endif

#ifndef PIN_SERIAL5_RX
  #define PIN_SERIAL5_RX     PD2  /* UART5 default RX pin */
#endif
#ifndef PIN_SERIAL5_TX
  #define PIN_SERIAL5_TX     PC12 /* UART5 default TX pin */
#endif

#ifndef PIN_SERIAL6_RX
  #define PIN_SERIAL6_RX     PC1  /* UART6 default RX pin */
#endif
#ifndef PIN_SERIAL6_TX
  #define PIN_SERIAL6_TX     PC0  /* UART6 default TX pin */
#endif

#ifndef PIN_SERIAL7_RX
  #define PIN_SERIAL7_RX     PC3  /* UART7 default RX pin */
#endif
#ifndef PIN_SERIAL7_TX
  #define PIN_SERIAL7_TX     PC2  /* UART7 default TX pin */
#endif

#ifndef PIN_SERIAL8_RX
  #define PIN_SERIAL8_RX     PC5  /* UART8 default RX pin */
#endif
#ifndef PIN_SERIAL8_TX
  #define PIN_SERIAL8_TX     PC4  /* UART8 default TX pin */
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
