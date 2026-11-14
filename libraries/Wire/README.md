# Wire

## CH32V407 / CH32V467 I3C controller

Boards can select `WIRE_I3C` to use the I3C peripheral
for hardware I2C transfers on PB11 (SDA) and PB10 (SCL). External pull-ups are
required. The normal I2C peripheral driver remains the default for other boards.

This backend currently supports controller mode, these default pins, 100 MHz
HCLK, and the 400 kHz clock setting. `Wire.begin()` selects that setting.
The timing registers select 1.5 us low and 1.1 us high, keeping the actual bus
rate below 400 kHz. The 8-bit timing fields cannot represent 100 kHz at this
HCLK. At 200 MHz even the longest low period is shorter than Fast-mode's
1.3 us minimum, so transfers are disabled at that clock. This backend does
not change the system clock or provide a software fallback.

The CH32V407 reference manual, section 19.4.1, requires `MEND=1` and nonzero
`DCNT` for legacy I2C messages. Consequently:

* `endTransmission(false)` returns 4 without sending the message.
* A zero-byte `endTransmission()` probe returns 4 without touching the bus.
* `requestFrom(..., false)` and the combined internal-address read overload
  return zero bytes without touching the bus.
* `endTransmission()` returns 2 for address NACK and 3 for data NACK.
  Other errors, including timeout, return 4; failed reads return zero bytes.
* Unsupported `setClock()` values disable transfers until a supported setting
  is restored. `setClock()` itself has no return value in the Arduino API.

Devices that accept STOP-separated command writes and reads can use ordinary
`Wire` calls. Devices requiring repeated START, zero-byte detection, or a
100 kHz bus are not supported by this backend. Unsupported operations are
never converted into a different bus transaction.

Transfers have a bounded timeout. Errors reset the controller and its FIFOs
before the next transfer. No bit-banged bus recovery is performed.

Reinitializing an active instance with unsupported pins or mode releases its
controller and the original PB11/PB10 pins, leaving unsupported pins untouched.
Destroying an instance releases its controller and buffers automatically; a
non-owner does not disturb the active bus. I3C `TwoWire` instances cannot be copied.

References: [WCH CH32V407 reference manual](https://www.wch.cn/downloads/CH32V407RM_PDF.html),
sections 19.4.1 and 19.4.16–18, and
[WCH AN16001](https://github.com/openwch/ch32_application_notes/blob/main/zh/AN16001/AN16001.md).

## CH32V407 / CH32V467 I2C peripheral (target)

With `WIRE_I3C` and `OPT_I2C_SLAVE=1`, a separate `TwoWire` instance can use
the native I2C1 peripheral as a target on PB9 (SDA) and PB8 (SCL). The I3C
controller remains available on its original pins. This target backend works
at 100 and 200 MHz CPU clocks; the I3C controller's clock restrictions above
still apply.

A board variant can define `PIN_WIRE1_SDA` and `PIN_WIRE1_SCL` to its digital
pin numbers for PB9 and PB8 to expose `Wire1` automatically. Otherwise, create
`TwoWire PeripheralWire(PB9, PB8)` using the variant's digital pin constants.
Connect external pull-ups to 3.3 V and a common ground with the controller.
The pins cannot simultaneously be used as GPIO or PWM.

Register `onReceive(callback)` and `onRequest(callback)` before calling
`begin(address)`. The target supports seven-bit addresses `0x08` through
`0x77`, repeated START, and address-only probes. Clock stretching must remain
enabled (the default). The external controller sets the bus rate, so
`setClock()` does not change it. Calling `begin()` without an address does not
enable controller mode on PB9/PB8. The default `Wire` instance remains a
controller on PB11/PB10.

Callbacks run in interrupt context: keep them short and avoid blocking, Serial
output, allocation, or bus reconfiguration. Receive messages retain the first
`I2C_TXRX_BUFFER_SIZE` bytes (32 by default), discarding any excess. An
`onRequest` callback runs once per read and can append up to that same limit
using `write()`; writes exceeding the remaining capacity return zero.
Reads beyond the queued reply return `0xff`. Calling `end()` releases the
target's address and pins without changing the I3C controller.

The `i2cMinimalSlave` example selects `Wire1` when the variant defines its pins.
