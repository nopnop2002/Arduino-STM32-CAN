#define DEBUG 0
#define AF8   0x08
#define AF9   0x09

#define CAN_PORT CAN1 // Using CAN1
//#define CAN_PORT CAN2 // Using CAN2

/* Symbolic names for bit rate of CAN message                                */
typedef enum {CAN_50KBPS, CAN_100KBPS, CAN_125KBPS, CAN_250KBPS, CAN_500KBPS, CAN_1000KBPS} BITRATE;

/* Symbolic names for formats of CAN message                                 */
typedef enum {STANDARD_FORMAT = 0, EXTENDED_FORMAT} CAN_FORMAT;

/* Symbolic names for type of CAN message                                    */
typedef enum {DATA_FRAME = 0, REMOTE_FRAME}         CAN_FRAME;

typedef struct
{
  uint32_t id;        /* 29 bit identifier                               */
  uint8_t  data[8];   /* Data field                                      */
  uint8_t  len;       /* Length of data field in bytes                   */
  uint8_t  ch;        /* Object channel(Not use)                         */
  uint8_t  format;    /* 0 - STANDARD, 1- EXTENDED IDENTIFIER            */
  uint8_t  type;      /* 0 - DATA FRAME, 1 - REMOTE FRAME                */
} CAN_msg_t;

typedef const struct
{
  uint8_t TS2;
  uint8_t TS1;
  uint8_t BRP;
} CAN_bit_timing_config_t;

CAN_bit_timing_config_t can_configs[6] = {{2, 13, 60}, {2, 13, 30}, {2, 13, 24}, {2, 13, 12}, {2, 13, 6}, {1, 6, 6}};

/**
 * Print registers.
*/ 
void printRegister(char * buf, uint32_t reg) {
  if (DEBUG == 0) return;
  Serial.print(buf);
  Serial.print(reg, HEX);
  Serial.println();
}


/**
 * Initializes the CAN GPIO registers.
 *
 * @params: addr    - Specified GPIO register address.
 * @params: index   - Specified GPIO index.
 * @params: afry    - Specified Alternative function selection AF0-AF15.
 * @params: speed   - Specified OSPEEDR register value.(Optional)
 *
 */
void CANSetGpio(GPIO_TypeDef * addr, uint8_t index, uint8_t afry, uint8_t speed = 3) {
    uint8_t _index2 = index * 2;
    uint8_t _index4 = index * 4;
    uint8_t ofs = 0;
    uint8_t setting;

    if (index > 7) {
      _index4 = (index - 8) * 4;
      ofs = 1;
    }

    uint32_t mask;
    printRegister("GPIO_AFR(b)=", addr->AFR[1]);
    mask = 0xF << _index4;
    addr->AFR[ofs]  &= ~mask;         // Reset alternate function
    //setting = 0x9;                    // AF9
    setting = afry;                   // Alternative function selection
    mask = setting << _index4;
    addr->AFR[ofs]  |= mask;          // Set alternate function
    printRegister("GPIO_AFR(a)=", addr->AFR[1]);

    printRegister("GPIO_MODER(b)=", addr->MODER);
    mask = 0x3 << _index2;
    addr->MODER   &= ~mask;           // Reset mode
    setting = 0x2;                    // Alternate function mode
    mask = setting << _index2;
    addr->MODER   |= mask;            // Set mode
    printRegister("GPIO_MODER(a)=", addr->MODER);

    printRegister("GPIO_OSPEEDR(b)=", addr->OSPEEDR);
    mask = 0x3 << _index2;
    addr->OSPEEDR &= ~mask;           // Reset speed
    setting = speed;
    mask = setting << _index2;
    addr->OSPEEDR |= mask;            // Set speed
    printRegister("GPIO_OSPEEDR(a)=", addr->OSPEEDR);

    printRegister("GPIO_OTYPER(b)=", addr->OTYPER);
    mask = 0x1 << index;
    addr->OTYPER  &= ~mask;           // Reset Output push-pull
    printRegister("GPIO_OTYPER(a)=", addr->OTYPER);

    printRegister("GPIO_PUPDR(b)=", addr->PUPDR);
    mask = 0x3 << _index2;
    addr->PUPDR   &= ~mask;           // Reset port pull-up/pull-down
    printRegister("GPIO_PUPDR(a)=", addr->PUPDR);
}

