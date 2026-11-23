//------------------------------------------------------------------------------
//    THIS FILE IS SPECIFIC TO FDCAN MODULES WITH FIXED SIZE RAM SECTIONS
//------------------------------------------------------------------------------

#include "ACANFD_STM32_from_cpp.h"

//------------------------------------------------------------------------------

#ifndef HAS_PROGRAMMABLE_FDCAN_RAM_SECTIONS
  #error "HAS_PROGRAMMABLE_FDCAN_RAM_SECTIONS is not defined"
#endif

#if HAS_PROGRAMMABLE_FDCAN_RAM_SECTIONS == false

//------------------------------------------------------------------------------

static const uint32_t WORD_COUNT_FOR_PAYLOAD_64_BYTES = 18 ;

//------------------------------------------------------------------------------
//    Constructor
//------------------------------------------------------------------------------

ACANFD_STM32::ACANFD_STM32 (volatile FDCAN_GlobalTypeDef * inPeripheralModuleBasePointer,
                            const uint32_t inRamBaseAddress,
                            const std::optional <IRQs> inIRQs,
                            const std::initializer_list <ACANFD_STM32::PinPort> & inTxPinArray,
                            const std::initializer_list <ACANFD_STM32::PinPort> & inRxPinArray) :
mRamBaseAddress (inRamBaseAddress),
mTxPinArray (inTxPinArray),
mRxPinArray (inRxPinArray),
mIRQs (inIRQs),
mPeripheralPtr (inPeripheralModuleBasePointer) {
}

//------------------------------------------------------------------------------
//    beginFD method
//------------------------------------------------------------------------------

uint32_t ACANFD_STM32::beginFD (const ACANFD_STM32_Settings & inSettings,
                                const ACANFD_STM32_ExtendedFilters & inExtendedFilters) {
  return beginFD (inSettings, ACANFD_STM32_StandardFilters (), inExtendedFilters) ;
}

//------------------------------------------------------------------------------
//    beginFD method
//------------------------------------------------------------------------------

