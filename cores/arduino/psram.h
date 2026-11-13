#pragma once

#include <stdbool.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/** Initialize the PSRAM heap. Called automatically before setup().
 * Allocations also initialize it on first use, including in global
 * constructors.
 * @return True if the supported CH32V467 PSRAM is available.
 */
bool psramInit(void);

#if !defined(CH32V467)
#define CH32_PSRAM_ONLY                                                        \
  __attribute__((deprecated(                                                   \
      "This chip has no supported PSRAM; allocation returns null")))
#else
#define CH32_PSRAM_ONLY
#endif

/** Allocate from PSRAM, like Arduino-Pico. Release with ordinary free().
 * @param size Requested bytes.
 * @return Aligned storage, or null if unavailable, exhausted, or size is zero.
 */
CH32_PSRAM_ONLY void *pmalloc(size_t size);

/** Allocate zero-filled PSRAM, like Arduino-Pico. Release with free().
 * @param count Number of elements.
 * @param size Bytes per element.
 * @return Storage, or null on overflow, zero size, or allocation failure.
 */
CH32_PSRAM_ONLY void *pcalloc(size_t count, size_t size);
#undef CH32_PSRAM_ONLY

/** @return Physical PSRAM capacity in bytes, or zero if unavailable. */
size_t getPSRAMSize(void);
/** @return Initial heap capacity in bytes, excluding allocator metadata. */
size_t getTotalPSRAMHeap(void);
/** @return Currently free PSRAM bytes, which may be fragmented. */
size_t getFreePSRAMHeap(void);
/** @return Used PSRAM heap bytes, including per-allocation overhead. */
size_t getUsedPSRAMHeap(void);

#ifdef __cplusplus
}
#endif