/**
 * Initializes the CAN filter registers.
 *
 * The bxCAN provides up to 28 scalable/configurable identifier filter banks for selecting the incoming messages the software needs and discarding the others. 
 * 
 * @preconditions   - This register can be written only when the filter initialization mode is set (FINIT=1) in the CAN_FMR register.
 * @params: index   - Specified filter index. index 27:14 are available in connectivity line devices only.
 * @params: scale   - Select filter scale.
 *                    0: Dual 16-bit scale configuration
 *                    1: Single 32-bit scale configuration
 * @params: mode    - Select filter mode.
 *                    0: Two 32-bit registers of filter bank x are in Identifier Mask mode
 *                    1: Two 32-bit registers of filter bank x are in Identifier List mode
 * @params: fifo    - Select filter assigned.
 *                    0: Filter assigned to FIFO 0
 *                    1: Filter assigned to FIFO 1
 * @params: bank1   - Filter bank register 1
 * @params: bank2   - Filter bank register 2
 *
 */
void CANSetFilter(uint8_t index, uint8_t scale, uint8_t mode, uint8_t fifo, uint32_t bank1, uint32_t bank2) {
  if (index > 27) return;

  CAN1->FA1R &= ~(0x1UL<<index);               // Deactivate filter

  if (scale == 0) {
    CAN1->FS1R &= ~(0x1UL<<index);             // Set filter to Dual 16-bit scale configuration
  } else {
    CAN1->FS1R |= (0x1UL<<index);              // Set filter to single 32 bit configuration
  }
    if (mode == 0) {
    CAN1->FM1R &= ~(0x1UL<<index);             // Set filter to Mask mode
  } else {
    CAN1->FM1R |= (0x1UL<<index);              // Set filter to List mode
  }

  if (fifo == 0) {
    CAN1->FFA1R &= ~(0x1UL<<index);            // Set filter assigned to FIFO 0
  } else {
    CAN1->FFA1R |= (0x1UL<<index);             // Set filter assigned to FIFO 1
  }

  CAN1->sFilterRegister[index].FR1 = bank1;    // Set filter bank registers1
  CAN1->sFilterRegister[index].FR2 = bank2;    // Set filter bank registers2

  CAN1->FA1R |= (0x1UL<<index);                // Activate filter

}

    
/**
 * Initializes the CAN controller with specified bit rate.
 *
 * @params: bitrate - Specified bitrate. If this value is not one of the defined constants, bit rate will be defaulted to 125KBS
 * @params: remap   - Select CAN port. 
 *                    =0:CAN1_RX mapped to PA11, CAN1_TX mapped to PA12
 *                       CAN2_RX mapped to PB5 , CAN2_TX mapped to PB6
 *                    =1:CAN1_RX mapped to PG0 , CAN1_TX mapped to PG1
 *                       CAN2_RX mapped to PG11, CAN2_TX mapped to PG12
 *                    =2:CAN1_RX mapped to PB8,  CAN1_TX mapped to PB9
 *                       CAN2_RX mapped to PB12, CAN2_TX mapped to PB13
 *                    =3:CAN1_RX mapped to PD0,  CAN1_TX mapped to PD1
 *                       CAN2_RX mapped to PB12, CAN2_TX mapped to PB13
 *
 */