uint32_t ACANFD_STM32::beginFD (const ACANFD_STM32_Settings & inSettings,
                                const ACANFD_STM32_StandardFilters & inStandardFilters,
                                const ACANFD_STM32_ExtendedFilters & inExtendedFilters) {
  uint32_t errorFlags = inSettings.checkBitSettingConsistency () ;


//------------------------------------------------------ Check settings
  if (inStandardFilters.count () > 28) {
    errorFlags |= kTooManyStandardFilters ;
  }
  if (inExtendedFilters.count () > 8) {
    errorFlags |= kTooManyExtendedFilters ;
  }

//---------------------------------------------- Configure TxPin
  //Serial.print("inSettings.mTxPin=");
  //Serial.println(inSettings.mTxPin);
  bool pinFound = inSettings.mTxPin == 255 ; // Use default TxPin ?
  auto txPinIterator = mTxPinArray.begin () ;
  while (!pinFound && (txPinIterator != mTxPinArray.end ())) {
    pinFound = txPinIterator->mPinName == inSettings.mTxPin ;
    if (!pinFound) {
      txPinIterator ++ ;
    }
  }
  if (pinFound) {
    //Serial.print("txPinIterator->mPinName=");
    //Serial.println(txPinIterator->mPinName);
    GPIO_TypeDef * gpio = set_GPIO_Port_Clock (txPinIterator->mPinName >> 4) ;
    const uint32_t pinIndex = txPinIterator->mPinName & 0x0F ;
    const uint32_t txPinMask = uint32_t (1) << pinIndex ;
    LL_GPIO_SetPinMode  (gpio, txPinMask, LL_GPIO_MODE_ALTERNATE) ;
    const uint32_t output = inSettings.mOpenCollectorOutput
      ? LL_GPIO_OUTPUT_OPENDRAIN
      : LL_GPIO_OUTPUT_PUSHPULL
    ;
    LL_GPIO_SetPinOutputType (gpio, txPinMask, output) ;
    LL_GPIO_SetPinSpeed (gpio, txPinMask, LL_GPIO_SPEED_HIGH) ;
    if (pinIndex < 8) {
      LL_GPIO_SetAFPin_0_7 (gpio, txPinMask, txPinIterator->mPinAlternateMode) ;
    }else{
      LL_GPIO_SetAFPin_8_15 (gpio, txPinMask, txPinIterator->mPinAlternateMode) ;
    }
  }else{ // Tx Pin not found
    errorFlags |= kInvalidTxPin ;
  }


//---------------------------------------------- Configure RxPin
  //Serial.print("inSettings.mRxPin=");
  //Serial.println(inSettings.mRxPin);
  auto rxPinIterator = mRxPinArray.begin () ;
  pinFound = inSettings.mRxPin == 255 ; // Use default RxPin ?
  while (!pinFound && (rxPinIterator != mRxPinArray.end ())) {
    pinFound = rxPinIterator->mPinName == inSettings.mRxPin ;
    if (!pinFound) {
      rxPinIterator ++ ;
    }
  }
  if (pinFound) {
    //Serial.print("rxPinIterator->mPinName=");
    //Serial.println(rxPinIterator->mPinName);
    GPIO_TypeDef * gpio = set_GPIO_Port_Clock (rxPinIterator->mPinName >> 4) ;
    const uint32_t pinIndex = rxPinIterator->mPinName & 0x0F ;
    const uint32_t rxPinMask = uint32_t (1) << pinIndex ;
    const uint32_t input = inSettings.mInputPullup
      ? LL_GPIO_PULL_UP
      : LL_GPIO_PULL_NO
    ;
    LL_GPIO_SetPinPull (gpio, rxPinMask, input) ;
    LL_GPIO_SetPinMode (gpio, rxPinMask, LL_GPIO_MODE_ALTERNATE) ;
    if (pinIndex < 8) {
      LL_GPIO_SetAFPin_0_7 (gpio, rxPinMask, rxPinIterator->mPinAlternateMode) ;
    }else{
      LL_GPIO_SetAFPin_8_15 (gpio, rxPinMask, rxPinIterator->mPinAlternateMode) ;
    }
  }else{ // Rx Pin not found
    errorFlags |= kInvalidRxPin ;
  }

//------------------------------------------------------ Start configuring CAN module
  mPeripheralPtr->CCCR = FDCAN_CCCR_INIT ;
  while ((mPeripheralPtr->CCCR & FDCAN_CCCR_INIT) == 0) {
  }


//------------------------------------------------------ Enable configuration change
  mPeripheralPtr->CCCR = FDCAN_CCCR_INIT | FDCAN_CCCR_CCE ;
  mPeripheralPtr->CCCR = FDCAN_CCCR_INIT | FDCAN_CCCR_CCE | FDCAN_CCCR_TEST ;
  //uint32_t cccr = FDCAN_CCCR_BRSE | FDCAN_CCCR_FDOE | FDCAN_CCCR_PXHD ;
  uint32_t cccr = FDCAN_CCCR_BRSE | FDCAN_CCCR_PXHD ;

//------------------------------------------------------ Select mode
  mPeripheralPtr->TEST = 0 ;
  switch (inSettings.mModuleMode) {
  case ACANFD_STM32_Settings::NORMAL_FD :
    cccr |= FDCAN_CCCR_FDOE ;
    break ;
  case ACANFD_STM32_Settings::INTERNAL_LOOP_BACK :
    mPeripheralPtr->TEST = FDCAN_TEST_LBCK ;
    cccr |= FDCAN_CCCR_MON | FDCAN_CCCR_TEST ;
    break ;
  case ACANFD_STM32_Settings::EXTERNAL_LOOP_BACK :
    mPeripheralPtr->TEST = FDCAN_TEST_LBCK ;
    cccr |= FDCAN_CCCR_TEST ;
    break ;
  case ACANFD_STM32_Settings::BUS_MONITORING :
    cccr |= FDCAN_CCCR_MON ;
    break ;
  }
  if (!inSettings.mEnableRetransmission) {
    cccr |= FDCAN_CCCR_DAR ;
  }


//------------------------------------------------------ Set nominal Bit Timing and Prescaler
  mPeripheralPtr->NBTP =
    ((inSettings.mArbitrationSJW - 1) << 25)
  |
    ((inSettings.mBitRatePrescaler - 1) << 16)
  |
    ((inSettings.mArbitrationPhaseSegment1 - 1) << 8)
  |
    ((inSettings.mArbitrationPhaseSegment2 - 1) << 0)
  ;


//------------------------------------------------------ Set data Bit Timing and Prescaler
  mPeripheralPtr->DBTP =
    ((inSettings.mBitRatePrescaler - 1) << 16)
  |
    ((inSettings.mDataPhaseSegment1 - 1) << 8)
  |
    ((inSettings.mDataPhaseSegment2 - 1) << 4)
  |
    ((inSettings.mDataSJW - 1) << 0)
  |
  // Enable Transceiver Delay Compensation ?
    ((inSettings.mTransceiverDelayCompensation > 0) ? FDCAN_DBTP_TDC : 0)
  ;


//------------------------------------------------------ Transmitter Delay Compensation
  mPeripheralPtr->TDCR = inSettings.mTransceiverDelayCompensation << 8 ;


//------------------------------------------------------ Global Filter Configuration
  mPeripheralPtr->RXGFC =
    (uint32_t (inSettings.mNonMatchingStandardFrameReception) << FDCAN_RXGFC_ANFS_Pos)
  |
    (uint32_t (inSettings.mNonMatchingExtendedFrameReception) << FDCAN_RXGFC_ANFE_Pos)
  |
    (uint32_t (inSettings.mDiscardReceivedStandardRemoteFrames) << FDCAN_RXGFC_RRFS_Pos)
  |
    (uint32_t (inSettings.mDiscardReceivedExtendedRemoteFrames) << FDCAN_RXGFC_RRFE_Pos)
  |
    (inStandardFilters.count () << FDCAN_RXGFC_LSS_Pos) // Standard filter count (up to 28)
  |
    (inExtendedFilters.count () << FDCAN_RXGFC_LSE_Pos) // Standard filter count (up to 8)
  ;


//-------------------- Allocate Standard ID Filters (0 ... 28 elements -> 0 ... 28 words)
  mStandardFilterCallBackArray.setCapacity (inStandardFilters.count ()) ;
  for (uint32_t i=0 ; i<inStandardFilters.count () ; i++) {
    uint32_t * address = (uint32_t *) (mRamBaseAddress + 4 * i) ;
    * address = inStandardFilters.filterAtIndex (i) ;
    mStandardFilterCallBackArray.append (inStandardFilters.callBackAtIndex (i)) ;
  }

//-------------------- Allocate Extended ID Filters (0 ... 8 elements -> 0 ... 16 words)
  mExtendedFilterCallBackArray.setCapacity (inExtendedFilters.count ()) ;
  for (uint32_t i=0 ; i<inExtendedFilters.count () ; i++) {
    uint32_t * address = (uint32_t *) (mRamBaseAddress + 0x70 + 8 * i) ;
    address [0] = inExtendedFilters.firstWordAtIndex (i) ;
    address [1] = inExtendedFilters.secondWordAtIndex (i) ;
    mExtendedFilterCallBackArray.append (inExtendedFilters.callBackAtIndex (i)) ;
  }


  if (errorFlags == 0) {
  //------------------------------------------------------ Configure Driver buffers
    mDriverTransmitFIFO.initWithSize (inSettings.mDriverTransmitFIFOSize) ;
    mDriverReceiveFIFO0.initWithSize (inSettings.mDriverReceiveFIFO0Size) ;
    mDriverReceiveFIFO1.initWithSize (inSettings.mDriverReceiveFIFO1Size) ;
    mNonMatchingStandardMessageCallBack = inSettings.mNonMatchingStandardMessageCallBack ;
    mNonMatchingExtendedMessageCallBack = inSettings.mNonMatchingExtendedMessageCallBack ;
  //------------------------------------------------------ Interrupts
    if (mIRQs) {
      uint32_t interruptRegister = FDCAN_IE_TCE ; // Enable Transmission Completed Interrupt
      interruptRegister |= FDCAN_IE_RF0NE ; // Receive FIFO 0 Non Empty
      interruptRegister |= FDCAN_IE_RF1NE ; // Receive FIFO 1 Non Empty
      mPeripheralPtr->IE = interruptRegister ;
      mPeripheralPtr->TXBTIE = ~0U ;
      mPeripheralPtr->ILS = FDCAN_ILS_RXFIFO1 | FDCAN_ILS_RXFIFO0 ; // Received message on IRQ1, others on IRQ0
      NVIC_EnableIRQ (mIRQs.value ().mIRQ0) ;
      NVIC_EnableIRQ (mIRQs.value ().mIRQ1) ;
      mPeripheralPtr->ILE = FDCAN_ILE_EINT1 | FDCAN_ILE_EINT0 ;
    }else{
      mPeripheralPtr->IE = 0 ; // All interrupts disabled
    }
  //------------------------------------------------------ Activate CAN controller
    mPeripheralPtr->CCCR = FDCAN_CCCR_INIT | FDCAN_CCCR_CCE | cccr ;
    mPeripheralPtr->CCCR = cccr ; // Reset INIT bit
    while ((mPeripheralPtr->CCCR & FDCAN_CCCR_INIT) != 0) { }
  }
//--- Return error code (0 --> no error)
  return errorFlags ;
}

