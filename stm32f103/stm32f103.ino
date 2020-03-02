enum BITRATE{CAN_50KBPS, CAN_100KBPS, CAN_125KBPS, CAN_250KBPS, CAN_500KBPS, CAN_1000KBPS};

#define DEBUG 0

typedef struct
{
  uint32_t id;
  uint8_t  data[8];
  uint8_t  len;
} CAN_msg_t;

typedef const struct
{
  uint8_t TS2;
  uint8_t TS1;
  uint8_t BRP;
} CAN_bit_timing_config_t;

CAN_bit_timing_config_t can_configs[6] = {{2, 13, 45}, {2, 15, 20}, {2, 13, 18}, {2, 13, 9}, {2, 15, 4}, {2, 15, 2}};

void printRegister(char * buf, uint32_t reg) {
  if (DEBUG == 0) return;
  Serial.print(buf);
  Serial.print(reg, HEX);
  Serial.println();
}

/**
 * Initializes the CAN controller with specified bit rate.
 *
 * @params: bitrate - Specified bitrate. If this value is not one of the defined constants, bit rate will be defaulted to 125KBS
 * @params: remap   - Select CAN port. 
 *                    =0:CAN_RX mapped to PA11, CAN_TX mapped to PA12
 *                    =1:Not used
 *                    =2:CAN_RX mapped to PB8, CAN_TX mapped to PB9 (not available on 36-pin package)
 *                    =3:CAN_RX mapped to PD0, CAN_TX mapped to PD1 (available on 100-pin and 144-pin package)
 *
 */
