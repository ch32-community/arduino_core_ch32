// Pico-compatible PSRAM allocation. Normal malloc/new retain the internal heap.
#include "Arduino.h"
#include <errno.h>
#include <reent.h>

#if defined(CH32V467)
#include "ch32/psram/CH32PSRAM.h"
// Use the same BSD-licensed TLSF allocator as Arduino-Pico. Keep it outside
// the core source tree so Arduino does not also compile a second copy.
#include "../../system/Drivers/tlsf/tlsf.c"

namespace {
CH32PSRAM hardware;
tlsf_t heap;
size_t initialFree;
constexpr uintptr_t psramBase = 0x80000000u;
constexpr size_t psramBytes = 4 * 1024 * 1024;

// QingKe V3V gates interrupts in GINTENR, not the standard mstatus MIE bit.
// Preserve the caller's state, including calls made with interrupts disabled.
class HeapLock {
public:
  HeapLock() {
    asm volatile("csrrc %0, 0x800, %1" : "=r"(saved) : "r"(0x88) : "memory");
    asm volatile("fence.i" ::: "memory");
  }
  ~HeapLock() { asm volatile("csrw 0x800, %0" : : "r"(saved) : "memory"); }

private:
  uint32_t saved;
};

bool isPSRAM(const void *ptr) {
  uintptr_t address = (uintptr_t)ptr;
  return address >= psramBase && address - psramBase < psramBytes;
}

bool countFree(void *, size_t size, int used, void *context) {
  if (!used) {
    *(size_t *)context += size;
  }
  return true;
}

size_t freeBytes() {
  size_t available = 0;
  tlsf_walk_pool(tlsf_get_pool(heap), countFree, &available);
  return available;
}

void releasePSRAM(void *ptr) {
  HeapLock lock;
  tlsf_free(heap, ptr);
}

void *resizePSRAM(struct _reent *context, void *ptr, size_t size) {
  if (!size) {
    releasePSRAM(ptr);
    return nullptr;
  }
  size_t oldSize;
  {
    HeapLock lock;
    oldSize = tlsf_block_size(ptr);
    // TLSF shrinking stays in place, preserving the original alignment.
    if (size <= oldSize) {
      return tlsf_realloc(heap, ptr, size);
    }
  }
  // TLSF's ordinary realloc may move to a merely 4-byte-aligned block.
  // Allocate explicitly to preserve the C/C++ maximum fundamental alignment.
  void *replacement = pmalloc(size);
  if (!replacement) {
    context->_errno = ENOMEM;
    return nullptr;
  }
  // Copy outside the allocator lock so a large buffer does not starve USB.
  memcpy(replacement, ptr, oldSize);
  releasePSRAM(ptr);
  return replacement;
}
} // namespace
#endif

bool psramInit(void) {
#if defined(CH32V467)
  if (heap) {
    return true;
  }
  // Initialization has bounded polling and needs neither SysTick nor IRQs.
  // Use foreground code; allocation from interrupt handlers is not supported.
  if (!hardware.begin()) {
    return false;
  }
  HeapLock lock;
  heap = tlsf_create_with_pool((void *)psramBase, psramBytes, psramBytes);
  if (heap) {
    initialFree = freeBytes();
  }
  return heap != nullptr;
#else
  return false;
#endif
}

void *pmalloc(size_t size) {
#if defined(CH32V467)
  if (!size || size >= psramBytes || !psramInit()) {
    return nullptr;
  }
  HeapLock lock;
  return tlsf_memalign(heap, alignof(max_align_t), size);
#else
  (void)size;
  return nullptr;
#endif
}

void *pcalloc(size_t count, size_t size) {
#if defined(CH32V467)
  if (!size || count > SIZE_MAX / size) {
    return nullptr;
  }
  size_t bytes = count * size;
  void *ptr = pmalloc(bytes);
  if (ptr) {
    memset(ptr, 0, bytes);
  }
  return ptr;
#else
  (void)count;
  (void)size;
  return nullptr;
#endif
}

size_t getPSRAMSize(void) {
#if defined(CH32V467)
  return psramInit() ? psramBytes : 0;
#else
  return 0;
#endif
}

size_t getTotalPSRAMHeap(void) {
#if defined(CH32V467)
  return psramInit() ? initialFree : 0;
#else
  return 0;
#endif
}

size_t getFreePSRAMHeap(void) {
#if defined(CH32V467)
  if (!psramInit()) {
    return 0;
  }
  HeapLock lock;
  return freeBytes();
#else
  return 0;
#endif
}

size_t getUsedPSRAMHeap(void) {
  return getTotalPSRAMHeap() - getFreePSRAMHeap();
}

// Wrap newlib's reentrant entry points, also reached by ordinary free/realloc
// and C++ delete. Internal pointers are forwarded to the original allocator.
extern "C" void __real__free_r(struct _reent *context, void *ptr);
extern "C" void *__real__realloc_r(struct _reent *context, void *ptr,
                                   size_t size);

extern "C" void __wrap__free_r(struct _reent *context, void *ptr) {
#if defined(CH32V467)
  if (isPSRAM(ptr)) {
    releasePSRAM(ptr);
    return;
  }
#endif
  __real__free_r(context, ptr);
}

extern "C" void *__wrap__realloc_r(struct _reent *context, void *ptr,
                                   size_t size) {
#if defined(CH32V467)
  if (isPSRAM(ptr)) {
    return resizePSRAM(context, ptr, size);
  }
#endif
  return __real__realloc_r(context, ptr, size);
}