//------------------------------------------------------------------------------
//    end
//------------------------------------------------------------------------------

void ACANFD_STM32::end (void) {
//--- Disable interrupts
  if (mIRQs) {
    NVIC_DisableIRQ (mIRQs.value ().mIRQ0) ;
    NVIC_DisableIRQ (mIRQs.value ().mIRQ1) ;
  }
//--- Free receive FIFOs
  mDriverReceiveFIFO0.free () ;
  mDriverReceiveFIFO1.free () ;
//--- Free transmit FIFO
  mDriverTransmitFIFO.free () ;
}

//------------------------------------------------------------------------------
//   poll
//------------------------------------------------------------------------------

void ACANFD_STM32::poll (void) {
  if (!mIRQs) {
    noInterrupts () ;
      isr0 () ;
      isr1 () ;
    interrupts () ;
  }
}

//------------------------------------------------------------------------------
//   RECEPTION
//------------------------------------------------------------------------------

bool ACANFD_STM32::availableFD0 (void) {
  noInterrupts () ;
    const bool hasMessage = !mDriverReceiveFIFO0.isEmpty () ;
  interrupts () ;
  return hasMessage ;
}

//------------------------------------------------------------------------------

bool ACANFD_STM32::receiveFD0 (CANFDMessage & outMessage) {
  noInterrupts () ;
    const bool hasMessage = mDriverReceiveFIFO0.remove (outMessage) ;
  interrupts () ;
  return hasMessage ;
}

