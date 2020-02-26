#include "stm32_def.h"

void initCanard(uint32_t speed);
void sendCanard(void);
bool receiveCanard(uint32_t * can_id, uint8_t * payload, uint8_t * payload_size);
void queueCanard(uint32_t can_id, uint8_t * payload, uint8_t payload_size);
