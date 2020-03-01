#include "stm32_def.h"

//void CAN_HW_Init(void) {
uint32_t CAN_HW_Init(uint8_t serial) {

  GPIO_InitTypeDef GPIO_InitStruct;

  // GPIO Ports Clock Enable
  __HAL_RCC_GPIOA_CLK_ENABLE();

  // CAN1 clock enable
  __HAL_RCC_CAN1_CLK_ENABLE();
  
  // CAN GPIO Configuration    
  // PA11     ------> CAN_RX
  // PA12     ------> CAN_TX 

#if defined (STM32F103x6) || defined (STM32F103xB)
  printstr("Board is STM32F103x6/F103xB");
  println();
  GPIO_InitStruct.Pin = GPIO_PIN_11;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = GPIO_PIN_12;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
  return 0x103 << 8 + serial;  // 0x103XX
  
#elif defined (STM32F303x8) || defined (STM32F303xC) || defined (STM32F303xE)
  printstr("Board is STM32F303x8/F303xC/F303xE");
  println();
  GPIO_InitStruct.Pin = GPIO_PIN_11 | GPIO_PIN_12;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF9_CAN;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
  return 0x303 << 8 + serial;  // 0x303XX

#elif defined (STM32F405xx) || defined (STM32F415xx) || defined (STM32F407xx) || defined (STM32F417xx)
  printstr("Board is STM32F405xx/F415xx/F407xx/417xx");
  println();
  GPIO_InitStruct.Pin = GPIO_PIN_11 | GPIO_PIN_12;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF9_CAN1;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
  return 0x405 << 8 + serial;  // 0x303XX

#else
#error "Warning untested processor variant"
#endif

}
