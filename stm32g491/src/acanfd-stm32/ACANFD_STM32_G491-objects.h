#pragma once

//------------------------------------------------------------------------------

#include "ACANFD_STM32_from_cpp.h"

//------------------------------------------------------------------------------
//  STM32G491
//    FDCAN1_RX : PA_11, PB_8,  PD_0
//    FDCAN1_TX : PA_12, PB_9,  PD_1
//    FDCAN2_RX : PB_5,  PB_12
//    FDCAN2_TX : PB_6,  PB_13
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
//   FDCAN2
//------------------------------------------------------------------------------

const std::initializer_list <ACANFD_STM32::PinPort> fdcan2_tx_pin_array {
  ACANFD_STM32::PinPort (PB_6,  9), // Tx Pin: PB_6, AF9
  ACANFD_STM32::PinPort (PB_13, 9)  // Tx Pin: PB_13, AF9
} ;

//------------------------------------------------------------------------------

const std::initializer_list <ACANFD_STM32::PinPort> fdcan2_rx_pin_array {
  ACANFD_STM32::PinPort (PB_5,  9), // Rx Pin: PB_5, AF9
  ACANFD_STM32::PinPort (PB_12, 9)  // Rx Pin: PB_12, AF9
} ;

//------------------------------------------------------------------------------

ACANFD_STM32 fdcan2 (
  FDCAN2, // CAN Peripheral base address
  SRAMCAN_BASE + 212 * 4,
  ACANFD_STM32::IRQs (FDCAN2_IT0_IRQn, FDCAN2_IT1_IRQn), // Interrupts
  fdcan2_tx_pin_array,
  fdcan2_rx_pin_array
) ;

//------------------------------------------------------------------------------

extern "C" void FDCAN2_IT0_IRQHandler (void) ;
extern "C" void FDCAN2_IT1_IRQHandler (void) ;

//------------------------------------------------------------------------------

void FDCAN2_IT0_IRQHandler (void) {
  fdcan2.isr0 () ;
}

//------------------------------------------------------------------------------

void FDCAN2_IT1_IRQHandler (void) {
  fdcan2.isr1 () ;
}

//------------------------------------------------------------------------------
