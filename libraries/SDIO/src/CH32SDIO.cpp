/* Native SDIO: WCH CH32V407 RM v1.2 chapter 24 and the SD Physical Layer
 * command sequence. Adapted from the Metro CH32V407/467 bring-up driver. */
#include "CH32SDIO.h"
#include <string.h>

#if defined(CH32V4x7)
#include "ch32v4x7_sdio.h"
#endif

/** @brief Save a failure and its hardware details. @return Always false. */
bool CH32SDIO::fail(Error error, uint32_t status) {
  _error = error;
  _status = status;
  return false;
}

/**
 * @brief Initialize a card on the remapped SDIO pins without formatting it.
 * @param fourBit True for four data lines; false to use only D0.
 * @note At 200 MHz HCLK, the minimum initialization clock exceeds 400 kHz;
 * some cards may fail to initialize. Use 100 MHz for compliant timing.
 * @return True on success. Inspect error(), status(), and lastCommand() on
 * failure.
 */
bool CH32SDIO::begin(bool fourBit) {
  end();
  _error = None;
  _status = _response = 0;
  _command = 0;
  _rca = 0;
  _highCapacity = false;
  memset(cid, 0, sizeof(cid));
#if defined(CH32V4x7)
  if (SystemCoreClock != 100000000 && SystemCoreClock != 200000000) {
    return fail(Unsupported);
  }
  if (!initialize(fourBit)) {
    end();
    return false;
  }
  _ready = true;
  return true;
#else
  (void)fourBit;
  return fail(Unsupported);
#endif
}

#if defined(CH32V4x7)
/**
 * @brief Issue a command and wait for its response or completion.
 * @param index SD command number.
 * @param argument Command argument.
 * @param responseType 0 for none, 1 for short, 3 for long.
 * @param crc Whether the response has a valid CRC (false for OCR).
 * @return True when the controller accepts the response.
 */
bool CH32SDIO::command(uint8_t index, uint32_t argument, uint8_t responseType,
                       bool crc) {
  _command = index;
  _response = 0;
  const uint32_t flags = SDIO_FLAG_CCRCFAIL | SDIO_FLAG_CTIMEOUT |
                         SDIO_FLAG_CMDREND | SDIO_FLAG_CMDSENT;
  SDIO->ICR = flags;
  SDIO->ARG = argument;
  SDIO->CMD = index | ((uint32_t)responseType << 6) | SDIO_CPSM_Enable;
  uint32_t start = millis();
  uint32_t status;
  do {
    status = SDIO->STA;
    if (millis() - start >= 100) {
      return fail(Timeout, status);
    }
  } while (!(status & flags));
  _response = SDIO->RESP1;
  if (status & SDIO_FLAG_CTIMEOUT) {
    return fail(Timeout, status);
  }
  if (crc && (status & SDIO_FLAG_CCRCFAIL)) {
    return fail(Command, status);
  }
  // R2 and R3 have no echoed command index; CRC-bearing short replies do.
  if (responseType == 1 && crc && (SDIO->RESPCMD & 0x3f) != index) {
    return fail(Command, SDIO->RESPCMD);
  }
  return true;
}

/** @brief Check the last R1 response. @return True if it has no card errors. */
bool CH32SDIO::checkCardStatus() {
  return !(_response & cardErrors) || fail(Card, _response);
}

/** @brief Configure the controller and enumerate a card.
 * @param fourBit Whether to switch the card and controller to four data lines.
 * @return True if the card is ready for sector transfers.
 */
