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
These CH32V00X packages have 24 pins:
  CH32V006E8R6 - PA0-5, PB0/1/3, PC0-2/4/5, PD0-7 - PD1=SWIO, PC5=RST, PD6=RX, PD5=TX, 64K flash, 8K RAM, OPA, CAPKEY, 2xUART, 3xTMR
  CH32V005E6R6 - 32K flash, 6K RAM, 2xTMR, no CAPKEY, no OPA polling, 2xUART

CH32V006E8R6 QSOP24 PINOUT
                   +-----v-----+
       SWIO D18~ 1-+PD1     PB3+-24  D17 MISO
            D19~ 2-+PC0     PB1+-23  D16
        SDA D20  3-+PC1     PB0+-22  D15
        SCL D0   4-+PC2     PA3+-21  D14 
            GND  5-+VSS     PD0+-20 ~D13 
            VCC  6-+VDD     PA2+-19 ~D12 A0 
        RST D21  7-+PC5     PA0+-18  D11  
      SS/A2 D1~  8-+PC4     PA1+-17  D10* A1  
     SCK/A3 D2~  9-+PD2     PA5+-16  D9  RX2
    MOSI/A4 D3~ 10-+PD3     PA4+-15  D8  TX2
         A7 D4  11-+PD4     PD7+-14 ~D7
      TX/A5 D5  12-+PD5     PD6+-13  D6 RX/A6
                   +-----------+         
*/

// Digital PinName array, PA1 is shorted to PA6
const PinName digitalPin[] = VARIANT_DIGITAL_PIN_MAP;
const uint32_t analogInputPin[] = VARIANT_ANALOG_PIN_MAP;
