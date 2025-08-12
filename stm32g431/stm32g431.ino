#define ENABLE_FILTER 0x00 // No filters are used.
//#define ENABLE_FILTER 0x01 // Use the standard filter. 
//#define ENABLE_FILTER 0x02 // Use the extended filter.
//#define ENABLE_FILTER 0x03 // Use both filter.

//-----------------------------------------------------------------
// IMPORTANT:
//   <ACANFD_STM32.h> should be included only once, generally from the .ino file
//   From an other file, include <ACANFD_STM32_from_cpp.h>
//-----------------------------------------------------------------

//#include <ACANFD_STM32.h>
#include "src/acanfd-stm32/ACANFD_STM32.h"

//-----------------------------------------------------------------


void setup () {
  Serial.begin (115200) ; // Use PA2/PA3 as STDOUT
  while (!Serial) {
    delay (50) ;
  }

  // Data Phase Bit Rate is not used.
  //ACANFD_STM32_Settings settings (125 * 1000, DataBitRateFactor::x5) ;
  //ACANFD_STM32_Settings settings (250 * 1000, DataBitRateFactor::x5) ;
  ACANFD_STM32_Settings settings (500 * 1000, DataBitRateFactor::x5) ;
  // ACANFD_STM32_Settings settings (1000 * 1000, DataBitRateFactor::x5) ;

  Serial.print ("Bit Rate prescaler: ") ;
  Serial.println (settings.mBitRatePrescaler) ;
  Serial.print ("Arbitration Phase segment 1: ") ;
  Serial.println (settings.mArbitrationPhaseSegment1) ;
  Serial.print ("Arbitration Phase segment 2: ") ;
  Serial.println (settings.mArbitrationPhaseSegment2) ;
  Serial.print ("Arbitration SJW: ") ;
  Serial.println (settings.mArbitrationSJW) ;
  Serial.print ("Actual Arbitration Bit Rate: ") ;
  Serial.print (settings.actualArbitrationBitRate ()) ;
  Serial.println (" bit/s") ;
  Serial.print ("Arbitration sample point: ") ;
  Serial.print (settings.arbitrationSamplePointFromBitStart ()) ;
  Serial.println ("%") ;
  Serial.print ("Exact Arbitration Bit Rate ? ") ;
  Serial.println (settings.exactArbitrationBitRate () ? "yes" : "no") ;
  Serial.print ("Data Phase segment 1: ") ;
  Serial.println (settings.mDataPhaseSegment1) ;
  Serial.print ("Data Phase segment 2: ") ;
  Serial.println (settings.mDataPhaseSegment2) ;
  Serial.print ("Data SJW: ") ;
  Serial.println (settings.mDataSJW) ;
  Serial.print ("Actual Data Bit Rate: ") ;
  Serial.print (settings.actualDataBitRate ()) ;
  Serial.println (" bit/s") ;
  Serial.print ("Data sample point: ") ;
  Serial.print (settings.dataSamplePointFromBitStart ()) ;
  Serial.println ("%") ;
  Serial.print ("Exact Data Bit Rate ? ") ;
  Serial.println (settings.exactDataBitRate () ? "yes" : "no") ;

  // Disable FD operation. A regular transceiver required.
  settings.mModuleMode = ACANFD_STM32_Settings::DISABLE_FD ;
  // Enable FD operation. FDCAN compatible transceiver required.
  //settings.mModuleMode = ACANFD_STM32_Settings::NORMAL_FD ;
  
  // Using PA11/PA12 as CAN port.
  settings.mTxPin = PA12;
  settings.mRxPin = PA11;
  // Using PB8/PB9 as CAN port.
  //settings.mTxPin = PB9;
  //settings.mRxPin = PB8;
  // Using PD0/PD1 as CAN port.
  //settings.mTxPin = PD1;
  //settings.mRxPin = PD0;

  // Automatic retransmission of messages not transmitted successfully enabled
  settings.mEnableRetransmission = false;
  // Automatic retransmission disabled
  //settings.mEnableRetransmission = true;

  // Set the filter
  ACANFD_STM32_StandardFilters standardFilters;
  ACANFD_STM32_ExtendedFilters extendedFilters;
  if ((ENABLE_FILTER & 0x01) == 0x01) {
    //--- Add dual filter: identifier1, identifier2 (2 matching identifiers)
    standardFilters.addDual (0x103, 0x104, ACANFD_STM32_FilterAction::FIFO0) ;
    //--- Add single filter: identifier (1 matching identifier)
    standardFilters.addSingle (0x105, ACANFD_STM32_FilterAction::FIFO0) ;
    //--- Reject standard frames that do not match any filter
    settings.mNonMatchingStandardFrameReception = ACANFD_STM32_FilterAction::REJECT ;
  }
  if ((ENABLE_FILTER & 0x02) == 0x02) {
    //--- Add single filter: identifier (1 matching identifier)
    extendedFilters.addSingle (0x103, ACANFD_STM32_FilterAction::FIFO0) ;
    //--- Add dual filter: identifier1, identifier2 (2 matching identifiers)
    extendedFilters.addDual (0x104, 0x105, ACANFD_STM32_FilterAction::FIFO0) ;
    //--- Reject extended frames that do not match any filter
    settings.mNonMatchingExtendedFrameReception = ACANFD_STM32_FilterAction::REJECT ;
  }
  uint32_t errorCode = fdcan1.beginFD (settings, standardFilters, extendedFilters) ;
  if (0 == errorCode) {
    Serial.println ("fdcan1 ok") ;
  }else{
    Serial.print ("Error can: 0x") ;
    Serial.println (errorCode, HEX) ;
    while(true);
  }
  Serial.print ("Systick ") ;
  Serial.println (SysTick->LOAD + 1) ;
}

