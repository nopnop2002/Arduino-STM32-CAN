#pragma once

//------------------------------------------------------------------------------
//   Settings for NUCLEO-G0B1RE
//------------------------------------------------------------------------------

#define HAS_PROGRAMMABLE_FDCAN_RAM_SECTIONS (false)

//------------------------------------------------------------------------------

#include <stm32g0xx_ll_rcc.h>

//------------------------------------------------------------------------------

inline uint32_t fdcanClock (void) {
  if (!__HAL_RCC_FDCAN_IS_CLK_ENABLED ()) { // Is not enabled ?
  //--- Enable CAN clock
    __HAL_RCC_FDCAN_CLK_ENABLE () ;
  //--- Reset CAN peripherals
    __HAL_RCC_FDCAN_FORCE_RESET () ;
    __HAL_RCC_FDCAN_RELEASE_RESET () ;
  //--- Select CAN clock
    LL_RCC_SetFDCANClockSource (RCC_FDCANCLKSOURCE_PCLK1) ;
  }
  return HAL_RCC_GetPCLK1Freq () ;
}

//------------------------------------------------------------------------------