bool CH32SDIO::initialize(bool fourBit) {
  // WCH's SDIO example enables both clocks. Do not reset the shared MAC clock.
  RCC_HBPeriphClockCmd(RCC_HBPeriph_SDIO | RCC_HBPeriph_ETH_MAC, ENABLE);
  RCC_PB2PeriphClockCmd(RCC_PB2Periph_AFIO | RCC_PB2Periph_GPIOB |
                            RCC_PB2Periph_GPIOC | RCC_PB2Periph_GPIOD,
                        ENABLE);
  GPIO_PinRemapConfig(GPIO_Remap_SDIO, ENABLE);
  GPIO_InitTypeDef pins = {};
  pins.GPIO_Mode = GPIO_Mode_AF_PP;
  pins.GPIO_Speed = GPIO_Speed_High;
  pins.GPIO_Pin = GPIO_Pin_14 | GPIO_Pin_15;
  GPIO_Init(GPIOB, &pins);
  pins.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12;
  GPIO_Init(GPIOC, &pins);
  pins.GPIO_Pin = GPIO_Pin_2;
  GPIO_Init(GPIOD, &pins);
  _controllerEnabled = true;
  SDIO->POWER = SDIO_PowerState_OFF;
  SDIO->CLKCR = 0;
  SDIO->CMD = 0;
  SDIO->DCTRL = 0;
  SDIO->MASK = 0;
  SDIO->ICR = staticFlags;
  // RM 24.6.2: SDIO_CK = HCLK / (CLKDIV + 2). CLKDIV=255 gives the slowest
  // clock: about 389 kHz at 100 MHz HCLK, or 778 kHz at 200 MHz HCLK.
  // The latter exceeds the 400 kHz initialization limit; some cards may fail.
  SDIO->CLKCR = identificationDivider | SDIO_ClockEdge_Falling | clockEnable;
  SDIO->POWER = SDIO_PowerState_ON;
  delay(5); // At least 74 idle clocks before CMD0.
  if (!command(CMD0, 0, 0)) {
    return false;
  }
  bool version2 = command(CMD8, interfacePattern);
  if (version2 && (_response & 0xfff) != interfacePattern) {
    return fail(Command, _response);
  }
  // A legacy SD card may time out on CMD8; CRC and other failures are fatal.
  if (!version2 && _error != Timeout) {
    return false;
  }
  _error = None;
  _status = 0;
  uint32_t start = millis();
  uint32_t ocr;
  do {
    if (!command(CMD55, 0) || !checkCardStatus() ||
        !command(ACMD41, voltageWindow | (version2 ? highCapacity : 0), 1,
                 false)) {
      return false;
    }
    ocr = _response;
    if (millis() - start >= 1500) {
      return fail(Timeout, ocr);
    }
    delay(1);
  } while (!(ocr & poweredUp));
  if (!(ocr & voltageWindow)) {
    return fail(Card, ocr);
  }
  _highCapacity = version2 && (ocr & highCapacity);
  if (!command(CMD2, 0, 3)) {
    return false;
  }
  cid[0] = SDIO->RESP1;
  cid[1] = SDIO->RESP2;
  cid[2] = SDIO->RESP3;
  cid[3] = SDIO->RESP4;
  if (!command(CMD3, 0)) {
    return false;
  }
  if (_response & r6Errors) {
    return fail(Card, _response);
  }
  _rca = _response >> 16;
  if (!_rca) {
    return fail(Card, _response);
  }
  if (!command(CMD9, (uint32_t)_rca << 16, 3)) {
    return false;
  }
  uint32_t csd[4] = {SDIO->RESP1, SDIO->RESP2, SDIO->RESP3, SDIO->RESP4};
  uint64_t sectors;
  if ((csd[0] >> 30) == 1 && _highCapacity) {
    uint32_t size = ((csd[1] & 0x3f) << 16) | (csd[2] >> 16);
    sectors = ((uint64_t)size + 1) * 1024;
  } else if ((csd[0] >> 30) == 0 && !_highCapacity) {
    uint32_t blockBits = (csd[1] >> 16) & 15;
    uint32_t size = ((csd[1] & 0x3ff) << 2) | (csd[2] >> 30);
    uint32_t multiplier = (csd[2] >> 15) & 7;
    sectors =
        (((uint64_t)size + 1) << (multiplier + 2 + blockBits)) / sectorBytes;
  } else {
    return fail(Capacity, csd[0]);
  }
  // SDSC commands use byte addresses; do not let a malformed CSD wrap them.
  if (!sectors || sectors > UINT32_MAX ||
      (!_highCapacity && sectors > (UINT64_C(1) << 32) / sectorBytes)) {
    return fail(Capacity);
  }
  _sectors = sectors;
  if (!command(CMD7, (uint32_t)_rca << 16) || !checkCardStatus() ||
      !cardReady()) {
    return false;
  }
  if (!_highCapacity &&
      (!command(setBlockLength, sectorBytes) || !checkCardStatus())) {
    return false;
  }
  if (fourBit && (!command(CMD55, (uint32_t)_rca << 16) || !checkCardStatus() ||
                  !command(ACMD6, 2) || !checkCardStatus())) {
    return false;
  }
  uint32_t divisor = (SystemCoreClock + transferHz - 1) / transferHz;
  SDIO->CLKCR = (divisor - 2) | clockEnable | SDIO_ClockEdge_Falling |
                SDIO_HardwareFlowControl_Enable |
                (fourBit ? SDIO_BusWide_4b : SDIO_BusWide_1b);
  _clockHz = SystemCoreClock / divisor;
  return true;
}