bool CANInit(BITRATE bitrate, int remap)
{
  // Reference manual
  // https://www.st.com/content/ccc/resource/technical/document/reference_manual/4d/ed/bc/89/b5/70/40/dc/DM00135183.pdf/files/DM00135183.pdf/jcr:content/translations/en.DM00135183.pdf
  
  RCC->APB1ENR |= 0x2000000UL;           // Enable CAN1 clock 
  RCC->APB1ENR |= 0x4000000UL;           // Enable CAN2 clock 
  
  if (remap == 0) {
    // CAN1
    RCC->AHB1ENR |= 0x1;                 // Enable GPIOA clock 
    CANSetGpio(GPIOA, 11, AF9);          // Set PA11 to AF9
    CANSetGpio(GPIOA, 12, AF9);          // Set PA12 to AF9
    
    // CAN2
    RCC->AHB1ENR |= 0x2;                 // Enable GPIOB clock 
    CANSetGpio(GPIOB, 5, AF9);           // Set PB5 to AF9
    CANSetGpio(GPIOB, 6, AF9);           // Set PB6 to AF9
  }

#if defined GPIOG
  if (remap == 1) {
    // CAN1
    RCC->AHB1ENR |= 0x40;                // Enable GPIOG clock 
    CANSetGpio(GPIOG, 0, AF9);           // Set PG0 to AF9
    CANSetGpio(GPIOG, 1, AF9);           // Set PG1 to AF9

    // CAN2
    RCC->AHB1ENR |= 0x40;                // Enable GPIOG clock 
    CANSetGpio(GPIOG, 11, AF9);          // Set PG11 to AF9
    CANSetGpio(GPIOG, 12, AF9);          // Set PG11 to AF9
  }
#endif

  if (remap == 2) {
    // CAN1
    RCC->AHB1ENR |= 0x2;                 // Enable GPIOB clock 
    CANSetGpio(GPIOB, 8, AF8);           // Set PB8 to AF8
    CANSetGpio(GPIOB, 9, AF8);           // Set PB9 to AF8

    // CAN2
    RCC->AHB1ENR |= 0x2;                 // Enable GPIOB clock 
    CANSetGpio(GPIOB, 12, AF9);          // Set PB12 to AF9
    CANSetGpio(GPIOB, 13, AF9);          // Set PB13 to AF9
  }
    
  if (remap == 3) {
    // CAN1
    RCC->AHB1ENR |= 0x8;                 // Enable GPIOD clock 
    CANSetGpio(GPIOD, 0, AF9);           // Set PD0 to AF9
    CANSetGpio(GPIOD, 1, AF9);           // Set PD1 to AF9

    // CAN2
    RCC->AHB1ENR |= 0x2;                 // Enable GPIOB clock 
    CANSetGpio(GPIOB, 12, AF9);          // Set PB12 to AF9
    CANSetGpio(GPIOB, 13, AF9);          // Set PB13 to AF9
  }

  CAN1->MCR |= 0x1UL;                    // Require CAN1 to Initialization mode 
  while (!(CAN1->MSR & 0x1UL));          // Wait for Initialization mode
  printRegister("CAN1->MCR=", CAN1->MCR);

  CAN2->MCR |= 0x1UL;                    // Require CAN2 to Initialization mode
  while (!(CAN2->MSR & 0x1UL));          // Wait for Initialization mode
  printRegister("CAN2->MCR=", CAN2->MCR);

  //CAN1->MCR = 0x51UL;                  // Hardware initialization(No automatic retransmission)
  CAN1->MCR = 0x41UL;                    // Hardware initialization(With automatic retransmission)

  //CAN2->MCR = 0x51UL;                  // Hardware initialization(No automatic retransmission)
  CAN2->MCR = 0x41UL;                    // Hardware initialization(With automatic retransmission)

  
  // Set bit rates 
  CAN1->BTR &= ~(((0x03) << 24) | ((0x07) << 20) | ((0x0F) << 16) | (0x3FF)); 
  CAN1->BTR |=  (((can_configs[bitrate].TS2-1) & 0x07) << 20) | (((can_configs[bitrate].TS1-1) & 0x0F) << 16) | ((can_configs[bitrate].BRP-1) & 0x3FF);
  printRegister("CAN1->BTR=", CAN1->BTR);

  CAN2->BTR &= ~(((0x03) << 24) | ((0x07) << 20) | ((0x0F) << 16) | (0x3FF)); 
  CAN2->BTR |=  (((can_configs[bitrate].TS2-1) & 0x07) << 20) | (((can_configs[bitrate].TS1-1) & 0x0F) << 16) | ((can_configs[bitrate].BRP-1) & 0x3FF);
  printRegister("CAN2->BTR=", CAN2->BTR);

  // Configure Filters to default values
  CAN1->FMR |=   0x1UL;                  // Set to filter initialization mode
  CAN1->FMR &= 0xFFFFC0FF;               // Clear CAN2 start bank
  printRegister("CAN1->FMR=", CAN1->FMR);

  // bxCAN has 28 filters.
  // These filters are used for both CAN1 and CAN2.
  // STM32F413 has CAN1 and CAN2, so CAN2 filters are offset by 14
  CAN1->FMR |= 0xE00;                    // Start bank for the CAN2 interface

  // Set fileter 0
  // Single 32-bit scale configuration 
  // Two 32-bit registers of filter bank x are in Identifier Mask mode
  // Filter assigned to FIFO 0 
  // Filter bank register to all 0
  CANSetFilter(0, 1, 0, 0, 0x0UL, 0x0UL); 

  // Set fileter 14
  // Single 32-bit scale configuration 
  // Two 32-bit registers of filter bank x are in Identifier Mask mode
  // Filter assigned to FIFO 0 
  // Filter bank register to all 0
  CANSetFilter(14, 1, 0, 0, 0x0UL, 0x0UL); 

  CAN1->FMR &= ~(0x1UL);                 // Deactivate initialization mode

  uint16_t TimeoutMilliseconds = 1000;
  bool can2 = false;
  CAN2->MCR &= ~(0x1UL);                 // Require CAN2 to normal mode  

  // Wait for normal mode
  // If the connection is not correct, it will not return to normal mode.
  for (uint16_t wait_ack = 0; wait_ack < TimeoutMilliseconds; wait_ack++) {
    //Serial.print("CAN2->MCR=0x");
    //Serial.print(CAN2->MCR, HEX);
    //Serial.print(" CAN2->MSR=0x");
    //Serial.println(CAN2->MSR, HEX);
    if ((CAN2->MSR & 0x1UL) == 0) {
      can2 = true;
      break;
    }
    delayMicroseconds(1000);
  }
  Serial.print("can2=");
  Serial.println(can2);
  if (can2) {
    Serial.println("CAN2 initialize ok");
  } else {
    Serial.println("CAN2 initialize fail!!");
  }

  bool can1 = false;
  CAN1->MCR &= ~(0x1UL);                 // Require CAN1 to normal mode 

  // Wait for normal mode
  // If the connection is not correct, it will not return to normal mode.
  for (uint16_t wait_ack = 0; wait_ack < TimeoutMilliseconds; wait_ack++) {
    //Serial.print("CAN1->MCR=0x");
    //Serial.print(CAN1->MCR, HEX);
    //Serial.print(" CAN1->MSR=0x");
    //Serial.println(CAN1->MSR, HEX);
    if ((CAN1->MSR & 0x1UL) == 0) {
      can1 = true;
      break;
    }
    delayMicroseconds(1000);
  }
  Serial.print("can1=");
  Serial.println(can1);
  if (can1) {
    Serial.println("CAN1 initialize ok");
  } else {
    Serial.println("CAN1 initialize fail!!");
    return false;
  }
  return true;
}

