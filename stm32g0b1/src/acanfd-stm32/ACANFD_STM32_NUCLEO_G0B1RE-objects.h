#pragma once

//------------------------------------------------------------------------------

#include "ACANFD_STM32_from_cpp.h"

//------------------------------------------------------------------------------
//  NUCLEO-G0B1RE: STM32G0B1RE (DS13560 Rev 4, december 2022)
//    FDCAN1_RX : PA11, PB8, PC4, PD0
//    FDCAN1_TX : PA12, PB9, PC5, PD1
//    FDCAN2_RX : PB0, PB5, PB12, PC2; not available on Nucleo-64 board: PD14
//    FDCAN2_TX : PB1, PB6, PB13, PC3; not available on Nucleo-64 board: PD15
//------------------------------------------------------------------------------
//   FDCAN1
//------------------------------------------------------------------------------

const std::initializer_list <ACANFD_STM32::PinPort> fdcan1_tx_pin_array {
  ACANFD_STM32::PinPort (PA_12, 3),
  ACANFD_STM32::PinPort (PB_9,  3),
  ACANFD_STM32::PinPort (PC_5,  3),
  ACANFD_STM32::PinPort (PD_1,  3),
  ACANFD_STM32::PinPort (PD_13, 3)
} ;

//------------------------------------------------------------------------------

const std::initializer_list <ACANFD_STM32::PinPort> fdcan1_rx_pin_array {
  ACANFD_STM32::PinPort (PA_11, 3),
  ACANFD_STM32::PinPort (PB_8,  3),
  ACANFD_STM32::PinPort (PC_4,  3),
  ACANFD_STM32::PinPort (PD_0,  3),
  ACANFD_STM32::PinPort (PD_12, 3)
} ;

//------------------------------------------------------------------------------

ACANFD_STM32 fdcan1 (
  FDCAN1, // CAN Peripheral base address
  SRAMCAN_BASE,
  std::nullopt, // No interrupt
  fdcan1_tx_pin_array,
  fdcan1_rx_pin_array
) ;

//------------------------------------------------------------------------------
//   FDCAN2
//------------------------------------------------------------------------------

const std::initializer_list <ACANFD_STM32::PinPort> fdcan2_tx_pin_array {
  ACANFD_STM32::PinPort (PB_1,  3),
  ACANFD_STM32::PinPort (PB_6,  3),
  ACANFD_STM32::PinPort (PB_13, 3),
  ACANFD_STM32::PinPort (PC_3,  3),
  ACANFD_STM32::PinPort (PD_15, 3)
} ;

//------------------------------------------------------------------------------

const std::initializer_list <ACANFD_STM32::PinPort> fdcan2_rx_pin_array {
  ACANFD_STM32::PinPort (PB_0,  3),
  ACANFD_STM32::PinPort (PB_5,  3),
  ACANFD_STM32::PinPort (PB_12, 3),
  ACANFD_STM32::PinPort (PC_3,  3),
  ACANFD_STM32::PinPort (PD_14, 3)
} ;

//------------------------------------------------------------------------------

ACANFD_STM32 fdcan2 (
  FDCAN2, // CAN Peripheral base address
  SRAMCAN_BASE + 212 * 4,
  std::nullopt, // No interrupt
  fdcan2_tx_pin_array,
  fdcan2_rx_pin_array
) ;

//------------------------------------------------------------------------------
