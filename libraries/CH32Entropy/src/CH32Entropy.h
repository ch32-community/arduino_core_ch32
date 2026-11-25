// SPDX-FileCopyrightText: 2026 Adafruit Industries
// SPDX-License-Identifier: MIT
#pragma once

#include <stddef.h>
#include <stdint.h>

#if !defined(CH32_ALLOW_EXPERIMENTAL_ENTROPY)
#error                                                                         \
    "CH32 RNG entropy is unvalidated. Define CH32_ALLOW_EXPERIMENTAL_ENTROPY only for experiments."
#endif

/** Collector state. Ready means operational, not a validated entropy budget. */
enum class CH32EntropyStatus {
  Uninitialized,    ///< No seed has been collected.
  Ready,            ///< An experimental seed is installed.
  HardwareError,    ///< RNG clock or seed error flag.
  Timeout,          ///< RNG did not produce data in the bounded wait.
  Repetition,       ///< Eight consecutive identical raw words.
  LowVariation,     ///< Fewer than 128 distinct words in the 1024-word pool.
  IntegrationError, ///< BearSSL was built without its getentropy hook.
};

bool ch32EntropyBegin();
void ch32EntropyEnd();
bool ch32EntropyReady();
CH32EntropyStatus ch32EntropyStatus();
uint64_t ch32EntropyRequests();

// Fault injection exists only in a separately built diagnostic firmware.
#if defined(CH32_ENTROPY_TEST)
using CH32EntropyReader = CH32EntropyStatus (*)(uint32_t *word);
bool ch32EntropyTestBegin(CH32EntropyReader reader);
#endif
