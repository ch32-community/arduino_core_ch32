// Link coverage for the newlib allocator wrappers on every CI processor.
// Inspect heapChecksPassed if this sketch is run on hardware.
volatile bool heapChecksPassed;
volatile size_t allocationBytes = 32;

void setup() {
  size_t bytes = allocationBytes;
  bool passed = true;
  uint8_t *buffer = (uint8_t *)malloc(bytes);
  if (!buffer) {
    heapChecksPassed = false;
    return;
  }
  memset(buffer, 42, bytes);
  uint8_t *larger = (uint8_t *)realloc(buffer, bytes * 2);
  passed = larger != nullptr;
  if (larger) {
    buffer = larger;
    for (size_t i = 0; i < bytes; i++) passed = passed && buffer[i] == 42;
  }
  free(buffer);

  buffer = (uint8_t *)calloc(bytes, 1);
  passed = passed && buffer;
  if (buffer) {
    for (size_t i = 0; i < bytes; i++) passed = passed && buffer[i] == 0;
  }
  free(buffer);

  // Unsupported targets deliberately warn on these calls and return null.
  buffer = (uint8_t *)pmalloc(bytes);
  passed = passed && (getPSRAMSize() ? buffer != nullptr : buffer == nullptr);
  free(buffer);
  buffer = (uint8_t *)pcalloc(bytes, 1);
  passed = passed && (getPSRAMSize() ? buffer != nullptr : buffer == nullptr);
  free(buffer);
  heapChecksPassed = passed;
}

void loop() {}
