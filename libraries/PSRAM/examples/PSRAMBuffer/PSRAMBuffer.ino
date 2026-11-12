#include <CH32PSRAM.h>

CH32PSRAM ram;

void setup() {
  Serial.begin(115200);
  while (!Serial) {
    delay(10);
  }
  Serial.print("CH32V467 PSRAM buffer example\n\r");

  if (!ram.begin()) {
    Serial.print("PSRAM is unavailable. Use a CH32V467RET6 with a 200 MHz system PLL and 100 or 200 MHz HCLK.\n\r");
    return;
  }

  Serial.print("PSRAM capacity in bytes: ");
  Serial.print(ram.size());
  Serial.print("\n\r");

  // Use aligned 32-bit elements. This buffer is separate from malloc()/new.
  volatile uint32_t *samples = ram.data();
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
}

void loop() {}
