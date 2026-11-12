/* PSRAM setup follows WCH CH32V4x7 EVT PSRAM example at
 * afee2d144b184176e5ca21202714ca9add1dd46f. Register operations deliberately
 * mirror the controller's command/busy sequencing, with bounded waits.
 * Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
 * WCH software, modified or not, is for WCH microcontrollers only. */
#include "CH32PSRAM.h"

/**
 * @brief Initialize PSRAM with a 200 MHz system PLL and 100 or 200 MHz HCLK.
 * @return True on success; false for unsupported chips/clocks or a timeout.
 * Calling begin() again on an initialized instance leaves its data intact.
 */
bool CH32PSRAM::begin() {
#if defined(CH32V467)
  if (SystemClock != 200000000 ||
      (SystemCoreClock != 100000000 && SystemCoreClock != 200000000)) {
    return false;
  }
  const uint32_t chip = *(volatile uint32_t *)chipIdAddress;
  if ((chip & chipIdMask) != supportedChipId) {
    return false;
  }
  if (_ready) {
    return true;
  }

  RCC_PB1PeriphClockCmd(RCC_PB1Periph_PWR, ENABLE);
  PWR_VDD18LevelConfig(PWR_VDD18_Level1);
  RCC_HBPeriphClockCmd(RCC_HBPeriph_PSRAM, ENABLE);
  RCC_HBPeriphResetCmd(RCC_HBPeriph_PSRAM, ENABLE);
  RCC_HBPeriphResetCmd(RCC_HBPeriph_PSRAM, DISABLE);

  // Select 32 Mbit and leave low-power mode. Both supported HCLK settings
  // retain the 200 MHz system PLL used by WCH's reference configuration.
  PSRAM->CTLR = PSRAM_CAPCFG_32M | PSRAM_EXIT_LPMD;
  // TIMING fields: trc=20, tcph=12, txlpd=7, from the 200 MHz EVT example.
  PSRAM->TIMING = 20u | (12u << 16) | (7u << 24);
  delay(1);

  // Set controller write latency before changing the memory's MR4.
  PSRAM->LATENCY = 7u << 8;
  if (!writeModeRegister(4, mr4WriteLatency)) {
    return false;
  }
  // Set controller read latency before changing the memory's MR0.
  PSRAM->LATENCY |= 7;
  if (!writeModeRegister(0, mr0ReadLatency)) {
    return false;
  }
  _ready = true;
  return true;
#else
  // The CH32V407 and other families have no supported in-package PSRAM.
  return false;
#endif
}

/**
 * @brief Get the initialized capacity.
 * @return Capacity in bytes, or zero before successful initialization.
 */
uint32_t CH32PSRAM::size() const { return _ready ? capacity : 0; }

/**
 * @brief Get the memory mapping, or nullptr before successful initialization.
 * @return Pointer for aligned 32-bit accesses. Memory is not added to the heap.
 */
volatile uint32_t *CH32PSRAM::data() const {
  return _ready ? (volatile uint32_t *)memoryAddress : nullptr;
}

/**
 * @brief Read a PSRAM mode register.
 * @param address Mode register address, from 0 through 15.
 * @return Raw register value, or -1 before begin(), on invalid input or
 * timeout.
 */
int CH32PSRAM::readModeRegister(uint8_t address) {
#if defined(CH32V467)
  if (!_ready || address > 15 || !waitReady()) {
    return -1;
  }
  // CMD1_CFG holds the read-register opcode in its low byte.
  uint32_t command = PSRAM->CMD1_CFG & 0xff;
  PSRAM->ADDR = ((uint32_t)address << 16) | (command << 8);
  PSRAM->ADDR |= PSRAM_BUSY;
  PSRAM->ADDR &= ~PSRAM_MW;
  if (!waitReady()) {
    return -1;
  }
  return PSRAM->DATA & 0xffff;
#else
  (void)address;
  return -1;
#endif
}

#if defined(CH32V467)
/** @brief Wait up to 10 ms for a mode-register command to finish. */
bool CH32PSRAM::waitReady() {
  uint32_t start = micros();
  while (PSRAM->ADDR & PSRAM_BUSY) {
    if (micros() - start >= commandTimeoutUs) {
      return false;
    }
  }
  return true;
}

/**
 * @brief Write a mode register during initialization.
 * @param address Mode register address.
 * @param value Raw mode register value.
 * @return True if the command finished within the timeout.
 */
bool CH32PSRAM::writeModeRegister(uint8_t address, uint16_t value) {
  if (!waitReady()) {
    return false;
  }
  PSRAM->DATA = value;
  // CMD1_CFG holds the write-register opcode in bits 15:8.
  uint32_t command = (PSRAM->CMD1_CFG >> 8) & 0xff;
  PSRAM->ADDR = ((uint32_t)address << 16) | (command << 8);
  PSRAM->ADDR |= PSRAM_BUSY;
  PSRAM->ADDR |= PSRAM_MW;
  return waitReady();
}
#endif
