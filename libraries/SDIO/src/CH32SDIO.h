#pragma once

#include <Arduino.h>
#include <SdFat_Adafruit_Fork.h>

/** Native SD memory-card block device for the CH32V407/467.
 * Uses the remapped pins: CLK PC12, CMD PD2, D0 PB14, D1 PB15,
 * D2 PC10, D3 PC11. Use one instance and external card pull-ups.
 */
class CH32SDIO : public FsBlockDeviceInterface {
public:
  /** Failure category; status() provides the associated register bits. */
  enum Error : uint8_t {
    None,            ///< No error.
    Unsupported,     ///< Unsupported MCU or clock.
    InvalidArgument, ///< Null buffer, invalid range, or uninitialized device.
    Timeout,         ///< Command, initialization, or data operation timed out.
    Command,         ///< Command CRC or response mismatch.
    Card,            ///< The card reported an R1/R6 status error.
    Data,            ///< Data CRC, FIFO, or transfer failure.
    Capacity         ///< Unsupported CSD format or capacity.
  };

  bool begin(bool fourBit = true);
  void end() override;
  bool isBusy() override;
  bool readSector(Sector_t sector, uint8_t *data) override;
  bool readSectors(Sector_t sector, uint8_t *data, size_t count) override;
  bool writeSector(Sector_t sector, const uint8_t *data) override;
  bool writeSectors(Sector_t sector, const uint8_t *data,
                    size_t count) override;
  Sector_t sectorCount() override;
  bool syncDevice() override;
  Error error() const;
  uint32_t status() const;
  uint8_t lastCommand() const;
  uint32_t response() const;
  uint32_t clockHz() const;
  uint32_t cid[4] = {}; ///< Raw CID response, most significant word first.

private:
  static constexpr uint32_t sectorBytes = 512; ///< SD logical sector size.
  static constexpr uint32_t identificationDivider =
      255;                                         ///< CLKDIV: 100 MHz/257.
  static constexpr uint32_t transferHz = 10000000; ///< Conservative data clock.
  static constexpr uint32_t clockEnable = 1u << 8; ///< CLKCR clock enable.
  static constexpr uint32_t staticFlags = 0x00c007ff; ///< ICR clearable flags.
  static constexpr uint32_t cardErrors = 0xfdffe008;  ///< R1 error/locked mask.
  static constexpr uint32_t r6Errors =
      0xe000; ///< R6 CRC/illegal/general errors.
  static constexpr uint32_t readyTransfer = 0x900; ///< R1 ready, TRAN state.
  static constexpr uint32_t stateMask = 0x1f00; ///< R1 ready and current state.
  static constexpr uint32_t voltageWindow = 0x00ff8000; ///< OCR 2.7–3.6 V.
  static constexpr uint32_t highCapacity = 1u << 30;    ///< OCR HCS/CCS bit.
  static constexpr uint32_t poweredUp = 1u << 31; ///< OCR power-up complete.
  static constexpr uint32_t interfacePattern = 0x1aa; ///< CMD8 voltage/echo.
  static constexpr uint8_t setBlockLength = 16; ///< SDSC SET_BLOCKLEN command.
  bool command(uint8_t index, uint32_t argument, uint8_t responseType = 1,
               bool crc = true);
  bool cardReady();
  bool checkCardStatus();
  bool initialize(bool fourBit);
  bool transfer(Sector_t sector, uint8_t *data, bool write);
  bool validRange(Sector_t sector, const void *data, size_t count);
  bool fail(Error error, uint32_t status = 0);
  Sector_t _sectors = 0;  ///< Capacity in sectors.
  Error _error = None;    ///< Last failure category.
  uint32_t _status = 0;   ///< Register bits associated with the last failure.
  uint32_t _response = 0; ///< Most recent short response.
  uint32_t _clockHz = 0;  ///< Selected data clock.
  uint16_t _rca = 0;      ///< Relative card address.
  uint8_t _command = 0;   ///< Most recently issued command.
  bool _highCapacity = false; ///< True for block-addressed SDHC/SDXC cards.
  bool _ready = false;        ///< True after successful initialization.
  bool _controllerEnabled = false; ///< This instance has configured SDIO.
};
