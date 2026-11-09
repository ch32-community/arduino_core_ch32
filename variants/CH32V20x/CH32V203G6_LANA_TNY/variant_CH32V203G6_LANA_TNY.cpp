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
// note: PA_11 and PA_12 are connected to USB connector
// note: PA_8, PA_10 and PB_2 are not available on CH32V203G6U6



extern "C" {

void pre_init(void) {
  hw_config_init();

  // Enable PD0 and PD1 as GPIO
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
  GPIO_PinRemapConfig(GPIO_Remap_PD01, ENABLE);

  // Enable PA13 and PA14 as GPIO
  GPIO_PinRemapConfig(GPIO_Remap_SWJ_Disable, ENABLE);
}

}