//-----------------------------------------------------------------

static uint32_t gSendDate = 0 ;
static uint32_t gSentCount = 0 ;
static uint32_t gReceivedCount = 0 ;
static uint8_t gFrameLength = 0;

//-----------------------------------------------------------------

void loop () {
  if (gSendDate < millis ()) {
    // Serial.print ("CPU frequency: ") ;
    // Serial.print (F_CPU) ;
    // Serial.println (" Hz") ;
    // Serial.print ("PCLK1 frequency: ") ;
    // Serial.print (HAL_RCC_GetPCLK1Freq ()) ;
    // Serial.println (" Hz") ;
    // Serial.print ("PCLK2 frequency: ") ;
    // Serial.print (HAL_RCC_GetPCLK2Freq ()) ;
    // Serial.println (" Hz") ;
    // Serial.print ("HCLK frequency: ") ;
    // Serial.print (HAL_RCC_GetHCLKFreq ()) ;
    // Serial.println (" Hz") ;
    // Serial.print ("SysClock frequency: ") ;
    // Serial.print (HAL_RCC_GetSysClockFreq ()) ;
    // Serial.println (" Hz") ;
    // Serial.print ("CAN Clock: ") ;
    // Serial.print (fdcanClock ()) ;
    // Serial.println (" Hz") ;
    
    CANFDMessage CAN_TX_msg ;
    CAN_TX_msg.data [0] = 0 ;
    CAN_TX_msg.data [1] = 1 ;
    CAN_TX_msg.data [2] = 2 ;
    CAN_TX_msg.data [3] = 3 ;
    CAN_TX_msg.data [4] = 4 ;
    CAN_TX_msg.data [5] = 5 ;
    CAN_TX_msg.data [6] = 6 ;
    CAN_TX_msg.data [7] = 7 ;
    CAN_TX_msg.len = gFrameLength;
    
    if ( ( gSentCount % 2) == 0) {
      CAN_TX_msg.type = CANFDMessage::CAN_DATA;
      if (CAN_TX_msg.len == 0) CAN_TX_msg.type = CANFDMessage::CAN_REMOTE;
      CAN_TX_msg.ext = true;
      CAN_TX_msg.id = 0x320431 ;
    } else {
      CAN_TX_msg.type = CANFDMessage::CAN_DATA;
      if (CAN_TX_msg.len == 0) CAN_TX_msg.type = CANFDMessage::CAN_REMOTE;
      CAN_TX_msg.ext = false;
      CAN_TX_msg.id = 0x431 ;
    }
    const uint32_t sendStatus = fdcan1.tryToSendReturnStatusFD (CAN_TX_msg) ;
    if (sendStatus == 0) {
      gSendDate += 1000 ;
      gSentCount += 1 ;
      gFrameLength += 1;
      if (gFrameLength == 9) gFrameLength = 0;
      //Serial.print ("fdcan1 sent: ") ;
      //Serial.println (gSentCount) ;
    } else {
      Serial.print("sendStatus=");
      Serial.println(sendStatus);
      while(true);
    }
  }
  CANFDMessage CAN_RX_msg ;
  if (fdcan1.receiveFD0 (CAN_RX_msg)) {
    gReceivedCount += 1 ;
    //Serial.print ("fdcan1 received: ") ;
    //Serial.println (gReceivedCount) ;
    if (CAN_RX_msg.ext == true) {
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
    if (CAN_RX_msg.type == CANFDMessage::CAN_DATA) {
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
}
