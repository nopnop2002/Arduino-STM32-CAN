#pragma once

//------------------------------------------------------------------------------
//   Settings for STM32H750
//------------------------------------------------------------------------------

#define HAS_PROGRAMMABLE_FDCAN_RAM_SECTIONS (true)
static const uint32_t FDCAN_MESSAGE_RAM_WORD_SIZE = 2560 ;

//------------------------------------------------------------------------------

#include <stm32h7xx_ll_rcc.h>

//------------------------------------------------------------------------------

inline uint32_t fdcanClock (void) {
  if (!__HAL_RCC_FDCAN_IS_CLK_ENABLED ()) {
    //--- Enable CAN clock
    __HAL_RCC_FDCAN_CLK_ENABLE () ;
    //--- Reset CAN peripherals
    __HAL_RCC_FDCAN_FORCE_RESET () ;
    __HAL_RCC_FDCAN_RELEASE_RESET () ;
    //--- Select CAN clock
    LL_RCC_SetFDCANClockSource (LL_RCC_FDCAN_CLKSOURCE_PLL1Q) ;
  }
  //return HAL_RCC_GetPCLK1Freq () ;
  return HAL_RCCEx_GetPeriphCLKFreq(RCC_PERIPHCLK_FDCAN) ;
}

//------------------------------------------------------------------------------