bool CANInit(enum BITRATE bitrate, int remap)
{
  // Reference manual
  // https://www.st.com/content/ccc/resource/technical/document/reference_manual/59/b9/ba/7f/11/af/43/d5/CD00171190.pdf/files/CD00171190.pdf/jcr:content/translations/en.CD00171190.pdf

  RCC->APB1ENR |= 0x2000000UL;       // Enable CAN clock 
  RCC->APB2ENR |= 0x1UL;             // Enable AFIO clock
  AFIO->MAPR   &= 0xFFFF9FFF;        // reset CAN remap
                                     // CAN_RX mapped to PA11, CAN_TX mapped to PA12

  if (remap == 0) {
    RCC->APB2ENR |= 0x4UL;           // Enable GPIOA clock
    GPIOA->CRH   &= ~(0xFF000UL);    // Configure PA12(0b0000) and PA11(0b0000)
                                     // 0b0000
                                     //   MODE=00(Input mode)
                                     //   CNF=00(Analog mode)

    GPIOA->CRH   |= 0xB8FFFUL;       // Configure PA12(0b1011) and PA11(0b1000)
                                     // 0b1011
                                     //   MODE=11(Output mode, max speed 50 MHz) 
                                     //   CNF=10(Alternate function output Push-pull
                                     // 0b1000
                                     //   MODE=00(Input mode)
                                     //   CNF=10(Input with pull-up / pull-down)
                                     
    GPIOA->ODR |= 0x1UL << 12;       // PA12 Upll-up
    
  }
                                
  if (remap == 2) {
    AFIO->MAPR   |= 0x00004000;      // set CAN remap
                                     // CAN_RX mapped to PB8, CAN_TX mapped to PB9 (not available on 36-pin package)

    RCC->APB2ENR |= 0x8UL;           // Enable GPIOB clock
    GPIOB->CRH   &= ~(0xFFUL);       // Configure PB9(0b0000) and PB8(0b0000)
                                     // 0b0000
                                     //   MODE=00(Input mode)
                                     //   CNF=00(Analog mode)

    GPIOB->CRH   |= 0xB8UL;          // Configure PB9(0b1011) and PB8(0b1000)
                                     // 0b1011
                                     //   MODE=11(Output mode, max speed 50 MHz) 
                                     //   CNF=10(Alternate function output Push-pull
                                     // 0b1000
                                     //   MODE=00(Input mode)
                                     //   CNF=10(Input with pull-up / pull-down)
                                     
    GPIOB->ODR |= 0x1UL << 8;        // PB8 Upll-up
  }
  
  if (remap == 3) {
    AFIO->MAPR   |= 0x00005000;      // set CAN remap
                                     // CAN_RX mapped to PD0, CAN_TX mapped to PD1 (available on 100-pin and 144-pin package)

    RCC->APB2ENR |= 0x20UL;          // Enable GPIOD clock
    GPIOD->CRL   &= ~(0xFFUL);       // Configure PD1(0b0000) and PD0(0b0000)
                                     // 0b0000
                                     //   MODE=00(Input mode)
                                     //   CNF=00(Analog mode)

    GPIOD->CRH   |= 0xB8UL;          // Configure PD1(0b1011) and PD0(0b1000)
                                     // 0b1000
                                     //   MODE=00(Input mode)
                                     //   CNF=10(Input with pull-up / pull-down)
                                     // 0b1011
                                     //   MODE=11(Output mode, max speed 50 MHz) 
                                     //   CNF=10(Alternate function output Push-pull
                                     
    GPIOD->ODR |= 0x1UL << 0;        // PD0 Upll-up
  }

  CAN1->MCR |= 0x1UL;                   // Require CAN1 to Initialization mode 
  while (!(CAN1->MSR & 0x1UL));         // Wait for Initialization mode

  //CAN1->MCR = 0x51UL;                 // Hardware initialization(No automatic retransmission)
  CAN1->MCR = 0x41UL;                   // Hardware initialization(With automatic retransmission)
   
  // Set bit rates 
  CAN1->BTR &= ~(((0x03) << 24) | ((0x07) << 20) | ((0x0F) << 16) | (0x1FF)); 
  CAN1->BTR |=  (((can_configs[bitrate].TS2-1) & 0x07) << 20) | (((can_configs[bitrate].TS1-1) & 0x0F) << 16) | ((can_configs[bitrate].BRP-1) & 0x1FF);

  // Configure Filters to default values
  CAN1->FMR  |=   0x1UL;                // Set to filter initialization mode
  CAN1->FMR  &= 0xFFFFC0FF;             // Clear CAN2 start bank

  // bxCAN has 28 filters.
  // These filters are used for both CAN1 and CAN2.
  // STM32F103 has only CAN1, so all 28 are used for CAN1
  CAN1->FMR  |= 0x1C << 8;              // Assign all filters to CAN1

  CAN1->FA1R &= ~(0x1UL);               // Deactivate filter 0
  CAN1->FS1R |=   0x1UL;                // Set first filter to single 32 bit configuration

  CAN1->sFilterRegister[0].FR1 = 0x0UL; // Set filter registers to 0
  CAN1->sFilterRegister[0].FR2 = 0x0UL; // Set filter registers to 0
  CAN1->FM1R &= ~(0x1UL);               // Set filter to mask mode

  CAN1->FFA1R &= ~(0x1UL);              // Apply filter to FIFO 0  
  CAN1->FA1R  |=   0x1UL;               // Activate filter 0
  
  CAN1->FMR   &= ~(0x1UL);              // Deactivate initialization mode

  uint16_t TimeoutMilliseconds = 1000;
  bool can1 = false;
  CAN1->MCR   &= ~(0x1UL);              // Require CAN1 to normal mode 

  // Wait for normal mode
  // If the connection is not correct, it will not return to normal mode.
  for (uint16_t wait_ack = 0; wait_ack < TimeoutMilliseconds; wait_ack++) {
    if ((CAN1->MSR & 0x1UL) == 0) {
      can1 = true;
      break;
    }
    delayMicroseconds(1000);
  }
  //Serial.print("can1=");
  //Serial.println(can1);
  if (can1) {
    Serial.println("CAN1 initialize ok");
  } else {
    Serial.println("CAN1 initialize fail!!");
    return false;
  }
  return true; 
}

#if 0 
void CANSetFilter(uint16_t id)
{
     static uint32_t filterID = 0;
     
     if (filterID == 112)
     {
         return;
     }
     
     CAN1->FMR  |=   0x1UL;                // Set to filter initialization mode
     
     switch(filterID%4)
     {
         case 0:
            // if we need another filter bank, initialize it
            CAN1->FA1R |= 0x1UL <<(filterID/4);
            CAN1->FM1R |= 0x1UL << (filterID/4);
            CAN1->FS1R &= ~(0x1UL << (filterID/4)); 
            
            CAN1->sFilterRegister[filterID/4].FR1 = (id << 5) | (id << 21);
            CAN1->sFilterRegister[filterID/4].FR2 = (id << 5) | (id << 21);
            break;
         case 1:
            CAN1->sFilterRegister[filterID/4].FR1 &= 0x0000FFFF;
            CAN1->sFilterRegister[filterID/4].FR1 |= id << 21;
            break;
         case 2:
            CAN1->sFilterRegister[filterID/4].FR2 = (id << 5) | (id << 21);
            break;
         case 3:
              CAN1->sFilterRegister[filterID/4].FR2 &= 0x0000FFFF;
              CAN1->sFilterRegister[filterID/4].FR2 |= id << 21;
              break;
     }
     filterID++;
     CAN1->FMR   &= ~(0x1UL);              // Deactivate initialization mode
}
 
