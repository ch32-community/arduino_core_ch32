/*
 *******************************************************************************
 * Copyright (c) 2017, STMicroelectronics
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. Neither the name of STMicroelectronics nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *******************************************************************************
 Modified 27 may 2024 by TempersLee for CH32V/M00X series.
 */

#ifndef _PINAF_CH32V00X_H
#define _PINAF_CH32V00X_H

#include "variant.h"   // Needed to allow testing if peripheral is enabled

#ifdef __cplusplus
extern "C" {
#endif

enum {
  AFIO_NONE,

  AFIO_PartialRemap1_SPI1_ENABLE,
  AFIO_PartialRemap2_SPI1_ENABLE,
  AFIO_PartialRemap3_SPI1_ENABLE,
  AFIO_PartialRemap4_SPI1_ENABLE,
  AFIO_PartialRemap5_SPI1_ENABLE,
  AFIO_FullRemap_SPI1_ENABLE,      
  AFIO_Remap_SPI1_DISABLE,
  
  AFIO_PartialRemap1_I2C1_ENABLE,
  AFIO_PartialRemap2_I2C1_ENABLE,
  AFIO_PartialRemap3_I2C1_ENABLE,
  AFIO_FullRemap_I2C1_ENABLE,
  AFIO_Remap_I2C1_DISABLE,

  AFIO_PartialRemap1_USART1_ENABLE,
  AFIO_PartialRemap2_USART1_ENABLE,
  AFIO_PartialRemap3_USART1_ENABLE,
  AFIO_PartialRemap4_USART1_ENABLE,
  AFIO_PartialRemap5_USART1_ENABLE,
  AFIO_PartialRemap6_USART1_ENABLE,
  AFIO_PartialRemap7_USART1_ENABLE,
  AFIO_PartialRemap8_USART1_ENABLE,      
  AFIO_FullRemap_USART1_ENABLE,
  AFIO_Remap_USART1_DISABLE,

  AFIO_PartialRemap1_TIM1_ENABLE,
  AFIO_PartialRemap2_TIM1_ENABLE,
  AFIO_PartialRemap3_TIM1_ENABLE,
  AFIO_PartialRemap4_TIM1_ENABLE,
  AFIO_PartialRemap5_TIM1_ENABLE,
  AFIO_PartialRemap6_TIM1_ENABLE,
  AFIO_PartialRemap7_TIM1_ENABLE,
  AFIO_PartialRemap8_TIM1_ENABLE,            
  AFIO_FullRemap_TIM1_ENABLE,
  AFIO_Remap_TIM1_DISABLE,

  AFIO_PartialRemap1_TIM2_ENABLE,
  AFIO_PartialRemap2_TIM2_ENABLE,
  AFIO_PartialRemap3_TIM2_ENABLE,
  AFIO_PartialRemap4_TIM2_ENABLE,
  AFIO_PartialRemap5_TIM2_ENABLE,
  AFIO_PartialRemap6_TIM2_ENABLE,        
  AFIO_FullRemap_TIM2_ENABLE,
  AFIO_Remap_TIM2_DISABLE,

  AFIO_Remap_PA1_2_ENABLE,
  AFIO_Remap_PA1_2_DISABLE,

  AFIO_Remap_ADC1_ETRGINJ_ENBALE,
  AFIO_Remap_ADC1_ETRGINJ_DISABLE,
  AFIO_Remap_ADC1_ETRGREG_ENABLE,
  AFIO_Remap_ADC1_ETRGREG_DISABLE,

  AFIO_PartialRemap1_USART2_ENABLE,  
  AFIO_PartialRemap2_USART2_ENABLE,
  AFIO_PartialRemap3_USART2_ENABLE,
  AFIO_PartialRemap4_USART2_ENABLE,
  AFIO_PartialRemap5_USART2_ENABLE,
  AFIO_FullRemap_USART2_ENABLE,
  AFIO_FullRemap_USART2_DISABLE,

  AFIO_Remap_LSI_CAL_ENABLE,
  AFIO_Remap_LSI_CAL_DISABLE,

  AFIO_Remap_SDI_Disable_ENABLE,
  AFIO_Remap_SDI_Disable_DISABLE

};

static inline void pinV32_DisconnectDebug(PinName pin)
{
  /** Enable this flag gives the possibility to use debug pins without any risk
    * to lose traces
    */
#ifndef CH32V_LOCK_DEBUG
  // Enable AFIO clock
  RCC_PB2PeriphClockCmd(RCC_PB2Periph_AFIO, ENABLE);

  // Disconnect JTAG-DP + SW-DP signals.
  // Warning: Need to reconnect under reset
  if ((pin == PD_1)
#if defined(GPIOB_BASE) // MxRD 250625: V002 and V004 have no port B
    || (pin == PB_3)
#endif // #if defined(GPIOB_BASE)
    ) 
  {
     GPIO_PinRemapConfig(GPIO_Remap_SDI_Disable, ENABLE);  // JTAG-DP Disabled and SW-DP Disabled
  }
#else
  (void)(pin);
#endif 
}

static inline void pin_SetV32AFPin(uint32_t afnum)
{
  // Enable AFIO clock
  RCC_PB2PeriphClockCmd(RCC_PB2Periph_AFIO, ENABLE);

// MxRD 261109 - GPIO_PinRemapConfig() calls combined for SPI/I2C/UART/TIM to reduce flash usage.
// Note: This optimization assumes consecutive remap register values and specific order of the AFIO_... enum, 
// with consecutive values groups ending with AFIO_FullRemap_xxx_DISABLE after the last enable value.
#if defined(SPI_MODULE_ENABLED) || defined(I2C_MODULE_ENABLED) || defined(UART_MODULE_ENABLED) || defined(TIM_MODULE_ENABLED)
  struct RemapGroup {
    uint32_t gpio_first;
    uint8_t first;
    uint8_t last;   // Last enable value; disable follows it.
    uint8_t shift;  // GPIO encoding step is 1 << shift.
  };

  static const struct RemapGroup groups[] = {
  #if defined(SPI_MODULE_ENABLED)
    { GPIO_PartialRemap1_SPI1, AFIO_PartialRemap1_SPI1_ENABLE, AFIO_FullRemap_SPI1_ENABLE, 0 },
  #endif
  #if defined(I2C_MODULE_ENABLED)
    { GPIO_PartialRemap1_I2C1, AFIO_PartialRemap1_I2C1_ENABLE, AFIO_FullRemap_I2C1_ENABLE, 3 },
  #endif
  #if defined(UART_MODULE_ENABLED)
    { GPIO_PartialRemap1_USART1, AFIO_PartialRemap1_USART1_ENABLE, AFIO_FullRemap_USART1_ENABLE, 6 },
    #if defined(USART2)   // MxRD 250710: V002 and V004 have no USART2
    { GPIO_PartialRemap1_USART2, AFIO_PartialRemap1_USART2_ENABLE, AFIO_FullRemap_USART2_ENABLE, 4 },
    #endif
  #endif
  #if defined(TIM_MODULE_ENABLED)
    { GPIO_PartialRemap1_TIM1, AFIO_PartialRemap1_TIM1_ENABLE, AFIO_FullRemap_TIM1_ENABLE, 10 },
    { GPIO_PartialRemap1_TIM2, AFIO_PartialRemap1_TIM2_ENABLE, AFIO_FullRemap_TIM2_ENABLE, 0 },
  #endif
  };

  for (unsigned int i = 0; i < sizeof(groups) / sizeof(groups[0]); ++i) {
    const struct RemapGroup *g = &groups[i];

    if (afnum >= g->first && afnum <= (uint32_t)g->last + 1U) {
      const int disabled = (afnum == (uint32_t)g->last + 1U);
      const uint32_t offset = disabled
          ? (uint32_t)g->last - g->first
          : afnum - g->first;
      const uint32_t remap = g->gpio_first + (offset << g->shift);

      GPIO_PinRemapConfig(remap, disabled ? DISABLE : ENABLE);
      return;
    }
  }
#endif // SPI | I2C | UART | TIM _MODULE_ENABLED 

  // ADC and the others below have separate cases
  switch (afnum) {
    case AFIO_Remap_PA1_2_ENABLE:
      GPIO_PinRemapConfig(GPIO_Remap_PA1_2,ENABLE);
      break;
    case AFIO_Remap_PA1_2_DISABLE:
      GPIO_PinRemapConfig(GPIO_Remap_PA1_2,DISABLE);
      break;

  #if defined(ADC_MODULE_ENABLED)
    case AFIO_Remap_ADC1_ETRGINJ_ENBALE:
      GPIO_PinRemapConfig(GPIO_Remap_ADC1_ETRGINJ,ENABLE);
      break;
    case AFIO_Remap_ADC1_ETRGINJ_DISABLE:
      GPIO_PinRemapConfig(GPIO_Remap_ADC1_ETRGINJ,DISABLE);
      break;
    case AFIO_Remap_ADC1_ETRGREG_ENABLE:
      GPIO_PinRemapConfig(GPIO_Remap_ADC1_ETRGREG,ENABLE);
      break;
    case AFIO_Remap_ADC1_ETRGREG_DISABLE:
      GPIO_PinRemapConfig(GPIO_Remap_ADC1_ETRGREG,DISABLE);
      break;
  #endif

    case AFIO_Remap_LSI_CAL_ENABLE:
      GPIO_PinRemapConfig(GPIO_Remap_LSI_CAL,ENABLE);
      break;
    case AFIO_Remap_LSI_CAL_DISABLE:
      GPIO_PinRemapConfig(GPIO_Remap_LSI_CAL,DISABLE);
      break;
    case AFIO_Remap_SDI_Disable_ENABLE:
      GPIO_PinRemapConfig(GPIO_Remap_SDI_Disable,ENABLE);
      break;
    case AFIO_Remap_SDI_Disable_DISABLE:
      GPIO_PinRemapConfig(GPIO_Remap_SDI_Disable,DISABLE);
      break;
      
    default:
    case AFIO_NONE:
      break;
  }
}

#ifdef __cplusplus
}
#endif

#endif /* _PINAF_CH32V00X_H */
