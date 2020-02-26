#include "Arduino.h"
#include "stm32yyxx_ll.h"

unsigned long previousMillis = 0;     // stores last time output was updated
const long interval = 1000;           // transmission interval (milliseconds)
uint32_t transfer_id;                 // hardware model id (CAN ID to send)

void setup() {
  // initialize Serial
  printbegin(115200);
  printstr("Initializing...");
  printint(HAL_RCC_GetHCLKFreq());
  println();

  // initialize digital pins
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);

  //uint32_t ret = DWT_Delay_Init();

  // initialize Hardware
  transfer_id = CAN_HW_Init(0);

  // set CAN bit rate
  initCanard(1*1000*1000);
}

void loop() {
  uint32_t can_id; // Extended format CAN ID (29bits MAX=0x1FFFFFFF)
  uint8_t payload[8];
  uint8_t payload_len;

  // Send CAN Packet from queue
  sendCanard();

  // Receive CAN Packet 
  if (receiveCanard(&can_id, payload, &payload_len)) {
    printframe(can_id, payload_len, payload);
  }

  // Push CAN Packet to queue
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
    //can_id = 0x12345678;
    for(int i=0;i<8;i++){
      payload[i] = i;
    }
    queueCanard(transfer_id, payload, 8);
  }
}