/** @brief Wait for READY_FOR_DATA and TRAN state. @return True when ready. */
bool CH32SDIO::cardReady() {
  uint32_t start = millis();
  do {
    if (!command(CMD13, (uint32_t)_rca << 16) || !checkCardStatus()) {
      return false;
    }
    if ((_response & stateMask) == readyTransfer) {
      return true;
    }
  } while (millis() - start < 500);
  return fail(Timeout, _response);
}
#endif

/** @brief Validate a buffer and sector range.
 * @param sector First sector.
 * @param data Buffer (may be null only for a zero-length request).
 * @param count Number of sectors.
 * @return True if the initialized device and buffer range are valid.
 */
bool CH32SDIO::validRange(Sector_t sector, const void *data, size_t count) {
  if (!_ready || sector > _sectors || count > _sectors - sector ||
      count > SIZE_MAX / sectorBytes || (count && !data)) {
    return fail(InvalidArgument);
  }
  _error = None;
  _status = 0;
  return true;
}

/** @brief Transfer one 512-byte sector with hardware CRC and bounded waits.
 * @param sector Logical sector.
 * @param data Read destination or write source; no alignment requirement.
 * @param write True for a write, false for a read.
 * @return True when all bytes and the card's CRC/status complete successfully.
 */
bool CH32SDIO::transfer(Sector_t sector, uint8_t *data, bool write) {
  if (!validRange(sector, data, 1)) {
    return false;
  }
#if defined(CH32V4x7)
  if (!cardReady()) {
    end();
    return false;
  }
  SDIO->DCTRL = 0;
  SDIO->ICR = staticFlags;
  SDIO->DTIMER = _clockHz / 2; // 500 ms hardware data timeout.
  SDIO->DLEN = sectorBytes;
  if (!write) {
    SDIO->DCTRL =
        SDIO_DataBlockSize_512b | SDIO_TransferDir_ToSDIO | SDIO_DPSM_Enable;
  }
  uint32_t address = _highCapacity ? sector : sector * sectorBytes;
  if (!command(write ? CMD24 : CMD17, address) || !checkCardStatus()) {
    SDIO->DCTRL = 0;
    end();
    return false;
  }
  if (write) {
    SDIO->DCTRL =
        SDIO_DataBlockSize_512b | SDIO_TransferDir_ToCard | SDIO_DPSM_Enable;
  }
  unsigned words = 0;
  uint32_t start = millis();
  const uint32_t errors = SDIO_FLAG_DCRCFAIL | SDIO_FLAG_DTIMEOUT |
                          SDIO_FLAG_TXUNDERR | SDIO_FLAG_RXOVERR |
                          SDIO_FLAG_STBITERR;
  while (true) {
    uint32_t status = SDIO->STA;
    if (status & errors) {
      fail(Data, status);
      break;
    }
    if (!write) {
      while ((SDIO->STA & SDIO_FLAG_RXDAVL) && words < sectorBytes / 4) {
        uint32_t value = SDIO->FIFO;
        memcpy(data + 4 * words++, &value, 4);
      }
    } else if ((status & SDIO_FLAG_TXFIFOHE) && words < sectorBytes / 4) {
      for (unsigned n = 0; n < 8 && words < sectorBytes / 4; n++) {
        uint32_t value;
        memcpy(&value, data + 4 * words++, 4);
        SDIO->FIFO = value;
      }
    }
    if ((SDIO->STA & SDIO_FLAG_DBCKEND) && words == sectorBytes / 4) {
      // DATAEND only marks the byte count; DBCKEND also waits for the CRC.
      if (SDIO->STA & errors) {
        fail(Data, SDIO->STA);
        break;
      }
      SDIO->DCTRL = 0;
      SDIO->ICR = staticFlags;
      if (write && !cardReady()) {
        end();
        return false;
      }
      return true;
    }
    if (millis() - start >= 600) {
      fail(Timeout, SDIO->STA);
      break;
    }
  }
  SDIO->DCTRL = 0;
  SDIO->ICR = staticFlags;
  // Require reinitialization after an incomplete data transfer.
  end();
  return false;
#else
  (void)write;
  return fail(Unsupported);
#endif
}