//------------------------------------------------------------------------------

bool ACANFD_STM32::availableFD1 (void) {
  noInterrupts () ;
    const bool hasMessage = !mDriverReceiveFIFO1.isEmpty () ;
  interrupts () ;
  return hasMessage ;
}

//------------------------------------------------------------------------------

bool ACANFD_STM32::receiveFD1 (CANFDMessage & outMessage) {
  noInterrupts () ;
    const bool hasMessage = mDriverReceiveFIFO1.remove (outMessage) ;
  interrupts () ;
  return hasMessage ;
}

//------------------------------------------------------------------------------

bool ACANFD_STM32::dispatchReceivedMessage (void) {
  CANFDMessage message ;
  bool result = false ;
  if (receiveFD0 (message)) {
    result = true ;
    internalDispatchReceivedMessage (message) ;
  }
  if (receiveFD1 (message)) {
    result = true ;
    internalDispatchReceivedMessage (message) ;
  }
  return result ;
}

//------------------------------------------------------------------------------

bool ACANFD_STM32::dispatchReceivedMessageFIFO0 (void) {
  CANFDMessage message ;
  const bool result = receiveFD0 (message) ;
  if (result) {
    internalDispatchReceivedMessage (message) ;
  }
  return result ;
}

//------------------------------------------------------------------------------

bool ACANFD_STM32::dispatchReceivedMessageFIFO1 (void) {
  CANFDMessage message ;
  const bool result = receiveFD1 (message) ;
  if (result) {
    internalDispatchReceivedMessage (message) ;
  }
  return result ;
}

//------------------------------------------------------------------------------

void ACANFD_STM32::internalDispatchReceivedMessage (const CANFDMessage & inMessage) {
  const uint32_t filterIndex = inMessage.idx ;
  ACANFDCallBackRoutine callBack = nullptr ;
  if (inMessage.ext) {
    if (filterIndex == 255) {
      callBack = mNonMatchingStandardMessageCallBack ;
    }else if (filterIndex < mExtendedFilterCallBackArray.count ()) {
      callBack = mExtendedFilterCallBackArray [filterIndex] ;
    }
  }else{ // Standard message
    if (filterIndex == 255) {
      callBack = mNonMatchingExtendedMessageCallBack ;
    }else if (filterIndex < mStandardFilterCallBackArray.count ()) {
      callBack = mStandardFilterCallBackArray [filterIndex] ;
    }
  }
  if (callBack != nullptr) {
    callBack (inMessage) ;
  }
}

