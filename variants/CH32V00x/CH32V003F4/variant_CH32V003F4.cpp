/**
 *******************************************************************************
 * Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
 * All rights reserved.
 *
 * This software component is licensed by WCH under BSD 3-Clause license,
 * the "License"; You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 *                        opensource.org/licenses/BSD-3-Clause
 *
 *******************************************************************************
 */

#include "pins_arduino.h"




// Digital PinName array,Some GPIOs are bound to the same pin. 
const PinName digitalPin[] = VARIANT_DIGITAL_PIN_MAP;
const uint32_t analogInputPin[] = VARIANT_ANALOG_PIN_MAP;
