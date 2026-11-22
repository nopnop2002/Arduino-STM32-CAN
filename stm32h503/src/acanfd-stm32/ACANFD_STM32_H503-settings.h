#pragma once

//------------------------------------------------------------------------------
//   Settings for STM32H503
//------------------------------------------------------------------------------

#define HAS_PROGRAMMABLE_FDCAN_RAM_SECTIONS (false)

//------------------------------------------------------------------------------

#include <stm32h5xx_ll_rcc.h>

//------------------------------------------------------------------------------

inline uint32_t fdcanClock (void) {
  if (!__HAL_RCC_FDCAN_IS_CLK_ENABLED ()) { // Is not enabled ?
    //--- Configure the FDCAN clock source
    RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};
    PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_FDCAN;
    PeriphClkInit.PLL2.PLL2Source = RCC_PLL2_SOURCE_CSI;
    PeriphClkInit.PLL2.PLL2M = 1;
    PeriphClkInit.PLL2.PLL2N = 48;
    PeriphClkInit.PLL2.PLL2P = 2;
    PeriphClkInit.PLL2.PLL2Q = 1;
    PeriphClkInit.PLL2.PLL2R = 2;
    PeriphClkInit.PLL2.PLL2RGE = RCC_PLL2_VCIRANGE_2;
    PeriphClkInit.PLL2.PLL2VCOSEL = RCC_PLL2_VCORANGE_WIDE;
    PeriphClkInit.PLL2.PLL2FRACN = 0;
    PeriphClkInit.PLL2.PLL2ClockOut = RCC_PLL2_DIVQ;
    PeriphClkInit.FdcanClockSelection = RCC_FDCANCLKSOURCE_PLL2Q;
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK) {
      Serial.println("HAL_RCCEx_PeriphCLKConfig fail");
      while(true);
    }
    //--- Enable CAN clock
    __HAL_RCC_FDCAN_CLK_ENABLE () ;
    //--- Reset CAN peripherals
    __HAL_RCC_FDCAN_FORCE_RESET () ;
    __HAL_RCC_FDCAN_RELEASE_RESET () ;
    //--- Select CAN clock
    //LL_RCC_SetFDCANClockSource (RCC_FDCANCLKSOURCE_PLL2Q) ;
  }
  //return HAL_RCC_GetPCLK1Freq () ;
  return HAL_RCCEx_GetPeriphCLKFreq(RCC_PERIPHCLK_FDCAN) ;
}
//------------------------------------------------------------------------------
