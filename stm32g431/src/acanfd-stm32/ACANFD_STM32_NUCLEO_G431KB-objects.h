#pragma once

//------------------------------------------------------------------------------

#include "ACANFD_STM32_from_cpp.h"

//------------------------------------------------------------------------------
//  NUCLEO-G431KB: STM32G431KB (DS12589 Rev 6, October 2021)
//    FDCAN_RX : PA11, PB8
//    FDCAN_TX : PA12, PB9
//------------------------------------------------------------------------------
//   FDCAN1
//------------------------------------------------------------------------------

const std::initializer_list <ACANFD_STM32::PinPort> fdcan1_tx_pin_array {
  ACANFD_STM32::PinPort (PA_12, 9), // Tx Pin: PA_12, AF9
  ACANFD_STM32::PinPort (PB_9,  9), // Tx Pin: PB_9, AF9
  ACANFD_STM32::PinPort (PD_1,  9), // Tx Pin: PD_1, AF9
} ;

//------------------------------------------------------------------------------

const std::initializer_list <ACANFD_STM32::PinPort> fdcan1_rx_pin_array {
  ACANFD_STM32::PinPort (PA_11, 9), // Rx Pin: PA_11, AF9
  ACANFD_STM32::PinPort (PB_8,  9), // Rx Pin: PB_9, AF9
  ACANFD_STM32::PinPort (PD_0,  9), // Rx Pin: PD_0, AF9
} ;

//------------------------------------------------------------------------------

ACANFD_STM32 fdcan1 (
  FDCAN1, // CAN Peripheral base address
  SRAMCAN_BASE, // CAN RAM Base Address
  ACANFD_STM32::IRQs (FDCAN1_IT0_IRQn, FDCAN1_IT1_IRQn), // interrupts
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
