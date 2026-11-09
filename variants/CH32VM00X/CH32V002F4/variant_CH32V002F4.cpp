/**
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

#include "pins_arduino.h"

/*

CH32V002 comes in same packages as CH32V003, with addition of QFN12:
  CH32V002F4P6 - TSSOP20 - PC0-PC7, PD0-PD7, PA1/PA2, A0-A7      - PD1=SWIO, PD7=RST, PD6=RX, PD5=TX
  CH32V002F4U6 - QFN20   - PC0-PC7, PD0-PD7, PA1/PA2, A0-A7      - PD1=SWIO, PD7=RST, PD6=RX, PD5=TX
  CH32V002A4M6 - SOP16   - PC0-PC7, PD1/4-7, PA1/2, A0-2/5-7     - PD1=SWIO, PD7=RST, PD6=RX, PD5=TX
  CH32V002D4U6 - QFN12   - PC0/3/4/6/7, PD0/1/4/7, PA1/2, A0-2/7 - PD1=SWIO, PD7=RST
  CH32V002J4M6 - SOP8    - PC1/2/4, PD1, PA1/2, A0-2/5/6         - PD1=SWIO=PD4=PD5=TX, PA1=PD6=A1=A6=RX
The largest package has 20 pins. For compatibility of Arduino pin numbers the TSSOP20 pinout is used.

V003/V002/V006 TSSOP20 PINOUT
            +-----v-----+
   14/A7 1 -+PD4     PD3+- 20  13/A4
   TX/A5 2 -+PD5     PD2+- 19  12/A3
   RX/A6 3 -+PD6     PD1+- 18  SWIO
     RST 4 -+PD7     PC7+- 17  9
    0/A1 5 -+PA1     PC6+- 16  8
    1/A0 6 -+PA2     PC5+- 15  7
     GND 7 -+VSS     PC4+- 14  6/A2
      10 8 -+PD0     PC3+- 13  5
     VCC 9 -+VDD     PC2+- 12  4/SCL
      2 10 -+PC0     PC1+- 11  3/SDA
            +-----------+        

CH32V002 vs CH32V003

V002: RV32EmC, 16K Flash, 4KB SRAM, 3328B bootloader, 256B user mem, 12-bit ADC, System Clock Monitor, 2-5V, QFN12
V003: RV32EC,  16K Flash, 2KB SRAM, 1920B bootloasder, 64B user mmm, 10-bit ADC, OPA, 3v#-5V
V004: RV32EmC, 32K Flash, 6KB SRAM, 3328B bootloader, 256B user mem, 12-bit ADC, System Clock Monitor, 2-5V
V005: RV32EmC, 32K Flash, 6KB SRAM, 3328B bootloader, 256B user mem, 12-bit ADC, System Clock Monitor, 2-5V, QFN12, QSOP24, UART2
V006: RV32EmC, 62K Flash, 8KB SRAM, 3328B bootloader, 256B user mem, 12-bit ADC, 8-ch touch. System Clock Monitor, 2-5V, OPA, UART2, 16-bit simple timer, QFN32

Model           Flash Ram Gpio                      SPI
V003            15K 2K
CH32V002F4P6    16K 4K  18  1   1   2   8+3 1   1   1   TSSOP20
F4U6                                                    QFN20
A4M6                    14              6+3             SOP16
D4U6                    11              4+3         -   QFN12
J4M6                    6               6+3         -   SOP8
V004            32K 6K
V005            32K 6K
V006K8          64K 8K

*/
// Digital PinName array. On smaller packages some GPIOs are bound to the same pin.
// Leading pinout is TSSOP20 (18x gpio), same as V003 TSSOP20
const PinName digitalPin[] = VARIANT_DIGITAL_PIN_MAP;
const uint32_t analogInputPin[] = VARIANT_ANALOG_PIN_MAP;
