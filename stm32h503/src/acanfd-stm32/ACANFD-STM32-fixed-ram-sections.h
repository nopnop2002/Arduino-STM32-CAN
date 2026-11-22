//------------------------------------------------------------------------------
//    THIS FILE IS SPECIFIC TO FDCAN MODULES WITH FIXED SIZE RAM SECTIONS
//------------------------------------------------------------------------------

#pragma once

//------------------------------------------------------------------------------

#include "ACANFD_STM32_Settings.h"
#include "ACANFD_STM32_FIFO.h"
#include "ACANFD_STM32_CANFDMessage.h"

#include <optional>

//------------------------------------------------------------------------------

class ACANFD_STM32 {

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  //  TX and Rx pins available port
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  public: class PinPort final {
    public: const uint8_t mPinName ;
    public: const uint8_t mPinAlternateMode ;

    public: PinPort (const uint8_t inPinName,
                     const uint8_t inPinAlternateMode) :
    mPinName (inPinName),
    mPinAlternateMode (inPinAlternateMode) {
    }
  } ;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  //  IRQs
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  public: class IRQs final {
    public: const IRQn_Type mIRQ0 ;
    public: const IRQn_Type mIRQ1 ;

    public: IRQs (const IRQn_Type inIRQ0,
                  const IRQn_Type inIRQ1) :
    mIRQ0 (inIRQ0),
    mIRQ1 (inIRQ1) {
    }
  } ;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  //  Bus Status
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  public: class BusStatus {
    public: BusStatus (volatile FDCAN_GlobalTypeDef * inModulePtr) ;
    public: const uint16_t mErrorCount ; // Copy of ECR register
    public: const uint32_t mProtocolStatus ; // Copy of PSR register
    public: inline uint16_t txErrorCount (void) const { return isBusOff () ? 256 : uint8_t (mErrorCount) ; }
    public: inline uint8_t rxErrorCount (void) const { return uint8_t (mErrorCount >> 8) ; }
    public: inline bool isBusOff (void) const { return (mProtocolStatus & (1 << 7)) != 0 ; }
    public: inline uint8_t transceiverDelayCompensationOffset (void) const { return uint8_t (mProtocolStatus >> 16) & 0x7F ; }
  } ;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  //  ACANFD_STM32 class
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

//-------------------- Constructor

  public: ACANFD_STM32 (volatile FDCAN_GlobalTypeDef * inPeripheralModuleBasePointer,
                        const uint32_t inRamBaseAddress,
                        const std::optional <IRQs> inIRQs,
                        const std::initializer_list <ACANFD_STM32::PinPort> & inTxPinArray,
                        const std::initializer_list <ACANFD_STM32::PinPort> & inRxPinArray) ;

//-------------------- begin; returns a result code :
//  0 : Ok
//  other: every bit denotes an error
  public: static const uint32_t kMessageRamAllocatedSizeTooSmall       = 1 << 20 ;
  public: static const uint32_t kMessageRamOverflow                    = 1 << 21 ;
  public: static const uint32_t kHardwareRxFIFO0SizeGreaterThan64      = 1 << 22 ;
  public: static const uint32_t kHardwareTransmitFIFOSizeGreaterThan32 = 1 << 23 ;
  public: static const uint32_t kDedicacedTransmitTxBufferCountGreaterThan30 = 1 << 24 ;
  public: static const uint32_t kTxBufferCountGreaterThan32            = 1 << 25 ;
  public: static const uint32_t kHardwareTransmitFIFOSizeEqualToZero   = 1 << 26 ;
  public: static const uint32_t kHardwareRxFIFO1SizeGreaterThan64      = 1 << 27 ;
  public: static const uint32_t kTooManyStandardFilters                = 1 << 28 ;
  public: static const uint32_t kTooManyExtendedFilters                = 1 << 29 ;
  public: static const uint32_t kInvalidTxPin                          = 1 << 30 ;
  public: static const uint32_t kInvalidRxPin                          = 1 << 31 ;

  public: uint32_t beginFD (const ACANFD_STM32_Settings & inSettings,
                            const ACANFD_STM32_StandardFilters & inStandardFilters = ACANFD_STM32_StandardFilters (),
                            const ACANFD_STM32_ExtendedFilters & inExtendedFilters = ACANFD_STM32_ExtendedFilters ()) ;

