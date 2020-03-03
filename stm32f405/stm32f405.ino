#define DEBUG 0

enum BITRATE{CAN_50KBPS, CAN_100KBPS, CAN_125KBPS, CAN_250KBPS, CAN_500KBPS, CAN_1000KBPS};

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

CAN_bit_timing_config_t can_configs[6] = {{2, 12, 56}, {2, 12, 28}, {2, 13, 21}, {2, 11, 12}, {2, 11, 6}, {1, 5, 6}};

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
 * Initializes the CAN filter registers.
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
 *                    =1:Not used
 *                    =2:CAN1_RX mapped to PB8,  CAN1_TX mapped to PB9 (not available on 36-pin package)
 *                       CAN2_RX mapped to PB12, CAN2_TX mapped to PB13
 *                    =3:CAN1_RX mapped to PD0,  CAN1_TX mapped to PD1 (available on 100-pin and 144-pin package)
 *                       CAN2_RX mapped to PB12, CAN2_TX mapped to PB13
 *
 */
bool CANInit(enum BITRATE bitrate, int remap)
{
  // Reference manual
  // https://www.st.com/content/ccc/resource/technical/document/reference_manual/3d/6d/5a/66/b4/99/40/d4/DM00031020.pdf/files/DM00031020.pdf/jcr:content/translations/en.DM00031020.pdf

  RCC->APB1ENR |= 0x2000000UL;         // Enable CAN1 clock 
  RCC->APB1ENR |= 0x4000000UL;         // Enable CAN2 clock 
  
  if (remap == 0) {
    // CAN1
    RCC->AHB1ENR |= 0x1;               // Enable GPIOA clock 

    printRegister("GPIOA_AFR(b)=", GPIOA->AFR[1]);
    GPIOA->AFR[1]  &= 0xFFF00FFF;      // Reset alternate function PA12 and PA11   
    GPIOA->AFR[1]  |= 0x99000;         // Set AF9 PA12 and PA11
    printRegister("GPIOA_AFR(a)=", GPIOA->AFR[1]);

    printRegister("GPIOA_MODER(b)=", GPIOA->MODER);
    GPIOA->MODER   &= 0xFC3FFFFF;      // Reset mode PA12 and PA11 
    GPIOA->MODER   |= 0x2800000;       // set Alternate function mode PA12 and PA11
    printRegister("GPIOA_MODER(a)=", GPIOA->MODER);

    printRegister("GPIOA_OSPEEDR(b)=", GPIOA->OSPEEDR);
    GPIOA->OSPEEDR &= 0xFC3FFFFF;      // Reset speed PA12 and PA11
    GPIOA->OSPEEDR |= 0x2800000;       // set high speed PA12 and PA11
    printRegister("GPIOA_OSPEEDR(a)=", GPIOA->OSPEEDR);

    printRegister("GPIOA_OTYPER(b)=", GPIOA->OTYPER);
    GPIOA->OTYPER  &= 0xE7FF;          // Reset Output push-pull PA12 and PA11
    printRegister("GPIOA_OTYPER(a)=", GPIOA->OTYPER);

    printRegister("GPIOA_PUPDR(b)=", GPIOA->PUPDR);
    GPIOA->PUPDR   &= 0xFC3FFFFF;      // Reset port pull-up/pull-down PA12 and PA11
    printRegister("GPIOA_PUPDR(a)=", GPIOA->PUPDR);
    
    // CAN2
    RCC->AHB1ENR |= 0x2;               // Enable GPIOB clock 

    GPIOB->AFR[0]  &= 0xF00FFFFF;      // Reset alternate function PB6 and PB5   
    GPIOB->AFR[0]  |= 0x9900000;       // Set AF9 PB6 and PB5
    printRegister("GPIOB_AFR(a)=", GPIOB->AFR[0]);

    GPIOB->MODER   &= 0xFFFFC3FF;      // Reset mode PB6 and PB5 
    GPIOB->MODER   |= 0x2800;          // set Alternate function mode PB6 and PB5
    printRegister("GPIOB_MODER(a)=", GPIOB->MODER);

    GPIOB->OSPEEDR &= 0xFFFFC3FF;      // Reset speed PB6 and PB5
    GPIOB->OSPEEDR |= 0x2800;          // set high speed PB6 and PB5
    printRegister("GPIOB_OSPEEDR(a)=", GPIOB->OSPEEDR);

    GPIOB->OTYPER  &= 0xFF9F;          // Reset Output push-pull PB6 and PB5
    printRegister("GPIOB_OTYPER(a)=", GPIOB->OTYPER);

    GPIOB->PUPDR   &= 0xFFFFC3FF;      // Reset port pull-up/pull-down PB6 and PB5
    printRegister("GPIOB_PUPDR(a)=", GPIOB->PUPDR);
  }

  if (remap == 2) {
    // CAN1
    RCC->AHB1ENR |= 0x2;               // Enable GPIOB clock 

    GPIOB->AFR[1]  &= 0xFFFFFF00;      // Reset alternate function PB9 and PB8
    GPIOB->AFR[1]  |= 0x99;            // Set AF9 PB9 and PB8
    printRegister("GPIOB_AFR(a)=", GPIOB->AFR[1]);

    GPIOB->MODER   & 0xFFF0FFFF;      // Reset mode PB9 and PB8 
    GPIOB->MODER   |= 0xA0000;         // set Alternate function mode PB9 and PB8
    printRegister("GPIOB_MODER(a)=", GPIOB->MODER);

    GPIOB->OSPEEDR &= 0xFFF0FFFF;      // Reset speed PB9 and PB8
    GPIOB->OSPEEDR |= 0xA0000;         // set high speed PB9 and PB8
    printRegister("GPIOB_OSPEEDR(a)=", GPIOB->OSPEEDR);

    GPIOB->OTYPER  &= 0xFcFF;          // Reset Output push-pull PB9 and PB8
    printRegister("GPIOB_OTYPER(a)=", GPIOB->OTYPER);

    GPIOB->PUPDR   &= 0xFFF0FFFF;      // Reset port pull-up/pull-down PB9 and PB8
    printRegister("GPIOB_PUPDR(a)=", GPIOB->PUPDR);

    // CAN2
    RCC->AHB1ENR |= 0x2;               // Enable GPIOB clock 

    GPIOB->AFR[1]  &= 0xFF00FFFF;      // Reset alternate function PB13 and PB12   
    GPIOB->AFR[1]  |= 0x990000;        // Set AF9 PB13 and PB12
    printRegister("GPIOB_AFR(a)=", GPIOB->AFR[1]);

    GPIOB->MODER   &= 0xF0FFFFFF;      // Reset mode PB13 and PB12 
    GPIOB->MODER   |= 0xA000000;        // set Alternate function mode PB13 and PB12
    printRegister("GPIOB_MODER(a)=", GPIOB->MODER);

    GPIOB->OSPEEDR &= 0xF0FFFFFF;      // Reset speed PB13 and PB12
    GPIOB->OSPEEDR |= 0xA000000;        // set high speed PB13 and PB12
    printRegister("GPIOB_OSPEEDR(a)=", GPIOB->OSPEEDR);

    GPIOB->OTYPER  &= 0xCFFF;          // Reset Output push-pull PB13 and PB12
    printRegister("GPIOB_OTYPER(a)=", GPIOB->OTYPER);

    GPIOB->PUPDR   &= 0xF0FFFFFF;      // Reset port pull-up/pull-down PB13 and PB12
    printRegister("GPIOB_PUPDR(a)=", GPIOB->PUPDR);
  }
    
  if (remap == 3) {
    // CAN1
    RCC->AHB1ENR |= 0x8;               // Enable GPIOD clock 

    GPIOD->AFR[0]  &= 0xFFFFFF00;      // Reset alternate function PD1 and PD0
    GPIOD->AFR[0]  |= 0x99;            // Set AF9 PD1 and PD0
    printRegister("GPIOD_AFR(a)=", GPIOD->AFR[0]);

    GPIOD->MODER   &= 0xFFFFFFF0;      // Reset mode PD1 and PD0 
    GPIOD->MODER   |= 0xA;             // set Alternate function mode PD1 and PD0
    printRegister("GPIOD_MODER(a)=", GPIOD->MODER);

    GPIOD->OSPEEDR &= 0xFFFFFFF0;      // Reset speed PD1 and PD0
    GPIOD->OSPEEDR |= 0xA;             // set high speed PD1 and PD0
    printRegister("GPIOD_OSPEEDR(a)=", GPIOD->OSPEEDR);

    GPIOD->OTYPER  &= 0xFFFc;          // Reset Output push-pull PD1 and PD0
    printRegister("GPIOD_OTYPER(a)=", GPIOD->OTYPER);

    GPIOD->PUPDR   &= 0xFFFFFFF0;      // Reset port pull-up/pull-down PD1 and PD0
    printRegister("GPIOD_PUPDR(a)=", GPIOD->PUPDR);

    // CAN2
    RCC->AHB1ENR |= 0x2;               // Enable GPIOB clock 

    GPIOB->AFR[1]  &= 0xFF00FFFF;      // Reset alternate function PB13 and PB12   
    GPIOB->AFR[1]  |= 0x990000;        // Set AF9 PB13 and PB12
    printRegister("GPIOB_AFR(a)=", GPIOB->AFR[1]);

    GPIOB->MODER   &= 0xF0FFFFFF;      // Reset mode PB13 and PB12 
    GPIOB->MODER   |= 0xA000000;        // set Alternate function mode PB13 and PB12
    printRegister("GPIOB_MODER(a)=", GPIOB->MODER);

    GPIOB->OSPEEDR &= 0xF0FFFFFF;      // Reset speed PB13 and PB12
    GPIOB->OSPEEDR |= 0xA000000;        // set high speed PB13 and PB12
    printRegister("GPIOB_OSPEEDR(a)=", GPIOB->OSPEEDR);

    GPIOB->OTYPER  &= 0xCFFF;          // Reset Output push-pull PB13 and PB12
    printRegister("GPIOB_OTYPER(a)=", GPIOB->OTYPER);

    GPIOB->PUPDR   &= 0xF0FFFFFF;      // Reset port pull-up/pull-down PB13 and PB12
    printRegister("GPIOB_PUPDR(a)=", GPIOB->PUPDR);
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
  CAN1->BTR &= ~(((0x03) << 24) | ((0x07) << 20) | ((0x0F) << 16) | (0x1FF)); 
  CAN1->BTR |=  (((can_configs[bitrate].TS2-1) & 0x07) << 20) | (((can_configs[bitrate].TS1-1) & 0x0F) << 16) | ((can_configs[bitrate].BRP-1) & 0x1FF);
  printRegister("CAN1->BTR=", CAN1->BTR);

  CAN2->BTR &= ~(((0x03) << 24) | ((0x07) << 20) | ((0x0F) << 16) | (0x1FF)); 
  CAN2->BTR |=  (((can_configs[bitrate].TS2-1) & 0x07) << 20) | (((can_configs[bitrate].TS1-1) & 0x0F) << 16) | ((can_configs[bitrate].BRP-1) & 0x1FF);
  printRegister("CAN2->BTR=", CAN2->BTR);

  // Configure Filters to default values
  CAN1->FMR  |=   0x1UL;                 // Set to filter initialization mode
  CAN1->FMR  &= 0xFFFFC0FF;              // Clear CAN2 start bank
  printRegister("CAN1->FMR=", CAN1->FMR);

  // bxCAN has 28 filters.
  // These filters are used for both CAN1 and CAN2.
  // STM32F405 has CAN1 and CAN2, so CAN2 filters are offset by 14
  CAN1->FMR  |= 0xE00;                   // Start bank for the CAN2 interface

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

#if 0
  CAN1->FA1R &= ~(0x1UL);                // Deactivate filter 0
  //CAN1->FS1R |=   0x1UL;                 // Set first filter to single 32 bit configuration
  CAN1->FS1R  =   0x0FFFFFFF;            // Set all filter to single 32 bit configuration

  CAN1->sFilterRegister[0].FR1 = 0x0UL;  // Set filter0 registers to 0
  CAN1->sFilterRegister[0].FR2 = 0x0UL;  // Set filter0 registers to 0

  CAN1->sFilterRegister[14].FR1 = 0x0UL; // Set filter14 registers to 0
  CAN1->sFilterRegister[14].FR2 = 0x0UL; // Set filter14 registers to 0
  
  //CAN1->FM1R &= ~(0x1UL);               // Set filter to mask mode
  CAN1->FM1R  = 0;                       // Set all filter to mask mode

  CAN1->FFA1R &= ~(0x1UL);               // Apply filter0 to FIFO 0  
  CAN1->FFA1R &= ~(0x4000UL);            // Apply filter14 to FIFO 0  
  printRegister("CAN1->FFA1R=", CAN1->FFA1R);

  CAN1->FA1R  |=   0x1UL;                // Activate filter0
  CAN1->FA1R  |=   0x4000UL;             // Activate filter14
  printRegister("CAN1->FA1R=", CAN1->FA1R);
#endif
  
  CAN1->FMR   &= ~(0x1UL);               // Deactivate initialization mode

  uint16_t TimeoutMilliseconds = 1000;
  bool can2 = false;
  CAN2->MCR   &= ~(0x1UL);               // Require CAN2 to normal mode  

  // Wait for normal mode
  // If the connection is not correct, it will not return to normal mode.
  for (uint16_t wait_ack = 0; wait_ack < TimeoutMilliseconds; wait_ack++) {
    if ((CAN2->MSR & 0x1UL) == 0) {
      can2 = true;
      break;
    }
    delayMicroseconds(1000);
  }
  //Serial.print("can2=");
  //Serial.println(can2);
  if (can2) {
    Serial.println("CAN2 initialize ok");
  } else {
    Serial.println("CAN2 initialize fail!!");
  }

  bool can1 = false;
  CAN1->MCR   &= ~(0x1UL);               // Require CAN1 to normal mode 

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
 * @preconditions     - A valid CAN message is received
 * @params CAN_rx_msg - CAN message structure for reception
 * 
 */
void CANReceive(uint8_t ch, CAN_msg_t* CAN_rx_msg)
{
  if(ch == 1) {
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
  } // end CAN1

  if(ch == 2) {
    uint32_t id = CAN2->sFIFOMailBox[0].RIR;
    if ((id & STM32_CAN_RIR_IDE) == 0) { // Standard frame format
        CAN_rx_msg->id = (CAN_STD_ID_MASK & (id >> 21U));
    } 
    else {                               // Extended frame format
        CAN_rx_msg->id = (CAN_EXT_ID_MASK & (id >> 3U));
    }

    if ((id & STM32_CAN_RIR_RTR) != 0) {
        CAN_rx_msg->id |= CAN_FRAME_RTR;
    }

    
    CAN_rx_msg->len = (CAN2->sFIFOMailBox[0].RDTR) & 0xFUL;
    
    CAN_rx_msg->data[0] = 0xFFUL &  CAN2->sFIFOMailBox[0].RDLR;
    CAN_rx_msg->data[1] = 0xFFUL & (CAN2->sFIFOMailBox[0].RDLR >> 8);
    CAN_rx_msg->data[2] = 0xFFUL & (CAN2->sFIFOMailBox[0].RDLR >> 16);
    CAN_rx_msg->data[3] = 0xFFUL & (CAN2->sFIFOMailBox[0].RDLR >> 24);
    CAN_rx_msg->data[4] = 0xFFUL &  CAN2->sFIFOMailBox[0].RDHR;
    CAN_rx_msg->data[5] = 0xFFUL & (CAN2->sFIFOMailBox[0].RDHR >> 8);
    CAN_rx_msg->data[6] = 0xFFUL & (CAN2->sFIFOMailBox[0].RDHR >> 16);
    CAN_rx_msg->data[7] = 0xFFUL & (CAN2->sFIFOMailBox[0].RDHR >> 24);
    
    CAN2->RF0R |= 0x20UL;
  } // END CAN2

}
 
/**
 * Encodes CAN messages using the CAN message struct and populates the 
 * data registers with the sent.
 * 
 * @params CAN_tx_msg - CAN message structure for transmission
 * 
 */
void CANSend(uint8_t ch, CAN_msg_t* CAN_tx_msg)
{
  volatile int count = 0;

  uint32_t out = 0;
  if (CAN_tx_msg->id > CAN_STD_ID_MASK) { // Extended frame format
      out = ((CAN_tx_msg->id & CAN_EXT_ID_MASK) << 3U) | STM32_CAN_TIR_IDE;
  }
  else {                                  // Standard frame format
      out = ((CAN_tx_msg->id & CAN_STD_ID_MASK) << 21U);
  }

  // Remote frame
  if (CAN_tx_msg->id & CAN_FRAME_RTR) {
      out |= STM32_CAN_TIR_RTR;
  }

  if (ch == 1) {
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

    // Wait until the mailbox is empty
    while(CAN1->sTxMailBox[0].TIR & 0x1UL && count++ < 1000000);

    // The mailbox don't becomes empty while loop
    if (CAN1->sTxMailBox[0].TIR & 0x1UL) {
      Serial.println("Send Fail");
      Serial.println(CAN1->ESR);
      Serial.println(CAN1->MSR);
      Serial.println(CAN1->TSR);
    }
  } // end CAN1

  if (ch == 2) {
    CAN2->sTxMailBox[0].TDTR &= ~(0xF);
    CAN2->sTxMailBox[0].TDTR |= CAN_tx_msg->len & 0xFUL;
    
    CAN2->sTxMailBox[0].TDLR  = (((uint32_t) CAN_tx_msg->data[3] << 24) |
                                 ((uint32_t) CAN_tx_msg->data[2] << 16) |
                                 ((uint32_t) CAN_tx_msg->data[1] <<  8) |
                                 ((uint32_t) CAN_tx_msg->data[0]      ));
    CAN2->sTxMailBox[0].TDHR  = (((uint32_t) CAN_tx_msg->data[7] << 24) |
                                 ((uint32_t) CAN_tx_msg->data[6] << 16) |
                                 ((uint32_t) CAN_tx_msg->data[5] <<  8) |
                                 ((uint32_t) CAN_tx_msg->data[4]      ));

    // Send Go
    CAN2->sTxMailBox[0].TIR = out | STM32_CAN_TIR_TXRQ;

    // Wait until the mailbox is empty
    while(CAN2->sTxMailBox[0].TIR & 0x1UL && count++ < 1000000);

    // The mailbox don't becomes empty while loop
    if (CAN2->sTxMailBox[0].TIR & 0x1UL) {
      Serial.println("Send Fail");
      Serial.println(CAN1->ESR);
      Serial.println(CAN1->MSR);
      Serial.println(CAN1->TSR);
    }
  } // end CAN2

}

 /**
 * Returns whether there are CAN messages available.
 *
 * @returns If pending CAN messages are in the CAN controller
 *
 */
uint8_t CANMsgAvail(uint8_t ch)
{
  if (ch == 1) {
    // Check for pending FIFO 0 messages
    return CAN1->RF0R & 0x3UL;
  } // end CAN1

  if (ch == 2) {
    // Check for pending FIFO 0 messages
    return CAN2->RF0R & 0x3UL;
  } // end CAN2

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

  uint8_t send_ch = 1;
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    if ( ( counter % 2) == 0) {
        CAN_TX_msg.id = 0x1032F405;
    } else {
        CAN_TX_msg.id = 0x105;
    }
    CANSend(send_ch, &CAN_TX_msg);
    counter++;
  }

  uint8_t recv_ch = 1;
  if(CANMsgAvail(recv_ch)) {
    CANReceive(recv_ch, &CAN_RX_msg);

    if (CAN_RX_msg.id > 0x7ff) {
      Serial.print("Channel:");
      Serial.print(recv_ch);
      Serial.print(" Extended ID: 0x");
      if (CAN_RX_msg.id < 0x10000000) Serial.print("0");
      if (CAN_RX_msg.id < 0x1000000) Serial.print("00");
      if (CAN_RX_msg.id < 0x100000) Serial.print("000");
      if (CAN_RX_msg.id < 0x10000) Serial.print("0000");
      Serial.print(CAN_RX_msg.id, HEX);
    } else {
      Serial.print("Channel:");
      Serial.print(recv_ch);
      Serial.print(" Standard ID: 0x");
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
