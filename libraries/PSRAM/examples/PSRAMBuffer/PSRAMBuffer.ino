void setup() {
  Serial.begin(115200);
  // Wait for the Serial Monitor. Remove this wait for standalone operation.
  while (!Serial) {
    delay(10);
  }
  delay(250);

  Serial.print("Adafruit Metro CH32V467 PSRAM buffer example\n\r");

  if (!getPSRAMSize()) {
    Serial.print("PSRAM is unavailable. Use a CH32V467RET6 with a 200 MHz system PLL and 100 or 200 MHz HCLK.\n\r");
    return;
  }

  Serial.print("PSRAM capacity in bytes: ");
  Serial.print(getPSRAMSize());
  Serial.print("\n\r");

  // Pico-compatible allocation; ordinary malloc()/new still use internal RAM.
  uint32_t *samples = (uint32_t *)pmalloc(16 * sizeof(uint32_t));
  if (!samples) {
    Serial.print("PSRAM allocation failed.\n\r");
    return;
  }
  for (uint32_t i = 0; i < 16; i++) {
    samples[i] = i * 100;
  }
  for (uint32_t i = 0; i < 16; i++) {
    Serial.print("Sample ");
    Serial.print(i);
    Serial.print(": ");
    Serial.print(samples[i]);
    Serial.print("\n\r");
  }

  // Ordinary realloc() recognizes PSRAM and preserves the existing samples.
  // Keep the old pointer until success so a failed resize cannot lose it.
  uint32_t *larger = (uint32_t *)realloc(samples, 32 * sizeof(uint32_t));
  if (larger) {
    samples = larger;
    samples[31] = 3100;
    Serial.print("Last sample after resizing: ");
    Serial.print(samples[31]);
    Serial.print("\n\r");
  } else {
    Serial.print("PSRAM resize failed; original samples are still allocated.\n\r");
  }
  free(samples);
  Serial.print("Free PSRAM heap in bytes: ");
  Serial.print(getFreePSRAMHeap());
  Serial.print("\n\r");
}

void loop() {}