void CANSetFilters(uint16_t* ids, uint8_t num)
{
    for (int i = 0; i < num; i++)
    {
        CANSetFilter(ids[i]);
    }
}
#endif

#define STM32_CAN_TIR_TXRQ              (1U << 0U)  // Bit 0: Transmit Mailbox Request
#define STM32_CAN_RIR_RTR               (1U << 1U)  // Bit 1: Remote Transmission Request
#define STM32_CAN_RIR_IDE               (1U << 2U)  // Bit 2: Identifier Extension
#define STM32_CAN_TIR_RTR               (1U << 1U)  // Bit 1: Remote Transmission Request
#define STM32_CAN_TIR_IDE               (1U << 2U)  // Bit 2: Identifier Extension

#define CAN_EXT_ID_MASK                 0x1FFFFFFFU
#define CAN_STD_ID_MASK                 0x000007FFU
#define CAN_FRAME_RTR                   (1UL << 30U) // Remote transmission
 
/**
 * Decodes CAN messages from the data registers and populates a 
 * CAN message struct with the data fields.
 * 
 * @preconditions A valid CAN message is received
 * @params CAN_rx_msg - CAN message structure for reception
 * 
 */
void CANReceive(CAN_msg_t* CAN_rx_msg)
{
    uint32_t id = CAN1->sFIFOMailBox[0].RIR;
    if ((id & STM32_CAN_RIR_IDE) == 0) { // Standard frame format
        CAN_rx_msg->id = (CAN_STD_ID_MASK & (id >> 21U));
    } 
    else {                               // Extended frame format
        CAN_rx_msg->id = (CAN_EXT_ID_MASK & (id >> 3U));
    }

    if ((id & STM32_CAN_RIR_RTR) != 0) {
        CAN_rx_msg->id |= CAN_FRAME_RTR;
    }

    
    CAN_rx_msg->len = (CAN1->sFIFOMailBox[0].RDTR) & 0xFUL;
    
    CAN_rx_msg->data[0] = 0xFFUL &  CAN1->sFIFOMailBox[0].RDLR;
    CAN_rx_msg->data[1] = 0xFFUL & (CAN1->sFIFOMailBox[0].RDLR >> 8);
    CAN_rx_msg->data[2] = 0xFFUL & (CAN1->sFIFOMailBox[0].RDLR >> 16);
    CAN_rx_msg->data[3] = 0xFFUL & (CAN1->sFIFOMailBox[0].RDLR >> 24);
    CAN_rx_msg->data[4] = 0xFFUL &  CAN1->sFIFOMailBox[0].RDHR;
    CAN_rx_msg->data[5] = 0xFFUL & (CAN1->sFIFOMailBox[0].RDHR >> 8);
    CAN_rx_msg->data[6] = 0xFFUL & (CAN1->sFIFOMailBox[0].RDHR >> 16);
    CAN_rx_msg->data[7] = 0xFFUL & (CAN1->sFIFOMailBox[0].RDHR >> 24);
    
    CAN1->RF0R |= 0x20UL;
}
 
/**
 * Encodes CAN messages using the CAN message struct and populates the 
 * data registers with the sent.
 * 
 * @params CAN_tx_msg - CAN message structure for transmission
 * 
 */