void CANMsgPrint(CAN_TypeDef * can, CAN_msg_t CAN_RX_msg) {
  if (can == CAN1) {
    Serial.print("CAN1 ");
  } else {
    Serial.print("CAN2 ");
  }
  if (CAN_RX_msg.format == EXTENDED_FORMAT) {
    Serial.print("Extended ID: 0x");
    if (CAN_RX_msg.id < 0x10000000) Serial.print("0");
    if (CAN_RX_msg.id < 0x1000000) Serial.print("0");
    if (CAN_RX_msg.id < 0x100000) Serial.print("0");
    if (CAN_RX_msg.id < 0x10000) Serial.print("0");
    if (CAN_RX_msg.id < 0x1000) Serial.print("0");
    if (CAN_RX_msg.id < 0x100) Serial.print("0");
    if (CAN_RX_msg.id < 0x10) Serial.print("0");
    Serial.print(CAN_RX_msg.id, HEX);
  } else {
    Serial.print("Standard ID: 0x");
    if (CAN_RX_msg.id < 0x100) Serial.print("0");
    if (CAN_RX_msg.id < 0x10) Serial.print("0");
    Serial.print(CAN_RX_msg.id, HEX);
    Serial.print("     ");
  }

  Serial.print(" DLC: ");
  Serial.print(CAN_RX_msg.len);
  if (CAN_RX_msg.type == DATA_FRAME) {
    Serial.print(" Data: ");
    for(int i=0; i<CAN_RX_msg.len; i++) {
      Serial.print("0x"); 
      Serial.print(CAN_RX_msg.data[i], HEX); 
      if (i != (CAN_RX_msg.len-1))  Serial.print(" ");
    }
    Serial.println();
  } else {
    Serial.println(" Data: REMOTE REQUEST FRAME");
  }
}

