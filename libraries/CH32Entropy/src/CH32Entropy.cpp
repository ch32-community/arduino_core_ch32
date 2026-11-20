// SPDX-FileCopyrightText: 2026 Adafruit Industries
// SPDX-License-Identifier: MIT
#include "CH32Entropy.h"
#include <Arduino.h>
#include <ESP_SSLClient.h>
#include <ch32_def.h>
#include <errno.h>
#include <unistd.h>

#if !defined(CH32V4x7)
#error "CH32Entropy currently supports only CH32V407/467."
#endif

namespace {
// These thresholds detect gross faults. They are NOT SP 800-90B health tests
// and do not establish any cryptographic entropy per word or per seed.
constexpr unsigned discardWords = 4096;
constexpr unsigned poolWords = 1024;
constexpr unsigned minimumDistinctWords = 128;
constexpr unsigned maximumRun = 8;
constexpr uint32_t sampleSpacingUs = 50;
constexpr uint32_t wordTimeoutUs = 20000;
constexpr uint64_t maximumRequests = UINT64_C(1) << 48;

using Reader = CH32EntropyStatus (*)(uint32_t *);
br_hmac_drbg_context generator;
CH32EntropyStatus status = CH32EntropyStatus::Uninitialized;
uint64_t requests = 0;
bool initialized = false;

void wipe(void *data, size_t size) {
  volatile uint8_t *bytes = (volatile uint8_t *)data;
  while (size--) {
    *bytes++ = 0;
  }
}

CH32EntropyStatus readWord(uint32_t *word) {
  uint32_t started = micros();
  do {
    // The SDK constants are masks for hardware status register bits.
    uint32_t flags = RNG->SR;
    if (flags & (RNG_FLAG_CECS | RNG_FLAG_SECS)) {
      return CH32EntropyStatus::HardwareError;
    }
    if (flags & RNG_FLAG_DRDY) {
      *word = RNG_GetRandomNumber();
      return CH32EntropyStatus::Ready;
    }
  } while (micros() - started < wordTimeoutUs);
  return CH32EntropyStatus::Timeout;
}

bool collect(Reader reader, bool spaced) {
  br_sha256_context hash;
  br_sha256_init(&hash);
  uint32_t distinct[minimumDistinctWords] = {};
  unsigned distinctCount = 0;
  unsigned run = 0;
  uint32_t previous = 0, word = 0;
  uint8_t digest[br_sha256_SIZE] = {};
  status = CH32EntropyStatus::Ready;
  uint32_t started = micros();
  for (unsigned i = 0; i < poolWords; i++) {
    if (spaced) {
      delayMicroseconds(sampleSpacingUs);
    }
    status = reader(&word);
    if (status != CH32EntropyStatus::Ready) {
      break;
    }
    if (micros() - started >= 1000000) {
      status = CH32EntropyStatus::Timeout;
      break;
    }
    if (i && word == previous) {
      run++;
    } else {
      run = 1;
    }
    previous = word;
    if (run >= maximumRun) {
      status = CH32EntropyStatus::Repetition;
      break;
    }
    if (distinctCount < minimumDistinctWords) {
      bool found = false;
      for (unsigned j = 0; j < distinctCount; j++) {
        if (distinct[j] == word) {
          found = true;
          break;
        }
      }
      if (!found) {
        distinct[distinctCount++] = word;
      }
    }
    // The supported CH32 targets are little-endian. Only raw RNG readings
    // enter the pool: no public chip ID, timestamp, or Arduino random() seed.
    br_sha256_update(&hash, &word, sizeof(word));
  }
  if (status == CH32EntropyStatus::Ready &&
      distinctCount < minimumDistinctWords) {
    status = CH32EntropyStatus::LowVariation;
  }
  if (status == CH32EntropyStatus::Ready) {
    br_sha256_out(&hash, digest);
    br_hmac_drbg_init(&generator, &br_sha256_vtable, digest, sizeof(digest));
    initialized = true;
  }
  wipe(&hash, sizeof(hash));
  wipe(distinct, sizeof(distinct));
  wipe(digest, sizeof(digest));
  wipe(&word, sizeof(word));
  wipe(&previous, sizeof(previous));
  return status == CH32EntropyStatus::Ready;
}
} // namespace

/**
 * @brief Collect an experimental seed, replacing and erasing any old state.
 * @return True when the collector's gross-fault checks passed.
 * @note Call from setup/loop with interrupts enabled, before opening TLS.
 * SHA-256 cannot create entropy. This is NOT a production-security guarantee.
 */
bool ch32EntropyBegin() {
  ch32EntropyEnd();
  const char *seederName = nullptr;
  if (!br_prng_seeder_system(&seederName) || !seederName ||
      strcmp(seederName, "getentropy")) {
    status = CH32EntropyStatus::IntegrationError;
    return false;
  }
  // Match the SYSCLK source used in the reset characterization. Do not change
  // CPU, USB, or Ethernet clocks. This module owns the RNG peripheral.
  RCC_RNGCLKConfig(RCC_RNGCLKSource_SYSCLK);
  RCC_HBPeriphClockCmd(RCC_HBPeriph_RNG, ENABLE);
  RNG_Cmd(ENABLE);
  uint32_t word = 0;
  uint32_t started = micros();
  for (unsigned i = 0; i < discardWords; i++) {
    status = readWord(&word);
    if (status != CH32EntropyStatus::Ready || micros() - started >= 1000000) {
      if (status == CH32EntropyStatus::Ready) {
        status = CH32EntropyStatus::Timeout;
      }
      wipe(&word, sizeof(word));
      return false;
    }
  }
  wipe(&word, sizeof(word));
  delay(100);
  return collect(readWord, true);
}

/** @brief Erase the software RNG state and prevent further output. */
void ch32EntropyEnd() {
  initialized = false;
  status = CH32EntropyStatus::Uninitialized;
  wipe(&generator, sizeof(generator));
  requests = 0;
}

/** @brief Report operational readiness, not cryptographic certification. */
bool ch32EntropyReady() {
  return initialized && status == CH32EntropyStatus::Ready &&
         requests < maximumRequests;
}

/** @brief Return the last initialization result. */
CH32EntropyStatus ch32EntropyStatus() { return status; }

/** @brief Count software RNG requests without exposing raw seeds or state. */
uint64_t ch32EntropyRequests() { return requests; }

/**
 * @brief BearSSL's standard system-seed hook, backed by HMAC_DRBG/SHA-256.
 * @param buffer Destination; unchanged on failure.
 * @param size Requested bytes, at most 256.
 * @return 0 on success, -1 with errno on failure. No fallback to random().
 * @note Main-loop use only; no concurrent calls or calls from interrupts.
 */
extern "C" int getentropy(void *buffer, size_t size) {
  if (size > 256) {
    errno = EIO;
    return -1;
  }
  if (!size) {
    return 0;
  }
  if (!buffer) {
    errno = EINVAL;
    return -1;
  }
  if (!ch32EntropyReady()) {
    errno = EAGAIN;
    return -1;
  }
  br_hmac_drbg_generate(&generator, buffer, size);
  requests++;
  return 0;
}

#if defined(CH32_ENTROPY_TEST)
/** @brief Seed only a diagnostic build from a synthetic fault-test source. */
bool ch32EntropyTestBegin(CH32EntropyReader reader) {
  ch32EntropyEnd();
  if (!reader) {
    return false;
  }
  return collect(reader, false);
}
#endif