//------------------------------------------------------------------------------
//   EMISSION
//------------------------------------------------------------------------------

bool ACANFD_STM32::sendBufferNotFullForIndex (const uint32_t inMessageIndex) {
  bool canSend = false ;
  noInterrupts () ;
    if (inMessageIndex == 0) { // Send via Tx FIFO ?
      canSend = !mDriverTransmitFIFO.isFull () ;
    }else{ // Send via dedicaced Tx Buffer ?
      const uint32_t numberOfDedicacedTxBuffers = (mPeripheralPtr->TXBC >> 16) & 0x3F ;
      if (inMessageIndex <= numberOfDedicacedTxBuffers) {
        const uint32_t txBufferIndex = inMessageIndex - 1 ;
        canSend = (mPeripheralPtr->TXBRP & (1U << txBufferIndex)) == 0 ;
      }
    }
  interrupts () ;
  return canSend ;
}

//------------------------------------------------------------------------------

uint32_t ACANFD_STM32::tryToSendReturnStatusFD (const CANFDMessage & inMessage) {
  noInterrupts () ;
    uint32_t sendStatus = 0 ; // Ok
    if (!inMessage.isValid ()) {
      sendStatus = kInvalidMessage ;
    }else if (inMessage.idx == 0) { // Send via Tx FIFO ?
      const uint32_t txfqs = mPeripheralPtr->TXFQS ;
      const uint32_t hardwareTransmitFifoFreeLevel = txfqs & 0x3F ;
      if ((hardwareTransmitFifoFreeLevel > 0) && mDriverTransmitFIFO.isEmpty ()) {
        const uint32_t putIndex = (txfqs >> 16) & 0x1F ;
        writeTxBuffer (inMessage, putIndex) ;
      }else if (!mDriverTransmitFIFO.isFull ()) {
        mDriverTransmitFIFO.append (inMessage) ;
      }else{
        sendStatus = kTransmitBufferOverflow ;
      }
    }else{ // Send via dedicaced Tx Buffer ?
      const uint32_t numberOfDedicacedTxBuffers = (mPeripheralPtr->TXBC >> 16) & 0x3F ;
      if (inMessage.idx <= numberOfDedicacedTxBuffers) {
        const uint32_t txBufferIndex = inMessage.idx - 1 ;
        const bool hardwareTxBufferIsEmpty = (mPeripheralPtr->TXBRP & (1U << txBufferIndex)) == 0 ;
        if (hardwareTxBufferIsEmpty) {
          writeTxBuffer (inMessage, txBufferIndex) ;
        }else{
          sendStatus = kTransmitBufferOverflow ;
        }
      }else{
        sendStatus = kTransmitBufferIndexTooLarge ;
      }
    }
  interrupts () ;
  return sendStatus ;
}

//------------------------------------------------------------------------------