#define STM32_CAN_TIR_TXRQ              (1U << 0U)  // Bit 0: Transmit Mailbox Request
#define STM32_CAN_RIR_RTR               (1U << 1U)  // Bit 1: Remote Transmission Request
#define STM32_CAN_RIR_IDE               (1U << 2U)  // Bit 2: Identifier Extension
#define STM32_CAN_TIR_RTR               (1U << 1U)  // Bit 1: Remote Transmission Request
#define STM32_CAN_TIR_IDE               (1U << 2U)  // Bit 2: Identifier Extension

#define CAN_EXT_ID_MASK                 0x1FFFFFFFU
#define CAN_STD_ID_MASK                 0x000007FFU
 
/**
 * Decodes CAN messages from the data registers and populates a 
 * CAN message struct with the data fields.
 * 
 * @preconditions     - A valid CAN message is received
 * @params CAN_rx_msg - CAN message structure for reception
 * 
 */
void CANReceive(CAN_TypeDef * can, CAN_msg_t* CAN_rx_msg)
{
    uint32_t id = can->sFIFOMailBox[0].RIR;
    if ((id & STM32_CAN_RIR_IDE) == 0) { // Standard frame format
        CAN_rx_msg->format = STANDARD_FORMAT;;
        CAN_rx_msg->id = (CAN_STD_ID_MASK & (id >> 21U));
    } 
    else {                               // Extended frame format
        CAN_rx_msg->format = EXTENDED_FORMAT;;
        CAN_rx_msg->id = (CAN_EXT_ID_MASK & (id >> 3U));
    }

    if ((id & STM32_CAN_RIR_RTR) == 0) {  // Data frame
        CAN_rx_msg->type = DATA_FRAME;
    }
    else {                                // Remote frame
        CAN_rx_msg->type = REMOTE_FRAME;
    }

    
    CAN_rx_msg->len = (can->sFIFOMailBox[0].RDTR) & 0xFUL;
    
    CAN_rx_msg->data[0] = 0xFFUL &  can->sFIFOMailBox[0].RDLR;
    CAN_rx_msg->data[1] = 0xFFUL & (can->sFIFOMailBox[0].RDLR >> 8);
    CAN_rx_msg->data[2] = 0xFFUL & (can->sFIFOMailBox[0].RDLR >> 16);
    CAN_rx_msg->data[3] = 0xFFUL & (can->sFIFOMailBox[0].RDLR >> 24);
    CAN_rx_msg->data[4] = 0xFFUL &  can->sFIFOMailBox[0].RDHR;
    CAN_rx_msg->data[5] = 0xFFUL & (can->sFIFOMailBox[0].RDHR >> 8);
    CAN_rx_msg->data[6] = 0xFFUL & (can->sFIFOMailBox[0].RDHR >> 16);
    CAN_rx_msg->data[7] = 0xFFUL & (can->sFIFOMailBox[0].RDHR >> 24);
    
    can->RF0R |= 0x20UL;

}
 
/**
 * Encodes CAN messages using the CAN message struct and populates the 
 * data registers with the sent.
 * 
 * @params CAN_tx_msg - CAN message structure for transmission
 * 
 */
