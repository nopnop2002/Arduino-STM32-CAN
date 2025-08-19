//------------------------------------------------------------------------------

#pragma once

//------------------------------------------------------------------------------

#include <Arduino.h>

//------------------------------------------------------------------------------
#if 0
#ifdef ARDUINO_NUCLEO_H743ZI2
  #include "ACANFD_STM32_NUCLEO_H743ZI2-settings.h"
#elif defined (ARDUINO_NUCLEO_G431KB)
  #include "ACANFD_STM32_NUCLEO_G431KB-settings.h"
#elif defined (ARDUINO_NUCLEO_G474RE)
  #include "ACANFD_STM32_NUCLEO_G474RE-settings.h"
#elif defined (ARDUINO_WEACT_G474CE)
  #include "ACANFD_STM32_WEACT_G474CE-settings.h"
#elif defined (ARDUINO_NUCLEO_G0B1RE)
  #include "ACANFD_STM32_NUCLEO_G0B1RE-settings.h"
#elif defined (ARDUINO_NUCLEO_H723ZG)
  #include "ACANFD_STM32_NUCLEO_H723ZG-settings.h"
#else
  #error "Unhandled Nucleo Board"
#endif
#endif

#include "ACANFD_STM32_NUCLEO_G474RE-settings.h"

//------------------------------------------------------------------------------

#include "ACANFD_STM32_Filters.h"
#include "ACANFD_STM32_DataBitRateFactor.h"

//------------------------------------------------------------------------------

uint32_t fdcanClock (void) ;

//------------------------------------------------------------------------------
//  ACANFD_STM32_Settings class
//------------------------------------------------------------------------------

class ACANFD_STM32_Settings {

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  //   Enumerations
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  public: typedef enum : uint8_t {
    DISABLE_FD,
    NORMAL_FD,
    INTERNAL_LOOP_BACK,
    EXTERNAL_LOOP_BACK,
    BUS_MONITORING
  } ModuleMode ;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  //    Constructors for a given bit rate
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  public: ACANFD_STM32_Settings (const uint32_t inDesiredArbitrationBitRate,
                                 const DataBitRateFactor inDataBitRateFactor,
                                 const uint32_t inTolerancePPM = 1000) ;

  public: ACANFD_STM32_Settings (const uint32_t inDesiredArbitrationBitRate,
                                 const uint32_t inDesiredArbitrationSamplePoint,
                                 const DataBitRateFactor inDataBitRateFactor,
                                 const uint32_t inDesiredDataSamplePoint,
                                 const uint32_t inTolerancePPM = 1000) ;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  //    Properties
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

//--- CAN FD bit timing
  public: const uint32_t mDesiredArbitrationBitRate ; // In bit/s
  public: const DataBitRateFactor mDataBitRateFactor ;
//--- bitrate prescaler is common to arbitration bitrate and data bitrate
  public: uint32_t mBitRatePrescaler = 32 ; // 1...32
//--- Arbitration segments
  public: uint32_t mArbitrationPhaseSegment1 = 256 ; // 1...256
  public: uint32_t mArbitrationPhaseSegment2 = 128 ;  // 2...128
  public: uint32_t mArbitrationSJW = 128 ; // 1...128
//--- Data segments
  public: uint32_t mDataPhaseSegment1 = 32 ; // 1...32
  public: uint32_t mDataPhaseSegment2 = 16 ;  // 2...16
  public: uint32_t mDataSJW = 16 ; // 1...16

//--- Transceiver Delay Compensation
  public: uint32_t mTransceiverDelayCompensation = 0 ; // 0 ... 127

