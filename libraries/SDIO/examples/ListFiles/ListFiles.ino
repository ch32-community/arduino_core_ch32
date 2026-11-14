#include <CH32SDIO.h>

CH32SDIO card;
FsVolume volume;

void setup() {
  Serial.begin(115200);
  while (!Serial) {
    delay(10);
  }
  Serial.print("CH32V407/467 native SDIO file listing\n\r");

  if (!card.begin()) {
    Serial.print("Card initialization failed. Error: ");
    Serial.print(card.error());
    Serial.print(" command: ");
    Serial.print(card.lastCommand());
    Serial.print(" status: 0x");
    Serial.print(card.status(), HEX);
    Serial.print("\n\r");
    return;
  }
  Serial.print("Card sectors: ");
  Serial.print(card.sectorCount());
  Serial.print(" SDIO clock (Hz): ");
  Serial.print(card.clockHz());
  Serial.print("\n\r");

  if (!volume.begin(&card)) {
    Serial.print("Could not mount an existing FAT/exFAT filesystem.\n\r");
    card.end();
    return;
  }
  Serial.print("Files in the root directory:\n\r");
  FsFile directory, entry;
  if (directory.open("/", O_RDONLY)) {
    while (entry.openNext(&directory, O_RDONLY)) {
      char name[128];
      entry.getName(name, sizeof(name));
      Serial.print(name);
      Serial.print(entry.isDirectory() ? "/\n\r" : "\n\r");
      entry.close();
    }
    directory.close();
  }
  volume.end();
  card.end();
}

void loop() {}