void CANSend(CAN_TypeDef * can, CAN_msg_t* CAN_tx_msg)
{
  volatile int count = 0;

  uint32_t out = 0;
  if (CAN_tx_msg->format == EXTENDED_FORMAT) { // Extended frame format
      out = ((CAN_tx_msg->id & CAN_EXT_ID_MASK) << 3U) | STM32_CAN_TIR_IDE;
  }
  else {                                       // Standard frame format
      out = ((CAN_tx_msg->id & CAN_STD_ID_MASK) << 21U);
  }

  // Remote frame
  if (CAN_tx_msg->type == REMOTE_FRAME) {
      out |= STM32_CAN_TIR_RTR;
  }

  can->sTxMailBox[0].TDTR &= ~(0xF);
  can->sTxMailBox[0].TDTR |= CAN_tx_msg->len & 0xFUL;
  
  can->sTxMailBox[0].TDLR  = (((uint32_t) CAN_tx_msg->data[3] << 24) |
                              ((uint32_t) CAN_tx_msg->data[2] << 16) |
                              ((uint32_t) CAN_tx_msg->data[1] <<  8) |
                              ((uint32_t) CAN_tx_msg->data[0]      ));
  can->sTxMailBox[0].TDHR  = (((uint32_t) CAN_tx_msg->data[7] << 24) |
                              ((uint32_t) CAN_tx_msg->data[6] << 16) |
                              ((uint32_t) CAN_tx_msg->data[5] <<  8) |
                              ((uint32_t) CAN_tx_msg->data[4]      ));

  // Send Go
  can->sTxMailBox[0].TIR = out | STM32_CAN_TIR_TXRQ;

  // Wait until the mailbox is empty
  while(can->sTxMailBox[0].TIR & 0x1UL && count++ < 1000000);

  // The mailbox don't becomes empty while loop
  if (can->sTxMailBox[0].TIR & 0x1UL) {
    Serial.println("Send Fail");
    Serial.println(can->ESR);
    Serial.println(can->MSR);
    Serial.println(can->TSR);
  }
}

 /**
 * Returns whether there are CAN messages available.
 *
 * @returns If pending CAN messages are in the CAN controller
 *
 */
uint8_t CANMsgAvail(CAN_TypeDef * can)
{
  // Check for pending FIFO 0 messages
  return can->RF0R & 0x3UL;
}

uint8_t counter = 0;
uint8_t frameLength = 0;
unsigned long previousMillis = 0;     // stores last time output was updated
const long interval = 1000;           // transmission interval (milliseconds)

void setup() {
  Serial.begin(115200);
  bool ret = CANInit(CAN_500KBPS, 0);  // CAN_RX mapped to PA11, CAN_TX mapped to PA12
  //bool ret = CANInit(CAN_500KBPS, 1);  // CAN_RX mapped to PG0, CAN_TX mapped to PG1
  //bool ret = CANInit(CAN_500KBPS, 2);  // CAN_RX mapped to PB8, CAN_TX mapped to PB9
  //bool ret = CANInit(CAN_500KBPS, 3);  // CAN_RX mapped to PD0, CAN_TX mapped to PD1
  //bool ret = CANInit(CAN_1000KBPS, 0);  // CAN_RX mapped to PA11, CAN_TX mapped to PA12
  //bool ret = CANInit(CAN_1000KBPS, 1);  // CAN_RX mapped to PG0, CAN_TX mapped to PG1
  //bool ret = CANInit(CAN_1000KBPS, 2);  // CAN_RX mapped to PB8, CAN_TX mapped to PB9
  //bool ret = CANInit(CAN_1000KBPS, 3);  // CAN_RX mapped to PD0, CAN_TX mapped to PD1
  if (!ret) while(true);
}

void loop() {
  CAN_msg_t CAN_TX_msg;
  CAN_msg_t CAN_RX_msg;
   
  CAN_TX_msg.data[0] = 0x00;
  CAN_TX_msg.data[1] = 0x01;
  CAN_TX_msg.data[2] = 0x02;
  CAN_TX_msg.data[3] = 0x03;
  CAN_TX_msg.data[4] = 0x04;
  CAN_TX_msg.data[5] = 0x05;
  CAN_TX_msg.data[6] = 0x06;
  CAN_TX_msg.data[7] = 0x07;
  CAN_TX_msg.len = frameLength;

  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    if ( ( counter % 2) == 0) {
      CAN_TX_msg.type = DATA_FRAME;
      if (CAN_TX_msg.len == 0) CAN_TX_msg.type = REMOTE_FRAME;
      CAN_TX_msg.format = EXTENDED_FORMAT;
      CAN_TX_msg.id = 0x32F413;
    } else {
      CAN_TX_msg.type = DATA_FRAME;
      if (CAN_TX_msg.len == 0) CAN_TX_msg.type = REMOTE_FRAME;
      CAN_TX_msg.format = STANDARD_FORMAT;
      CAN_TX_msg.id = 0x413;
    }
    CANSend(CAN_PORT, &CAN_TX_msg);
    frameLength++;
    if (frameLength == 9) frameLength = 0;
    counter++;
  }

  if(CANMsgAvail(CAN_PORT)) {
    CANReceive(CAN_PORT, &CAN_RX_msg);
    CANMsgPrint(CAN_PORT, CAN_RX_msg); 
  }

}