  public: bool mBitSettingOk = true ; // The above configuration is correct

//--- Module Mode
  public : ModuleMode mModuleMode = NORMAL_FD ;

//--- Driver receive FIFO Sizes
  public: uint16_t mDriverReceiveFIFO0Size = 60 ;
  public: uint16_t mDriverReceiveFIFO1Size = 60 ;

//--- Remote frame reception
  public: bool mDiscardReceivedStandardRemoteFrames = false ;
  public: bool mDiscardReceivedExtendedRemoteFrames = false ;

//-- Accept non matching frames ?
  public: ACANFD_STM32_FilterAction mNonMatchingStandardFrameReception = ACANFD_STM32_FilterAction::FIFO0 ;
  public: ACANFD_STM32_FilterAction mNonMatchingExtendedFrameReception = ACANFD_STM32_FilterAction::FIFO0 ;
  public: void (*mNonMatchingStandardMessageCallBack) (const CANFDMessage & inMessage) = nullptr ;
  public: void (*mNonMatchingExtendedMessageCallBack) (const CANFDMessage & inMessage) = nullptr ;

//--- Driver transmit buffer Size
  public: uint16_t mDriverTransmitFIFOSize = 10 ;

//--- Automatic retransmission
  public: bool mEnableRetransmission = true ;

//--- TxPin is push/pull, or open collector output
  public: bool mOpenCollectorOutput = false ; // false --> push / pull, true --> open collector

//--- RxPin has internal pullup ?
  public: bool mInputPullup = false ; // false --> no pull up, no pull down, true --> pullup, no pull down

//--- Tx Pin
  public: uint8_t mTxPin = 255 ; // By default, uses the first entry of Tx pin array

//--- Rx Pin
  public: uint8_t mRxPin = 255 ; // By default, uses the first entry of Rx pin array

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // Accessors
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

//--- Compute actual bitrate
  public: uint32_t actualArbitrationBitRate (void) const ;
  public: uint32_t actualDataBitRate (void) const ;

//--- Exact bitrate ?
  public: bool exactArbitrationBitRate (void) const ;
  public: bool exactDataBitRate (void) const ;

//--- Distance between actual bitrate and requested bitrate (in ppm, part-per-million)
  public: uint32_t ppmFromWishedBitRate (void) const ;

//--- Distance of sample point from bit start (in ppc, part-per-cent, denoted by %)
  public: float arbitrationSamplePointFromBitStart (void) const ;
  public: float dataSamplePointFromBitStart (void) const ;

//--- Bit settings are consistent ? (returns 0 if ok)
  public: uint32_t checkBitSettingConsistency (void) const ;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // Constants returned by CANBitSettingConsistency
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  public: static const uint32_t kBitRatePrescalerIsZero                       = 1U <<  0 ;
  public: static const uint32_t kBitRatePrescalerIsGreaterThan32              = 1U <<  1 ;
  public: static const uint32_t kArbitrationPhaseSegment1IsZero               = 1U <<  2 ;
  public: static const uint32_t kArbitrationPhaseSegment1IsGreaterThan256     = 1U <<  3 ;
  public: static const uint32_t kArbitrationPhaseSegment2IsLowerThan2         = 1U <<  4 ;
  public: static const uint32_t kArbitrationPhaseSegment2IsGreaterThan128     = 1U <<  5 ;
  public: static const uint32_t kArbitrationSJWIsZero                         = 1U <<  6 ;
  public: static const uint32_t kArbitrationSJWIsGreaterThan128               = 1U <<  7 ;
  public: static const uint32_t kArbitrationSJWIsGreaterThanPhaseSegment2     = 1U <<  8 ;

  public: static const uint32_t kDataPhaseSegment1IsZero                      = 1U << 10 ;
  public: static const uint32_t kDataPhaseSegment1IsGreaterThan32             = 1U << 11 ;
  public: static const uint32_t kDataPhaseSegment2IsLowerThan2                = 1U << 12 ;
  public: static const uint32_t kDataPhaseSegment2IsGreaterThan16             = 1U << 13 ;
  public: static const uint32_t kDataSJWIsZero                                = 1U << 14 ;
  public: static const uint32_t kDataSJWIsGreaterThan16                       = 1U << 15 ;
  public: static const uint32_t kDataSJWIsGreaterThanPhaseSegment2            = 1U << 16 ;
  public: static const uint32_t kTransceiverDelayCompensationIsGreaterThan127 = 1U << 17 ;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  //  Extension for programmable RAM section CANFD modules
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  #ifndef HAS_PROGRAMMABLE_FDCAN_RAM_SECTIONS
    #error "HAS_PROGRAMMABLE_FDCAN_RAM_SECTIONS is not defined"
  #endif

  #if HAS_PROGRAMMABLE_FDCAN_RAM_SECTIONS == true

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    public: typedef enum : uint8_t {
      PAYLOAD_8_BYTES  = 0,
      PAYLOAD_12_BYTES = 1,
      PAYLOAD_16_BYTES = 2,
      PAYLOAD_20_BYTES = 3,
      PAYLOAD_24_BYTES = 4,
      PAYLOAD_32_BYTES = 5,
      PAYLOAD_48_BYTES = 6,
      PAYLOAD_64_BYTES = 7
    } Payload ;

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    public: static uint32_t wordCountForPayload (const Payload inPayload) {
      const uint32_t WORD_COUNT [8] = {4, 5, 6, 7, 8, 10, 14, 18} ;
      return WORD_COUNT [uint32_t (inPayload)] ;
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    public: static uint32_t frameDataByteCountForPayload (const Payload inPayload) {
      return (wordCountForPayload (inPayload) - 2) * 4 ;
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    //--- Hardware Rx FIFO 0
      public: uint8_t mHardwareRxFIFO0Size = 20 ; // 0 ... 64
      public: Payload mHardwareRxFIFO0Payload = PAYLOAD_64_BYTES ;

    //--- Hardware Rx FIFO 1
      public: uint8_t mHardwareRxFIFO1Size = 2 ; // 0 ... 64
      public: Payload mHardwareRxFIFO1Payload = PAYLOAD_64_BYTES ;

    //--- Hardware Transmit Buffers
    //    Required: mHardwareTransmitTxFIFOSize + mHardwareDedicacedTxBufferCount <= 32
      public: uint8_t mHardwareTransmitTxFIFOSize = 10 ; // 1 ... 32
      public: uint8_t mHardwareDedicacedTxBufferCount = 1 ; // 0 ... 30
      public: Payload mHardwareTransmitBufferPayload = PAYLOAD_64_BYTES ;

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  #endif

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

} ;

//------------------------------------------------------------------------------
