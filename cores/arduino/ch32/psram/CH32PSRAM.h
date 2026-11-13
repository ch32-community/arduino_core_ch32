#pragma once

#include <Arduino.h>

/** Internal hardware setup for the CH32V467RET6's 4 MiB PSRAM.
 * The core owns this memory as a heap; applications use pmalloc()/pcalloc().
 */
class CH32PSRAM {
public:
  bool begin();
  uint32_t size() const;
  volatile uint32_t *data() const;
  int readModeRegister(uint8_t address);

private:
  static constexpr uintptr_t memoryAddress = 0x80000000u; ///< PSRAM mapping.
  static constexpr uint32_t capacity = 4 * 1024 * 1024; ///< Capacity in bytes.
  static constexpr uintptr_t chipIdAddress = 0x1ffff704u; ///< Chip identifier.
  static constexpr uint32_t chipIdMask = 0xffff000fu; ///< Device/package bits.
  static constexpr uint32_t supportedChipId = 0x46750002u; ///< CH32V467RET6.
  static constexpr uint32_t commandPollLimit =
      1000000;                                        ///< Bounded busy polling.
  static constexpr uint16_t mr0ReadLatency = 4u << 2; ///< EVT read latency.
  static constexpr uint16_t mr4WriteLatency = 1u << 5; ///< EVT write latency.
  bool waitReady();
  bool writeModeRegister(uint8_t address, uint16_t value);
  bool _ready = false; ///< True after successful initialization.
};
