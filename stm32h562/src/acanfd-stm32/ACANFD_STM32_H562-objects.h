#pragma once

//------------------------------------------------------------------------------

#include "ACANFD_STM32_from_cpp.h"

//------------------------------------------------------------------------------
//  STM32H562
//    FDCAN1_RX : PA_11, PB_8,  PD_0,  PE_0
//    FDCAN1_TX : PA_12, PB_9,  PD_1,  PE_1
//------------------------------------------------------------------------------
//   FDCAN1
//------------------------------------------------------------------------------

const std::initializer_list <ACANFD_STM32::PinPort> fdcan1_tx_pin_array {
  ACANFD_STM32::PinPort (PA_12, 9), // Tx Pin: PA_12, AF9
  ACANFD_STM32::PinPort (PB_9,  9), // Tx Pin: PB_9, AF9
  ACANFD_STM32::PinPort (PD_1,  9), // Tx Pin: PD_1, AF9
  ACANFD_STM32::PinPort (PE_1,  9), // Tx Pin: PE_1, AF9
} ;

//------------------------------------------------------------------------------

const std::initializer_list <ACANFD_STM32::PinPort> fdcan1_rx_pin_array {
  ACANFD_STM32::PinPort (PA_11, 9), // Rx Pin: PA_11, AF9
  ACANFD_STM32::PinPort (PB_8,  9), // Rx Pin: PB_8, AF9
  ACANFD_STM32::PinPort (PD_0,  9), // Rx Pin: PD_0, AF9
  ACANFD_STM32::PinPort (PE_0,  9), // Rx Pin: PE_0, AF9
} ;

//------------------------------------------------------------------------------

ACANFD_STM32 fdcan1 (
  FDCAN1, // CAN Peripheral base address
  SRAMCAN_BASE,
  ACANFD_STM32::IRQs (FDCAN1_IT0_IRQn, FDCAN1_IT1_IRQn), // Interrupts
  fdcan1_tx_pin_array,
  fdcan1_rx_pin_array
) ;

//------------------------------------------------------------------------------

extern "C" void FDCAN1_IT0_IRQHandler (void) ;
extern "C" void FDCAN1_IT1_IRQHandler (void) ;

//------------------------------------------------------------------------------

void FDCAN1_IT0_IRQHandler (void) {
  fdcan1.isr0 () ;
}

//------------------------------------------------------------------------------

void FDCAN1_IT1_IRQHandler (void) {
  fdcan1.isr1 () ;
}
//------------------------------------------------------------------------------