/** @brief Read one sector. @param sector Sector number. @param data
 * Destination.
 * @return True on success. */
bool CH32SDIO::readSector(Sector_t sector, uint8_t *data) {
  return transfer(sector, data, false);
}
/** @brief Write one sector. @param sector Sector number. @param data Source.
 * @return True on success. */
bool CH32SDIO::writeSector(Sector_t sector, const uint8_t *data) {
  return transfer(sector, const_cast<uint8_t *>(data), true);
}
/** @brief Read sectors using single-block commands.
 * @param sector First sector. @param data Destination. @param count Sector
 * count.
 * @return True on success. */
bool CH32SDIO::readSectors(Sector_t sector, uint8_t *data, size_t count) {
  if (!validRange(sector, data, count)) {
    return false;
  }
  for (size_t i = 0; i < count; i++) {
    if (!readSector(sector + i, data + sectorBytes * i)) {
      return false;
    }
  }
  return true;
}
/** @brief Write sectors using single-block commands.
 * @param sector First sector. @param data Source. @param count Sector count.
 * @return True on success. */
bool CH32SDIO::writeSectors(Sector_t sector, const uint8_t *data,
                            size_t count) {
  if (!validRange(sector, data, count)) {
    return false;
  }
  for (size_t i = 0; i < count; i++) {
    if (!writeSector(sector + i, data + sectorBytes * i)) {
      return false;
    }
  }
  return true;
}
/** @brief Wait for outstanding card programming. @return True when ready. */
bool CH32SDIO::syncDevice() {
  if (!_ready) {
    return fail(InvalidArgument);
  }
#if defined(CH32V4x7)
  _error = None;
  _status = 0;
  return cardReady();
#else
  return fail(Unsupported);
#endif
}
/** @brief Query card readiness. @return True when busy, uninitialized, or
 * failed. */
bool CH32SDIO::isBusy() {
#if defined(CH32V4x7)
  if (!_ready || !command(CMD13, (uint32_t)_rca << 16) || !checkCardStatus()) {
    return true;
  }
  return (_response & stateMask) != readyTransfer;
#else
  return true;
#endif
}
/** @brief Stop SDIO. Close files and end the filesystem before calling this. */
void CH32SDIO::end() {
#if defined(CH32V4x7)
  if (_controllerEnabled) {
    SDIO->DCTRL = 0;
    SDIO->CMD = 0;
    SDIO->CLKCR = 0;
    SDIO->POWER = SDIO_PowerState_OFF;
  }
#endif
  _controllerEnabled = false;
  _ready = false;
  _sectors = 0;
  _clockHz = 0;
}
/** @return Capacity in sectors, or zero when uninitialized. */
Sector_t CH32SDIO::sectorCount() { return _ready ? _sectors : 0; }
/** @return Most recent failure category. */
CH32SDIO::Error CH32SDIO::error() const { return _error; }
/** @return Hardware status associated with the most recent failure. */
uint32_t CH32SDIO::status() const { return _status; }
/** @return Most recently issued SD command number. */
uint8_t CH32SDIO::lastCommand() const { return _command; }
/** @return Most recent cached short response, without accessing hardware. */
uint32_t CH32SDIO::response() const { return _response; }
/** @return Current data clock in Hz, or zero when stopped. */
uint32_t CH32SDIO::clockHz() const { return _clockHz; }
