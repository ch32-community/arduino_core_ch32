// SPDX-FileCopyrightText: Copyright (c) 2026 Adafruit Industries
// SPDX-License-Identifier: BSD-3-Clause
#include "Arduino.h"
#include "twi.h"
#include "twi_i2c_target.h"

#if defined(WIRE_I3C)
#if !defined(CH32V4x7)
#error "WIRE_I3C requires CH32V407 or CH32V467"
#endif
#if defined(WIRE_SOFTWARE)
#error "Select only one Wire backend"
#endif

// There is one I3C peripheral. A second TwoWire instance must not reset a bus
// already owned by another instance.
static i2c_t *owner;

/** @brief Reset the controller and configure legacy I2C timing at 100 MHz. */
static void configureController() {
  I3C_DeInit();
  I3C_Ctrl_BusTypeDef timing = {};
  timing.SDAHoldTime = I3C_SDAHoldTime_1_5;
  timing.WaitTime = I3C_WaitTime_State_2;
  // At 100 MHz HCLK: 1.5 us low + 1.1 us high, below 400 kHz with margin
  // over Fast-mode's 1.3 us low and 0.6 us high minima (RM 19.4.16).
  timing.SCLPPLowDuration = 149;
  timing.SCLI3CHighDuration = 109;
  timing.SCLODLowDuration = 149;
  timing.SCLI2CHighDuration = 109;
  timing.BusFreeDuration = 127;
  timing.BusIdleDuration = 99; // 1 us time unit at 100 MHz HCLK.
  I3C_Ctrl_Init(&timing);

  I3C_CtrlConfTypeDef control = {};
  control.DynamicAddr = 0x22;
  I3C_Ctrl_Config(&control);
  I3C_FifoConfTypeDef fifo = {};
  fifo.ControlFifo = I3C_CONTROLFIFO_ENABLE;
  I3C_SetConfigFifo(&fifo);
  // Despite its name, ENABLE sets NOARBH: do not send the I3C 0x7e header
  // before a legacy I2C transaction. See CH32V407 RM section 19.4.3.
  I3C_ArbitrationHeaderCmd(ENABLE);
  I3C_Cmd(ENABLE);
}

/**
 * @brief Enable supported timing, or disable transfers for unsupported clocks.
 * @param obj Controller owner.
 * @param frequency Requested bus frequency in Hz; only 400000 is supported.
 */
void i2c_setTiming(i2c_t *obj, uint32_t frequency) {
  if (owner != obj)
    return;
  I3C_Cmd(DISABLE);
  RCC_ClocksTypeDef clocks;
  RCC_GetClocksFreq(&clocks);
  // Do not silently overclock a 100 kHz device. Additional rates/clock trees
  // need their own timing validation before enabling them here.
  obj->i3cReady = frequency == 400000 && clocks.HCLK_Frequency == 100000000;
  if (obj->i3cReady)
    configureController();
}

/**
 * @brief Select I3C controller mode or the separate I2C1 target backend.
 * @param obj Wire instance with its pins and mode selected.
 * @param frequency Requested bus frequency in Hz.
 * The final argument is the target's shifted seven-bit address; the addressing
 * mode argument is unused because these backends support seven-bit addresses.
 */
void i2c_custom_init(i2c_t *obj, uint32_t frequency, uint32_t,
                     uint32_t ownAddress) {
  obj->i3cReady = false;
#if OPT_I2C_SLAVE
  if (!obj->isMaster && obj->sda == PB_9 && obj->scl == PB_8) {
    i2c_target_init(obj, ownAddress);
    return;
  }
#endif
  if (owner && owner != obj)
    return;
  if (!obj->isMaster || obj->sda != PB_11 || obj->scl != PB_10) {
    // begin(sda, scl) already stored the rejected selection. Release any
    // existing ownership and the actual peripheral pins, not that selection.
    i2c_deinit(obj);
    return;
  }
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO | RCC_APB2Periph_GPIOB, ENABLE);
  RCC_AHBPeriphClockCmd(RCC_HBPeriph_I3C, ENABLE);
  GPIO_PinRemapConfig(GPIO_Remap_I3C, DISABLE);
  GPIO_InitTypeDef pins = {};
  pins.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
  pins.GPIO_Speed = GPIO_Speed_50MHz;
  // The I3C peripheral controls open-drain phases in legacy mode; the GPIO
  // alternate function itself must be AF_PP (RM table 10-7).
  pins.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOB, &pins);
  owner = obj;
  i2c_setTiming(obj, frequency);
}

