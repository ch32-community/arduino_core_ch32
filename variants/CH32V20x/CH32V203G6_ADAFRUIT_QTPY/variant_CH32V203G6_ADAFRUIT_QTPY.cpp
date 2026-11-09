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
#include "hw_config.h"

// Digital PinName array
const PinName digitalPin[] = VARIANT_DIGITAL_PIN_MAP;
const uint32_t analogInputPin[] = VARIANT_ANALOG_PIN_MAP;



extern "C" {

void pre_init(void) {
  // Enable Flash enhance read mode for full 224KB
  FLASH->KEYR = 0x45670123; // FLASH_Unlock_Fast();
  FLASH->KEYR = 0xCDEF89AB;

  FLASH->CTLR |= (1 << 24); // Enhanced Read Mode

  FLASH->CTLR |= (1 << 15); // FLASH_Lock_Fast();

  hw_config_init();
}

}
