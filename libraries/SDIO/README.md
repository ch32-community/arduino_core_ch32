# CH32SDIO

Native SD memory-card access for CH32V407/467 at 100 or 200 MHz HCLK.
Install **SdFat - Adafruit Fork 2.3.103 or newer**. `CH32SDIO` implements its
`FsBlockDeviceInterface`; mount an existing FAT/exFAT filesystem with
`FsVolume::begin(&card)` as shown in `ListFiles`.

The driver uses the remapped SDIO pins, matching the Adafruit Metro socket:

| Signal | Pin |
| --- | --- |
| CLK | PC12 |
| CMD | PD2 |
| D0 | PB14 |
| D1 | PB15 |
| D2 | PC10 |
| D3 | PC11 |

External card pull-ups are required. Card-detect is not part of this interface;
applications can read their board's switch separately. `begin()` uses four data
lines; `begin(false)` selects one-bit operation. The bus initializes at no more
than 400 kHz and transfers data at 10 MHz. Both modes reserve the pins above.

Use one instance and call it from ordinary foreground code with interrupts
enabled. Transfers poll the FIFO with hardware flow control, allowing USB
interrupts to run. Data and command CRC errors and timeouts are reported through
`error()`, with the corresponding register bits in `status()`. `lastCommand()`
and `response()` provide command diagnostics. A failed data transfer requires
`begin()` again; close files and end the volume before reinitializing.

Sector buffers need not be aligned. Multi-sector methods currently repeat
single-block commands; DMA and streaming multi-block commands are not provided.
SD/SDSC and SDHC/SDXC CSD formats are handled; SDUC, MMC, SDIO function cards,
and capacities requiring more than `UINT32_MAX` sectors are unsupported.
Other MCU families and HCLK settings return `Unsupported`.

This driver never formats the card. Close and sync files, then call
`volume.end()` before `card.end()` or removing power. Do not remove a card during
writes. The `ListFiles` example only reads the filesystem. Its `.none.test.only`
marker excludes it from the core's generic board matrix; compile it explicitly
for a CH32V407/467 board with the pin assignment and clock settings above.
