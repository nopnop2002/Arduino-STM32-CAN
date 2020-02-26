#include "src/libcanard/canard.h"
#include "src/libcanard/drivers/stm32/canard_stm32.h"
#include "uavcan.h"

static CanardInstance g_canard;                //The library instance
static uint8_t g_canard_memory_pool[1024];     //Arena for memory allocation, used by the library

//////////////////////////////////////////////////////////////////////////////////////

void initCanard(uint32_t speed)
{

  CanardSTM32CANTimings timings;
  //int result = canardSTM32ComputeCANTimings(HAL_RCC_GetPCLK1Freq(), 1000000, &timings);
  int result = canardSTM32ComputeCANTimings(HAL_RCC_GetPCLK1Freq(), speed, &timings);
  if (result) {
    __ASM volatile("BKPT #01");
  }
  result = canardSTM32Init(&timings, CanardSTM32IfaceModeNormal);
  if (result) {
    __ASM volatile("BKPT #01");
  }

  canardInit(&g_canard,                         // Uninitialized library instance
             g_canard_memory_pool,              // Raw memory chunk used for dynamic allocation
             sizeof(g_canard_memory_pool),      // Size of the above, in bytes
             NULL,                              // Callback, Not used
             NULL,                              // Callback, Not used
             NULL);

  canardSetLocalNodeID(&g_canard, 10);

}


void sendCanard(void)
{

  const CanardCANFrame* txf = canardPeekTxQueue(&g_canard); 

  while(txf) {
    const int tx_res = canardSTM32Transmit(txf);
    if (tx_res < 0) {                 // Failure - drop the frame and report
      __ASM volatile("BKPT #01");     // TODO: handle the error properly
    }
    if(tx_res > 0) {
      canardPopTxQueue(&g_canard);
    }
    txf = canardPeekTxQueue(&g_canard); 
  }

}


bool receiveCanard(uint32_t * can_id, uint8_t * payload, uint8_t * payload_size)
{

  CanardCANFrame rx_frame;
  int res = canardSTM32Receive(&rx_frame);
  if(res) {
    *can_id = rx_frame.id;
    *payload_size = rx_frame.data_len;
    memcpy(payload, rx_frame.data, rx_frame.data_len);
    //printframe(rx_frame.id, rx_frame.data_len, rx_frame.data);
    //canardHandleRxFrame(&g_canard, &rx_frame, HAL_GetTick() * 1000);
    return true;
  }    
  return false;
}

void queueCanard(uint32_t can_id, uint8_t * payload, uint8_t payload_size)
{  
  canardBroadcastSimple(&g_canard, 
                  can_id,
                  payload, 
                  payload_size);
    
}