void ACANFD_STM32::writeTxBuffer (const CANFDMessage & inMessage, const uint32_t inTxBufferIndex) {
//--- Compute Tx Buffer address
  volatile uint32_t * txBufferPtr = (uint32_t *) (mRamBaseAddress + 0x278) ;
  txBufferPtr += inTxBufferIndex * WORD_COUNT_FOR_PAYLOAD_64_BYTES ;
//--- Identifier and extended bit
  uint32_t element0 ;
  if (inMessage.ext) {
    element0 = (inMessage.id & 0x1FFFFFFFU) | (1U << 30) ;
  }else{
    element0 = (inMessage.id & 0x7FFU) << 18 ;
  }
//--- Control
  uint32_t lengthCode ;
  if (inMessage.len > 48) {
    lengthCode = 15 ;
  }else if (inMessage.len > 32) {
    lengthCode = 14 ;
  }else if (inMessage.len > 24) {
    lengthCode = 13 ;
  }else if (inMessage.len > 20) {
    lengthCode = 12 ;
  }else if (inMessage.len > 16) {
    lengthCode = 11 ;
  }else if (inMessage.len > 12) {
    lengthCode = 10 ;
  }else if (inMessage.len > 8) {
    lengthCode = 9 ;
  }else{
    lengthCode = inMessage.len ;
  }
  uint32_t element1 = uint32_t (lengthCode) << 16 ;
//---
  switch (inMessage.type) {
  case CANFDMessage::CAN_REMOTE :
    element0 |= 1 << 29 ; // Set RTR bit
    break ;
  case CANFDMessage::CAN_DATA :
    txBufferPtr [2] = inMessage.data32 [0] ;
    txBufferPtr [3] = inMessage.data32 [1] ;
    break ;
  case CANFDMessage::CANFD_NO_BIT_RATE_SWITCH :
    { element1 |= 1 << 21 ; // Set FDF bit
      const uint32_t wc = (inMessage.len + 3) / 4 ;
      for (uint32_t i=0 ; i<wc ; i++) {
        txBufferPtr [i+2] = inMessage.data32 [i] ;
      }
    }
    break ;
  case CANFDMessage::CANFD_WITH_BIT_RATE_SWITCH :
    { element1 |= (1 << 21) | (1 << 20) ; // Set FDF and BRS bits
      const uint32_t wc = (inMessage.len + 3) / 4 ;
      for (uint32_t i=0 ; i<wc ; i++) {
        txBufferPtr [i+2] = inMessage.data32 [i] ;
      }
    }
    break ;
  }
  txBufferPtr [0] = element0 ;
  txBufferPtr [1] = element1 ;
//---Request transmit
  mPeripheralPtr->TXBAR = 1U << inTxBufferIndex ;
}

//------------------------------------------------------------------------------
//   INTERRUPT SERVICE ROUTINES
//------------------------------------------------------------------------------

static void getMessageFrom (const uint32_t * inMessageRamAddress,
                            CANFDMessage & outMessage) {
  const uint32_t w0 = inMessageRamAddress [0] ;
  outMessage.id = w0 & 0x1FFFFFFF ;
  const bool remote = (w0 & (1 << 29)) != 0 ;
  outMessage.ext = (w0 & (1 << 30)) != 0 ;
//   const bool esi = (w0 & (1 << 31)) != 0 ;
  if (!outMessage.ext) {
    outMessage.id >>= 18 ;
  }
  const uint32_t w1 = inMessageRamAddress [1] ;
  const uint32_t dlc = (w1 >> 16) & 0xF ;
  static const uint8_t CANFD_LENGTH_FROM_CODE [16] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 12, 16, 20, 24, 32, 48, 64} ;
  outMessage.len = CANFD_LENGTH_FROM_CODE [dlc] ;
  const bool fdf = (w1 & (1 << 21)) != 0 ;
  const bool brs = (w1 & (1 << 20)) != 0 ;
  if (fdf) { // CANFD frame
    outMessage.type = brs ? CANFDMessage::CANFD_WITH_BIT_RATE_SWITCH : CANFDMessage::CANFD_NO_BIT_RATE_SWITCH ;
  }else if (remote) {
    outMessage.type = CANFDMessage::CAN_REMOTE ;
  }else{
    outMessage.type = CANFDMessage::CAN_DATA ;
  }
//--- Filter index
  if ((w1 & (1U << 31)) != 0) { // Filter index available ?
    outMessage.idx = 255 ; // Not available
  }else{
    const uint32_t filterIndex = (w1 >> 24) & 0x7F ;
    outMessage.idx = uint8_t (filterIndex) ;
  }
//--- Get data
  if (outMessage.type != CANFDMessage::CAN_REMOTE) {
    const uint32_t wc = (outMessage.len + 3) / 4 ;
    for (uint32_t i=0 ; i<wc ; i++) {
      outMessage.data32 [i] = inMessageRamAddress [i+2] ;
    }
  }
}

//------------------------------------------------------------------------------

void ACANFD_STM32::isr0 (void) {
//--- Interrupt Acknowledge
  mPeripheralPtr->IR = FDCAN_IR_TC ;
//--- Write message into transmit fifo ?
  bool writeMessage = true ;
  CANFDMessage message ;
  while (writeMessage) {
    const uint32_t txfqs = mPeripheralPtr->TXFQS ;
    const uint32_t txFifoFreeLevel = txfqs & 0x3F ;
    if ((txFifoFreeLevel > 0) && mDriverTransmitFIFO.remove (message)) {
      const uint32_t putIndex = (txfqs >> 16) & 0x1F ;
      writeTxBuffer (message, putIndex) ;
    }else{
      writeMessage = false ;
    }
  }
}

