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

CAN_bit_timing_config_t can_configs[6] = {{2, 13, 45}, {2, 15, 20}, {2, 13, 18}, {2, 13, 9}, {2, 15, 4}, {2, 15, 2}};

void printRegister(char * buf, uint32_t reg) {
    Serial.print(buf);
    Serial.print(reg, HEX);
    Serial.println();
}

//extern CAN_bit_timing_config_t can_configs[6];
    
/**
 * Initializes the CAN controller with specified bit rate.
 *
 * @params: bitrate - Specified bitrate. If this value is not one of the defined constants, bit rate will be defaulted to 125KBS
 * @params: remap   - Select CAN port. 
 *                    =0:CAN_RX mapped to PA11, CAN_TX mapped to PA12
 *                    =1:Not used
 *                    =2:CAN_RX mapped to PB8, CAN_TX mapped to PB9 (not available on 36-pin package)
 *                    =3:CAN_RX mapped to PD0, CAN_TX mapped to PD1 (not available on 36-pin package)
 *
 */
void CANInit(enum BITRATE bitrate, int remap)
{
    // https://www.st.com/content/ccc/resource/technical/document/reference_manual/4a/19/6e/18/9d/92/43/32/DM00043574.pdf/files/DM00043574.pdf/jcr:content/translations/en.DM00043574.pdf


    RCC->APB1ENR |= 0x2000000UL;         // Enable CAN clock 

    if (remap == 0) {
      RCC->AHBENR |= 0x20000UL;          // Enable GPIOA clock 

      printRegister("GPIO_AFR(b)=", GPIOA->AFR[1]);
      GPIOA->AFR[1]  &= 0xFFF00FFF;      // Reset alternate function PA12 and PA11   
      GPIOA->AFR[1]  |= 0x99000;         // Set AF9 PA12 and PA11
      printRegister("GPIO_AFR(a)=", GPIOA->AFR[1]);
      
      printRegister("GPIO_MODER(b)=", GPIOA->MODER);
      GPIOA->MODER   &= 0xFC3FFFFF;      // Reset mode PA12 and PA11 
      GPIOA->MODER   |= 0x2800000;       // set Alternate function mode PA12 and PA11
      printRegister("GPIO_MODER(a)=", GPIOA->MODER);
 
      printRegister("GPIO_OSPEEDR(b)=", GPIOA->OSPEEDR);
      GPIOA->OSPEEDR &= 0xFC3FFFFF;      // Reset speed PB12 and PA11
      GPIOA->OSPEEDR |= 0x3C00000;       // set high speed PB12 and PA11
      printRegister("GPIO_OSPEEDR(a)=", GPIOA->OSPEEDR);
  
      printRegister("GPIO_OTYPER(b)=", GPIOA->OTYPER);
      GPIOA->OTYPER  &= 0xE7FF;          // Reset Output push-pull PA12 and PA11
      printRegister("GPIO_OTYPER(a)=", GPIOA->OTYPER);
  
      printRegister("GPIO_PUPDR(b)=", GPIOA->PUPDR);
      GPIOA->PUPDR   &= 0xFC3FFFFF;      // Reset port pull-up/pull-down PA12 and PA11
      printRegister("GPIO_PUPDR(a)=", GPIOA->PUPDR);
    }

    if (remap == 2) {
      RCC->AHBENR |= 0x40000UL;          // Enable GPIOB clock 

      printRegister("GPIO_AFR(a)=", GPIOB->AFR[1]);
      GPIOB->AFR[1]  &= 0xFFFFFF00;      // Reset alternate function PB9 and PB8   
      GPIOB->AFR[1]  |= 0x99;            // Set AF9 PB9 and PB8
      printRegister("GPIO_AFR(b)=", GPIOB->AFR[1]);
      
      printRegister("GPIO_MODER(b)=", GPIOB->MODER);
      GPIOB->MODER   &= 0xFFF0FFFF;      // Reset mode PB9 and PB8 
      GPIOB->MODER   |= 0xA0000;         // set Alternate function mode PB9 and PB8
      printRegister("GPIO_MODER(a)=", GPIOB->MODER);
 
      printRegister("GPIO_OSPEEDR(b)=", GPIOB->OSPEEDR);
      GPIOB->OSPEEDR &= 0xFFF0FFFF;      // Reset speed PB9 and PB8
      GPIOB->OSPEEDR |= 0xF0000;         // set high speed PB9 and PB8
      printRegister("GPIO_OSPEEDR(a)=", GPIOB->OSPEEDR);
  
      printRegister("GPIO_OTYPER(b)=", GPIOB->OTYPER);
      GPIOB->OTYPER  &= 0xFCFF;          // Reset Output push-pull PB9 and PB8
      printRegister("GPIO_OTYPER(a)=", GPIOB->OTYPER);
  
      printRegister("GPIO_PUPDR(b)=", GPIOB->PUPDR);
      GPIOB->PUPDR   &= 0xFFF0FFFF;      // Reset port pull-up/pull-down PB9 and PB8
      printRegister("GPIO_PUPDR(a)=", GPIOB->PUPDR);
    }

    if (remap == 3) {
      RCC->AHBENR |= 0x100000UL;         // Enable GPIOD clock 

      printRegister("GPIO_AFR(a)=", GPIOD->AFR[0]);
      GPIOD->AFR[0]  &= 0xFFFFFF00;      // Reset alternate function PD1 and PD0   
      GPIOD->AFR[0]  |= 0x99;            // Set AF9 PD1 and PD0
      printRegister("GPIO_AFR(b)=", GPIOD->AFR[0]);
      
      printRegister("GPIO_MODER(b)=", GPIOD->MODER);
      GPIOD->MODER   &= 0xFFFFFFF0;      // Reset mode PD1 and PD0 
      GPIOD->MODER   |= 0xA;             // set Alternate function mode PD1 and PD0
      printRegister("GPIO_MODER(a)=", GPIOD->MODER);
 
      printRegister("GPIO_OSPEEDR(b)=", GPIOD->OSPEEDR);
      GPIOD->OSPEEDR &= 0xFFFFFFF0;      // Reset speed PD1 and PD0
      GPIOD->OSPEEDR |= 0xF;             // set high speed PD1 and PD0
      printRegister("GPIO_OSPEEDR(a)=", GPIOD->OSPEEDR);
  
      printRegister("GPIO_OTYPER(b)=", GPIOD->OTYPER);
      GPIOD->OTYPER  &= 0xFFFC;          // Reset Output push-pull PD1 and PD0
      printRegister("GPIO_OTYPER(a)=", GPIOD->OTYPER);
  
      printRegister("GPIO_PUPDR(b)=", GPIOD->PUPDR);
      GPIOD->PUPDR   &= 0xFFFFFFF0;      // Reset port pull-up/pull-down PD1 and PD0
      printRegister("GPIO_PUPDR(a)=", GPIOD->PUPDR);

    }

    //CAN1->MCR = 0x51UL;              // Set CAN to initialization mode(No automatic retransmission)
    CAN->MCR = 0x41UL;                // Set CAN to initialization mode(With automatic retransmission)

    // Set bit rates 
    CAN->BTR &= ~(((0x03) << 24) | ((0x07) << 20) | ((0x0F) << 16) | (0x1FF)); 
    CAN->BTR |=  (((can_configs[bitrate].TS2-1) & 0x07) << 20) | (((can_configs[bitrate].TS1-1) & 0x0F) << 16) | ((can_configs[bitrate].BRP-1) & 0x1FF);
  
    // Configure Filters to default values
    //CAN->FM1R |= 0x1C << 8;              // Assign all filters to CAN1
    CAN->FMR  |=   0x1UL;                // Set to filter initialization mode
    CAN->FA1R &= ~(0x1UL);               // Deactivate filter 0
    CAN->FS1R |=   0x1UL;                // Set first filter to single 32 bit configuration
  
    CAN->sFilterRegister[0].FR1 = 0x0UL; // Set filter registers to 0
    CAN->sFilterRegister[0].FR2 = 0x0UL; // Set filter registers to 0
    CAN->FM1R &= ~(0x1UL);               // Set filter to mask mode
  
    CAN->FFA1R &= ~(0x1UL);              // Apply filter to FIFO 0  
    CAN->FA1R  |=   0x1UL;               // Activate filter 0
    
    CAN->FMR   &= ~(0x1UL);              // Deactivate initialization mode
    CAN->MCR   &= ~(0x1UL);              // Set CAN to normal mode 
  
    while (CAN->MSR & 0x1UL);            // Wait for Initialization acknowledge
 
}
 
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
 * @params CAN_rx_msg - CAN message struct that will be populated
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
 * @preconditions A valid CAN message is received
 * @params CAN_rx_msg - CAN message struct that will be populated
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
  CANInit(CAN_1000KBPS, 0);  // CAN_RX mapped to PA11, CAN_TX mapped to PA12
  //CANInit(CAN_1000KBPS, 2);  // CAN_RX mapped to PB8, CAN_TX mapped to PB9
  //CANInit(CAN_1000KBPS, 3);  // CAN_RX mapped to PD0, CAN_TX mapped to PD1
}

