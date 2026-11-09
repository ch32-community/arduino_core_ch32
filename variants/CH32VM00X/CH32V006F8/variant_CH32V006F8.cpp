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
These CH32V006F packages have 20 pins:
  CH32V006F8P6 - TSSOP20 - PC0-PC7, PD0-PD7, PA1/PA2, A0-A7      - PD1=SWIO, PD7=RST, PD6=RX, PD5=TX
  CH32V006F8U6 - QFN20   - PC0-PC7, PD0-PD7, PA1/PA2, A0-A7      - PD1=SWIO, PD7=RST, PD6=RX, PD5=TX
  CH32V006F4U6 - QFN20   - PC0-PC7, PD0-PD7, PA1/PA2, A0-A7      - PD1=SWIO, PD7=RST, PD6=RX, PD5=TX, 16K Flash, 4K RAM, no OPA

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
*/

// Digital PinName array,Some GPIOs are bound to the same pin. 
const PinName digitalPin[] = VARIANT_DIGITAL_PIN_MAP;
const uint32_t analogInputPin[] = VARIANT_ANALOG_PIN_MAP;