/** @brief Initialize a Wire instance using the supported 400 kHz setting. */
void i2c_init(i2c_t *obj) { i2c_custom_init(obj, 400000, 0, 0); }

/** @brief Release the pins and controller only if this instance owns them. */
void i2c_deinit(i2c_t *obj) {
  obj->i3cReady = false;
#if OPT_I2C_SLAVE
  i2c_target_deinit(obj);
#endif
  if (owner != obj)
    return;
  I3C_DeInit();
  // These are the only supported peripheral pins. The caller may have changed
  // its requested pins through begin(sda, scl) or setSDA/setSCL since claiming.
  pinMode(pinNametoDigitalPin(PB_11), INPUT);
  pinMode(pinNametoDigitalPin(PB_10), INPUT);
  RCC_AHBPeriphClockCmd(RCC_HBPeriph_I3C, DISABLE);
  owner = nullptr;
}

/**
 * @brief Perform one bounded, nonempty, STOP-terminated legacy I2C transfer.
 * @param obj Controller owner.
 * @param address Seven-bit target address shifted left by one.
 * @param data Transmit or receive buffer.
 * @param size Number of bytes to transfer.
 * @param read True for a read, false for a write.
 * @param sendStop Must be true; repeated START is unsupported.
 * @return Transfer status, including NACK, timeout, or unsupported request.
 */
static i2c_status_e transfer(i2c_t *obj, uint8_t address, uint8_t *data,
                             uint16_t size, bool read, bool sendStop) {
  // RM 19.4.1 requires MEND=1 and a nonzero DCNT for legacy I2C. Never
  // substitute STOP for a requested repeated START, or send dummy probe data.
  if (owner != obj || !obj->i3cReady || !sendStop || !size || !data)
    return I2C_ERROR;
  if (!digitalReadFast(obj->scl) || !digitalReadFast(obj->sda))
    return I2C_BUSY;
  // The 7-bit FREE field cannot represent the 1.3 us I2C Fast-mode bus-free
  // interval at 100 MHz. Keep both lines released for at least 2 us instead.
  delayMicroseconds(2);
  if (!digitalReadFast(obj->scl) || !digitalReadFast(obj->sda))
    return I2C_BUSY;

  I3C_ClearFlag(I3C_FLAG_FCF | I3C_FLAG_ERRF);
  uint32_t direction = read ? I3C_Direction_RD : I3C_Direction_WR;
  uint32_t start = micros();
  // Allow the wire time plus 25 ms for servicing delays; every loop is bounded.
  uint32_t timeout = 25000 + (uint32_t)size * 25;
  I3C_ControllerHandleMessage(address >> 1, size, direction,
                              I3C_CONTROLLER_MTYPE_LEGACY_I2C,
                              I3C_GENERATE_STOP);
  uint16_t count = 0;
  i2c_status_e result = I2C_OK;
  while (true) {
    uint32_t events = I3C->EVR;
    if (events & I3C_EVR_ERRF) {
      uint32_t error = I3C->STATER;
      if (error & I3C_STATER_ANACK)
        result = I2C_NACK_ADDR;
      else if (error & I3C_STATER_DNACK)
        result = I2C_NACK_DATA;
      else
        result = I2C_ERROR;
      break;
    }
    // Each byte must wait for its FIFO request, even when several bytes fit
    // in the FIFO. Back-to-back register writes can lose a request on this IP.
    if (count < size) {
      if (read && (events & I3C_EVR_RXFNEF))
        data[count++] = I3C_ReadByte();
      else if (!read && (events & I3C_EVR_TXFNFF))
        I3C_WriteByte(data[count++]);
    }
    if ((events & I3C_EVR_FCF) && count == size) {
      I3C_ClearFlag(I3C_FLAG_FCF);
      return I2C_OK;
    }
    if (micros() - start >= timeout) {
      result = I2C_TIMEOUT;
      break;
    }
  }
  // Abort the peripheral and discard pending FIFO bytes so a failed transfer
  // cannot contaminate the next request. This does not bit-bang the bus.
  configureController();
  return result;
}