//------------------------------------------------------------------------------

void ACANFD_STM32::isr1 (void) {
//--- Interrupt Acknowledge
  const uint32_t it = mPeripheralPtr->IR ;
  const uint32_t ack = it & (FDCAN_IR_RF0N | FDCAN_IR_RF1N) ;
  mPeripheralPtr->IR = ack ;
//--- Get messages
  CANFDMessage message ;
  bool loop = true ;
  while (loop) {
  //--- Get from FIFO 0
    const uint32_t rxf0s = mPeripheralPtr->RXF0S ;
    const uint32_t fifo0NotEmpty = (rxf0s & 0x7FU) > 0 ;
    if (fifo0NotEmpty) {
    //--- Get read index
      const uint32_t readIndex = (rxf0s >> 8) & 0x3F ;
    //--- Compute message RAM address
      const uint32_t * address = (uint32_t *) (mRamBaseAddress + 0x00B0) ;
      address += readIndex * WORD_COUNT_FOR_PAYLOAD_64_BYTES ;
    //--- Get message
      getMessageFrom (address, message) ;
    //--- Clear receive flag
      mPeripheralPtr->RXF0A = readIndex ;
    //--- Enter message into driver receive buffer 0
      mDriverReceiveFIFO0.append (message) ;
    }
  //--- Get from FIFO 1
    const uint32_t rxf1s = mPeripheralPtr->RXF1S ;
    const uint32_t fifo1NotEmpty = (rxf1s & 0x7FU) > 0 ;
    if (fifo1NotEmpty) {
    //--- Get read index
      const uint32_t readIndex = (rxf1s >> 8) & 0x3F ;
    //--- Compute message RAM address
      const uint32_t * address = (uint32_t *) (mRamBaseAddress + 0x0188) ;
      address += readIndex * WORD_COUNT_FOR_PAYLOAD_64_BYTES ;
    //--- Get message
      getMessageFrom (address, message) ;
    //--- Clear receive flag
      mPeripheralPtr->RXF1A = readIndex ;
    //--- Enter message into driver receive buffer 1
      mDriverReceiveFIFO1.append (message) ;
    }
  //--- Loop ?
    loop = fifo0NotEmpty || fifo1NotEmpty ;
  }
}

//------------------------------------------------------------------------------
//--- Status Flags (returns 0 if no error)
//  Bit 0 : hardware RxFIFO 0 overflow
//  Bit 1 : driver RxFIFO 0 overflow
//  Bit 2 : hardware RxFIFO 1 overflow
//  Bit 3 : driver RxFIFO 2 overflow
//  Bit 4 : bus off

uint32_t ACANFD_STM32::statusFlags (void) const {
  uint32_t result = 0 ; // Ok
  const uint32_t ir = mPeripheralPtr->IR ;
  const uint32_t psr = mPeripheralPtr->PSR ;
//--- Hardware RxFIFO 0 overflow ?
  if ((ir & FDCAN_IR_RF0L) != 0) {
    result |= 1U << 0 ;
  }
//--- Driver RxFIFO 0 overflow ?
  if (mDriverReceiveFIFO0.didOverflow ()) {
    result |= 1U << 1 ;
  }
//--- Hardware RxFIFO 1 overflow ?
  if ((ir & FDCAN_IR_RF1L) != 0) {
    result |= 1U << 2 ;
  }
//--- Driver RxFIFO 1 overflow ?
  if (mDriverReceiveFIFO1.didOverflow ()) {
    result |= 1U << 3 ;
  }
//--- Bus off ?
  if ((psr & FDCAN_PSR_BO) != 0) {
    result |= 1U << 4 ;
  }
//---
  return result ;
}

//------------------------------------------------------------------------------

ACANFD_STM32::BusStatus::BusStatus (volatile FDCAN_GlobalTypeDef * inModulePtr) :
mErrorCount (uint16_t (inModulePtr->ECR)),
mProtocolStatus (inModulePtr->PSR) {
}

//------------------------------------------------------------------------------

#endif