void CANSend(CAN_msg_t* CAN_tx_msg)
{
    volatile int count = 0;

    uint32_t out = 0;
    if (CAN_tx_msg->id > CAN_STD_ID_MASK) { // Extended frame format
        out = ((CAN_tx_msg->id & CAN_EXT_ID_MASK) << 3U) | STM32_CAN_TIR_IDE;
    }
    else {                                  // Standard frame format
        out = ((CAN_tx_msg->id & CAN_STD_ID_MASK) << 21U);
    }

    if (CAN_tx_msg->id & CAN_FRAME_RTR) {
        out |= STM32_CAN_TIR_RTR;
    }

    CAN1->sTxMailBox[0].TDTR &= ~(0xF);
    CAN1->sTxMailBox[0].TDTR |= CAN_tx_msg->len & 0xFUL;
    
    CAN1->sTxMailBox[0].TDLR  = (((uint32_t) CAN_tx_msg->data[3] << 24) |
                                 ((uint32_t) CAN_tx_msg->data[2] << 16) |
                                 ((uint32_t) CAN_tx_msg->data[1] <<  8) |
                                 ((uint32_t) CAN_tx_msg->data[0]      ));
    CAN1->sTxMailBox[0].TDHR  = (((uint32_t) CAN_tx_msg->data[7] << 24) |
                                 ((uint32_t) CAN_tx_msg->data[6] << 16) |
                                 ((uint32_t) CAN_tx_msg->data[5] <<  8) |
                                 ((uint32_t) CAN_tx_msg->data[4]      ));

    // Send Go
    CAN1->sTxMailBox[0].TIR = out | STM32_CAN_TIR_TXRQ;

    while(CAN1->sTxMailBox[0].TIR & 0x1UL && count++ < 1000000);
     
     if (!(CAN1->sTxMailBox[0].TIR & 0x1UL)) return;
     
     //Sends error log to screen
     while (CAN1->sTxMailBox[0].TIR & 0x1UL)
     {
         Serial.println("Send Fail");
         Serial.println(CAN1->ESR);        
         Serial.println(CAN1->MSR);        
         Serial.println(CAN1->TSR);
        
     }
}

 /**
 * Returns whether there are CAN messages available.
 *
 * @returns If pending CAN messages are in the CAN controller
 *
 */
uint8_t CANMsgAvail(void)
{
  return CAN1->RF0R & 0x3UL;
}


uint8_t counter = 0;
unsigned long previousMillis = 0;     // stores last time output was updated
const long interval = 1000;           // transmission interval (milliseconds)

void setup() {
  Serial.begin(115200);
  bool ret = CANInit(CAN_1000KBPS, 0);  // CAN_RX mapped to PA11, CAN_TX mapped to PA12
  //bool ret = CANInit(CAN_1000KBPS, 2);  // CAN_RX mapped to PB8, CAN_TX mapped to PB9
  //bool ret = CANInit(CAN_1000KBPS, 3);  // CAN_RX mapped to PD0, CAN_TX mapped to PD1
  if (!ret) while(true);
}

void loop() {
  CAN_msg_t CAN_TX_msg;
  CAN_msg_t CAN_RX_msg;
   
  CAN_TX_msg.data[0] = counter;
  CAN_TX_msg.data[1] = 0x01;
  CAN_TX_msg.data[2] = 0x02;
  CAN_TX_msg.data[3] = 0x03;
  CAN_TX_msg.data[4] = 0x04;
  CAN_TX_msg.data[5] = 0x05;
  CAN_TX_msg.data[6] = 0x06;
  CAN_TX_msg.data[7] = 0x07;
  CAN_TX_msg.len = 8;

  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    if ( ( counter % 2) == 0) {
        CAN_TX_msg.id = 0x1032F103;
    } else {
        CAN_TX_msg.id = 0x103;
    }
    CANSend(&CAN_TX_msg);
    counter++;
  }
  
  if(CANMsgAvail()) {
    CANReceive(&CAN_RX_msg);

    if (CAN_RX_msg.id > 0x7ff) {
      Serial.print("Extended ID: 0x");
      if (CAN_RX_msg.id < 0x10000000) Serial.print("0");
      if (CAN_RX_msg.id < 0x1000000) Serial.print("00");
      if (CAN_RX_msg.id < 0x100000) Serial.print("000");
      if (CAN_RX_msg.id < 0x10000) Serial.print("0000");
      Serial.print(CAN_RX_msg.id, HEX);
    } else {
      Serial.print("Standard ID: 0x");
      if (CAN_RX_msg.id < 0x100) Serial.print("0");
      if (CAN_RX_msg.id < 0x10) Serial.print("00");
      Serial.print(CAN_RX_msg.id, HEX);
      Serial.print("     ");
    }

    Serial.print(" DLC: ");
    Serial.print(CAN_RX_msg.len);
    Serial.print(" Data: ");
    for(int i=0; i<CAN_RX_msg.len; i++) {
      Serial.print("0x"); 
      Serial.print(CAN_RX_msg.data[i], HEX); 
      if (i != (CAN_RX_msg.len-1))  Serial.print(" ");
    }
    Serial.println();
  }
    
  

}