/** @brief Release SCL and allow a bounded clock-stretch interval. */
static bool probeClockHigh() {
  GPIO_SetBits(GPIOB, GPIO_Pin_10);
  uint32_t start = micros();
  while (!GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_10)) {
    if (micros() - start >= 1000)
      return false;
  }
  delayMicroseconds(5);
  return true;
}

/**
 * @brief Send a real address-only write using open-drain GPIO, then restore
 * I3C. Like Arduino-Pico's Wire probe, this handles a transaction the hardware
 * cannot express. No dummy data byte is sent and success requires a real ACK.
 * Data transfers remain in hardware. This is a single-controller bus path.
 */
static i2c_status_e probeAddress(i2c_t *obj, uint8_t address, bool sendStop) {
  if (owner != obj || !obj->i3cReady || !sendStop)
    return I2C_ERROR;
  if (!digitalReadFast(obj->scl) || !digitalReadFast(obj->sda))
    return I2C_BUSY;
  delayMicroseconds(5);
  if (!digitalReadFast(obj->scl) || !digitalReadFast(obj->sda))
    return I2C_BUSY;

  I3C_Cmd(DISABLE);
  // Set the output latches to released before selecting open-drain GPIO.
  GPIO_SetBits(GPIOB, GPIO_Pin_10 | GPIO_Pin_11);
  GPIO_InitTypeDef pins = {};
  pins.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
  pins.GPIO_Speed = GPIO_Speed_50MHz;
  pins.GPIO_Mode = GPIO_Mode_Out_OD;
  GPIO_Init(GPIOB, &pins);
  i2c_status_e result = I2C_TIMEOUT;
  if (probeClockHigh()) {
    GPIO_ResetBits(GPIOB, GPIO_Pin_11); // START while SCL is high.
    delayMicroseconds(5);
    bool clockOK = true;
    for (uint8_t mask = 0x80; mask; mask >>= 1) {
      GPIO_ResetBits(GPIOB, GPIO_Pin_10);
      // Keep SDA stable while SCL falls. delayMicroseconds subtracts 1 us
      // from nonzero requests, so request 2 us for at least 1 us of waiting.
      delayMicroseconds(2);
      if (address & mask)
        GPIO_SetBits(GPIOB, GPIO_Pin_11);
      else
        GPIO_ResetBits(GPIOB, GPIO_Pin_11);
      delayMicroseconds(5);
      if (!probeClockHigh()) {
        clockOK = false;
        break;
      }
    }
    if (clockOK) {
      GPIO_ResetBits(GPIOB, GPIO_Pin_10);
      delayMicroseconds(2); // Include the core's 1 us delay compensation.
      GPIO_SetBits(GPIOB, GPIO_Pin_11); // Release SDA for the target's ACK.
      delayMicroseconds(5);
      if (probeClockHigh())
        result =
            GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_11) ? I2C_NACK_ADDR : I2C_OK;
    }
  }
  // STOP, also on a NACK. A held-low SCL is bounded and reported as timeout.
  GPIO_ResetBits(GPIOB, GPIO_Pin_10);
  delayMicroseconds(2); // Hold SDA through SCL falling, including compensation.
  GPIO_ResetBits(GPIOB, GPIO_Pin_11);
  delayMicroseconds(5);
  if (!probeClockHigh())
    result = I2C_TIMEOUT;
  GPIO_SetBits(GPIOB, GPIO_Pin_11);
  delayMicroseconds(5);
  if (result == I2C_OK && !GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_11))
    result = I2C_BUSY;

  // Keep the bus released by GPIO while resetting/configuring I3C. Exposing
  // the disabled controller first can create an extra SCL edge after STOP.
  configureController();
  pins.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOB, &pins);
  return result;
}

/** @brief Write one message, or probe an address without sending data. */
i2c_status_e i2c_master_write(i2c_t *obj, uint8_t address, uint8_t *data,
                              uint16_t size, uint8_t sendStop) {
  if (!size)
    return probeAddress(obj, address, sendStop);
  return transfer(obj, address, data, size, false, sendStop);
}

/** @brief Read one STOP-terminated message and return its transfer status. */
i2c_status_e i2c_master_read(i2c_t *obj, uint8_t address, uint8_t *data,
                             uint16_t size) {
  return transfer(obj, address, data, size, true, true);
}
#endif
