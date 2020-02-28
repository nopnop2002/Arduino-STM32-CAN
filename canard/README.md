# canard
I forked from [here](https://github.com/geosmall/UAVCAN-for-STM32-Arduino).

# Changes from the original
The original supports transmission of 8 bytes or more, but is not compatible with other libraries.   
This example can only transfer up to 8 bytes of data, but can communicate with other libraries.   

# API
- Initialize   
void initCanard(uint32_t speed);   

- Send CAN Packet from queue   
void sendCanard(void);   

- Receive CAN Packet   
bool receiveCanard(uint32_t * can_id, uint8_t * payload, uint8_t * payload_size);

- Push CAN Packet to TX queue   
void queueCanard(uint32_t can_id, uint8_t * payload, uint8_t payload_size);

