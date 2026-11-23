// SPDX-FileCopyrightText: Copyright (c) 2026 Adafruit Industries
// SPDX-License-Identifier: BSD-3-Clause
#include "ch32_def.h"
#include "pins_arduino.h"

const PinName digitalPin[] = VARIANT_DIGITAL_PIN_MAP;
const uint32_t analogInputPin[] = VARIANT_ANALOG_PIN_MAP;
static_assert(sizeof(digitalPin) / sizeof(digitalPin[0]) == NUM_DIGITAL_PINS,
              "Metro pin count must match NUM_DIGITAL_PINS");

extern "C" void ch32TinyUSBEnterBootloader(void) {
  RCC_PB1PeriphClockCmd(RCC_PB1Periph_BKP | RCC_PB1Periph_PWR, ENABLE);
  PWR->CTLR |= PWR_CTLR_DBP;
  // TinyUF2's backup-register request survives the software reset.
  *(volatile uint16_t *)0x40006C28 = 0xF016;
  NVIC_SystemReset();
}