void loop() {
  CAN_msg_t CAN_TX_msg;
  CAN_msg_t CAN_rx_msg;
   
  CAN_TX_msg.data[0] = counter;
  CAN_TX_msg.data[1] = 0x01;
  CAN_TX_msg.data[2] = 0x02;
  CAN_TX_msg.data[3] = 0x03;
  CAN_TX_msg.len = 8;

  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    if ( ( counter % 2) == 0) {
        CAN_TX_msg.id = 0x1032F303;
    } else {
        CAN_TX_msg.id = 0x103;
    }
    CANSend(&CAN_TX_msg);
    counter++;
  }
  
  if(CANMsgAvail()) {
    CANReceive(&CAN_rx_msg);

    if (CAN_rx_msg.id > 0x7ff) {
      Serial.print("Extended ID: 0x");
      if (CAN_rx_msg.id < 0x10000000) Serial.print("0");
      if (CAN_rx_msg.id < 0x1000000) Serial.print("00");
      if (CAN_rx_msg.id < 0x100000) Serial.print("000");
      if (CAN_rx_msg.id < 0x10000) Serial.print("0000");
      Serial.print(CAN_rx_msg.id, HEX);
    } else {
      Serial.print("Standard ID: 0x");
      if (CAN_rx_msg.id < 0x100) Serial.print("0");
      if (CAN_rx_msg.id < 0x10) Serial.print("00");
      Serial.print(CAN_rx_msg.id, HEX);
      Serial.print("     ");
    }

    Serial.print(" DLC: ");
    Serial.print(CAN_rx_msg.len);
    Serial.print(" Data: ");
    for(int i=0; i<CAN_rx_msg.len; i++) {
      Serial.print("0x"); 
      Serial.print(CAN_rx_msg.data[i], HEX); 
      if (i != (CAN_rx_msg.len-1))  Serial.print(" ");
    }
    Serial.println();
  }
    
  

}