  public: uint32_t beginFD (const ACANFD_STM32_Settings & inSettings,
                            const ACANFD_STM32_ExtendedFilters & inExtendedFilters) ;

//-------------------- end
  public: void end (void) ;

//--- Testing send buffer
  public: bool sendBufferNotFullForIndex (const uint32_t inTxBufferIndex) ;

//--- Transmitting messages and return status (returns 0 if ok)
  public: uint32_t tryToSendReturnStatusFD (const CANFDMessage & inMessage) ;
  public: static const uint32_t kInvalidMessage              = 1 ;
  public: static const uint32_t kTransmitBufferIndexTooLarge = 2 ;
  public: static const uint32_t kTransmitBufferOverflow      = 3 ;

  public: inline uint32_t transmitFIFOSize (void) const { return mDriverTransmitFIFO.size () ; }
  public: inline uint32_t transmitFIFOCount (void) const { return mDriverTransmitFIFO.count () ; }
  public: inline uint32_t transmitFIFOPeakCount (void) const { return mDriverTransmitFIFO.peakCount () ; }

//--- Receiving messages
  public: bool availableFD0 (void) ;
  public: bool receiveFD0 (CANFDMessage & outMessage) ;
  public: bool availableFD1 (void) ;
  public: bool receiveFD1 (CANFDMessage & outMessage) ;
  public: bool dispatchReceivedMessage (void) ;
  public: bool dispatchReceivedMessageFIFO0 (void) ;
  public: bool dispatchReceivedMessageFIFO1 (void) ;

//--- Driver Transmit buffer
  protected: ACANFD_STM32_FIFO mDriverTransmitFIFO ;

//--- Poll
  public: void poll (void) ;

//--- Driver receive FIFO 0
  protected: ACANFD_STM32_FIFO mDriverReceiveFIFO0 ;
  public: uint32_t driverReceiveFIFO0Size (void) { return mDriverReceiveFIFO0.size () ; }
  public: uint32_t driverReceiveFIFO0Count (void) { return mDriverReceiveFIFO0.count () ; }
  public: uint32_t driverReceiveFIFO0PeakCount (void) { return mDriverReceiveFIFO0.peakCount () ; }
  public: void resetDriverReceiveFIFO0PeakCount (void) { mDriverReceiveFIFO0.resetPeakCount () ; }

//--- Driver receive FIFO 1
  protected: ACANFD_STM32_FIFO mDriverReceiveFIFO1 ;
  public: uint32_t driverReceiveFIFO1Size (void) { return mDriverReceiveFIFO1.size () ; }
  public: uint32_t driverReceiveFIFO1Count (void) { return mDriverReceiveFIFO1.count () ; }
  public: uint32_t driverReceiveFIFO1PeakCount (void) { return mDriverReceiveFIFO1.peakCount () ; }
  public: void resetDriverReceiveFIFO1PeakCount (void) { mDriverReceiveFIFO1.resetPeakCount () ; }


//--- Constant public properties
  public: const uint32_t mRamBaseAddress ;
  public: const std::initializer_list <ACANFD_STM32::PinPort> mTxPinArray ;
  public: const std::initializer_list <ACANFD_STM32::PinPort> mRxPinArray ;
  public: const std::optional <IRQs> mIRQs ;

//--- Protected properties
  protected: volatile FDCAN_GlobalTypeDef * mPeripheralPtr ;
  protected: ACANFD_STM32_DynamicArray < ACANFDCallBackRoutine > mStandardFilterCallBackArray ;
  protected: ACANFD_STM32_DynamicArray < ACANFDCallBackRoutine > mExtendedFilterCallBackArray ;
  protected: ACANFDCallBackRoutine mNonMatchingStandardMessageCallBack = nullptr ;
  protected: ACANFDCallBackRoutine mNonMatchingExtendedMessageCallBack = nullptr ;

//--- Internal methods
  public: void isr0 (void) ;
  public: void isr1 (void) ;
  private: void writeTxBuffer (const CANFDMessage & inMessage,
                               const uint32_t inTxBufferIndex) ;
  private: void internalDispatchReceivedMessage (const CANFDMessage & inMessage) ;

//--- Status Flags (returns 0 if no error)
//  Bit 0 : hardware RxFIFO 0 overflow
//  Bit 1 : driver RxFIFO 0 overflow
//  Bit 2 : hardware RxFIFO 1 overflow
//  Bit 3 : driver RxFIFO 2 overflow
//  Bit 4 : bus off
  public: uint32_t statusFlags (void) const ;

//--- Bus Status
  public: inline BusStatus getStatus (void) const { return BusStatus (mPeripheralPtr) ; }

//--- No copy
  private : ACANFD_STM32 (const ACANFD_STM32 &) = delete ;
  private : ACANFD_STM32 & operator = (const ACANFD_STM32 &) = delete ;

} ;

//------------------------------------------------------------------------------
