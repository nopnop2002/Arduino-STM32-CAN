//------------------------------------------------------------------------------

#include "ACANFD_STM32_Settings.h"
#include <algorithm>

//------------------------------------------------------------------------------
//    BIT DECOMPOSITION CONSTRAINTS
// Data bit Rate:
//    - The CAN bit time may be programmed in the range of 4 to 49 time quanta.
//    - The CAN time quantum may be programmed in the range of 1 to 32 GCLK_CAN periods.
//    - The bit rate configured for the CAN FD data phase via DBTP must be higher or
//      equal to the bit rate configured for the arbitration phase via NBTP.
// Data bit Rate:
//    - The CAN bit time may be programmed in the range of 4 to 385 time quanta.
//    - The CAN time quantum may be programmed in the range of 1 to 512 GCLK_CAN periods.
//------------------------------------------------------------------------------

static const uint32_t MIN_DATA_PS1 = 2 ;
static const uint32_t MAX_DATA_PS1 = 32 ;
static const uint32_t MIN_DATA_PS2 = 1 ;
static const uint32_t MAX_DATA_PS2 = 16 ;

static const uint32_t MAX_DATA_SJW = MAX_DATA_PS2 ;

static const uint32_t MIN_DATA_TQ_COUNT = 1 + MIN_DATA_PS1 + MIN_DATA_PS2 ;
static const uint32_t MAX_DATA_TQ_COUNT = 1 + MAX_DATA_PS1 + MAX_DATA_PS2 ;

static const uint32_t MAX_TRANSCEIVER_DELAY_COMPENSATION = 127 ;

//------------------------------------------------------------------------------

static const uint32_t MIN_ARBITRATION_PS1 = 2 ;
static const uint32_t MAX_ARBITRATION_PS1 = 256 ;
static const uint32_t MIN_ARBITRATION_PS2 = 1 ;
static const uint32_t MAX_ARBITRATION_PS2 = 128 ;

static const uint32_t MAX_ARBITRATION_SJW = MAX_ARBITRATION_PS2 ;

static const uint32_t MAX_ARBITRATION_TQ_COUNT = 1 + MAX_ARBITRATION_PS1 + MAX_ARBITRATION_PS2 ;


static const uint32_t MAX_BRP = 32 ;

//------------------------------------------------------------------------------
//    CONSTRUCTORS
//------------------------------------------------------------------------------

ACANFD_STM32_Settings::ACANFD_STM32_Settings (const uint32_t inDesiredArbitrationBitRate,
                                              const DataBitRateFactor inDataBitRateFactor,
                                              const uint32_t inTolerancePPM) :
ACANFD_STM32_Settings (inDesiredArbitrationBitRate, 75, inDataBitRateFactor, 75, inTolerancePPM) {
}

//------------------------------------------------------------------------------

ACANFD_STM32_Settings::ACANFD_STM32_Settings (const uint32_t inDesiredArbitrationBitRate,
                                              const uint32_t inDesiredArbitrationSamplePoint,
                                              const DataBitRateFactor inDataBitRateFactor,
                                              const uint32_t inDesiredDataSamplePoint,
                                              const uint32_t inTolerancePPM) :
mDesiredArbitrationBitRate (inDesiredArbitrationBitRate),
mDataBitRateFactor (inDataBitRateFactor) {
//---------------------------------------------- Configure CANFD bit decomposition
  const uint32_t dataBitRate = inDesiredArbitrationBitRate * uint32_t (inDataBitRateFactor) ;
  uint32_t dataTQCount = std::min (
    MAX_DATA_TQ_COUNT,
    MAX_ARBITRATION_TQ_COUNT / uint32_t (inDataBitRateFactor)
  ) ;
  uint32_t smallestDifference = UINT32_MAX ;
  mBitRatePrescaler = MAX_BRP ; // Setting for slowest bitrate
  uint32_t bestDataTQCount = dataTQCount ; // Setting for slowest bitrate
  const uint32_t FDCAN_ROOT_CLOCK_FREQUENCY = fdcanClock () ;
//   Serial.print ("FDCAN_ROOT_CLOCK_FREQUENCY = ") ;
//   Serial.println (FDCAN_ROOT_CLOCK_FREQUENCY) ;
  uint32_t BRP = FDCAN_ROOT_CLOCK_FREQUENCY / (dataBitRate * dataTQCount) ;
//--- Loop for finding best BRP and best TQCount
  while ((smallestDifference > 0) && (dataTQCount >= MIN_DATA_TQ_COUNT) && (BRP <= MAX_BRP)) {
  //--- Compute error using BRP (caution: BRP should be > 0)
    if (BRP > 0) {
      const uint32_t difference = FDCAN_ROOT_CLOCK_FREQUENCY - dataBitRate * dataTQCount * BRP ; // difference is always >= 0
      if (difference < smallestDifference) {
        smallestDifference = difference ;
        mBitRatePrescaler = BRP ;
        bestDataTQCount = dataTQCount ;
      }
    }
  //--- Compute difference using BRP+1 (caution: BRP+1 should be <= MAX_BRP)
    if (BRP < MAX_BRP) {
      const uint32_t difference = dataBitRate * dataTQCount * (BRP + 1) - FDCAN_ROOT_CLOCK_FREQUENCY ; // difference is always >= 0
      if (difference < smallestDifference) {
        smallestDifference = difference ;
        mBitRatePrescaler = BRP + 1 ;
        bestDataTQCount = dataTQCount ;
      }
    }
  //--- Continue with next value of TQCount
    dataTQCount -= 1 ;
    BRP = FDCAN_ROOT_CLOCK_FREQUENCY / (dataBitRate * dataTQCount) ;
  }
//-------------------------- Set Data segment lengthes
//--- Compute PS1
  const uint32_t dataSP = inDesiredDataSamplePoint * bestDataTQCount ;
  mDataPhaseSegment1 = dataSP / 100 - 1 ;
  { const uint32_t diff1 = dataSP - (mDataPhaseSegment1 + 1) * 100 ;
    if (diff1 > 0) {
      const uint32_t diff2 = (mDataPhaseSegment1 + 2) * 100 - dataSP ;
      if (diff2 < diff1) {
        mDataPhaseSegment1 += 1 ;
      }
    }
  }
  if (mDataPhaseSegment1 > MAX_DATA_PS1) {
    mDataPhaseSegment1 = MAX_DATA_PS1 ; // Always 1 <= PS1 <= 32
  }
//--- Set PS2 to remaining TQCount
  mDataPhaseSegment2 = bestDataTQCount - mDataPhaseSegment1 - 1 ;
//--- Adjust PS1 and PS2 if PS2 is too large
  if (mDataPhaseSegment2 > MAX_DATA_PS2) {
    mDataPhaseSegment1 -= mDataPhaseSegment2 - MAX_DATA_PS2 ;
    mDataPhaseSegment2 = MAX_DATA_PS2 ;
  }
//--- Set RJW to PS2
  mDataSJW = mDataPhaseSegment2 ;
//-------------------------- Set TDCO
  mTransceiverDelayCompensation = (dataBitRate <= 1'000'000)
    ? 0
    : ((mBitRatePrescaler * bestDataTQCount) / 2)
  ;
//-------------------------- Set Arbitration segment lengthes
  const uint32_t bestArbitrationTQCount = bestDataTQCount * uint32_t (inDataBitRateFactor) ;
//--- Compute PS1
  const uint32_t arbitrationSP = inDesiredArbitrationSamplePoint * bestArbitrationTQCount ;
  mArbitrationPhaseSegment1 = arbitrationSP / 100 - 1 ;
  { const uint32_t diff1 = arbitrationSP - (mArbitrationPhaseSegment1 + 1) * 100 ;
    if (diff1 > 0) {
      const uint32_t diff2 = (mArbitrationPhaseSegment1 + 2) * 100 - arbitrationSP ;
      if (diff2 < diff1) {
        mArbitrationPhaseSegment1 += 1 ;
      }
    }
  }
  if (mArbitrationPhaseSegment1 > MAX_ARBITRATION_PS1) {
    mArbitrationPhaseSegment1 = MAX_ARBITRATION_PS1 ; // Always 1 <= PS1 <= 256
  }
//--- Set PS2 to remaining TQCount
  mArbitrationPhaseSegment2 = uint8_t (bestArbitrationTQCount - mArbitrationPhaseSegment1 - 1) ;
//--- Adjust PS1 and PS2 if PS2 is too large
  if (mArbitrationPhaseSegment2 > MAX_ARBITRATION_PS2) {
    mArbitrationPhaseSegment1 -= mArbitrationPhaseSegment2 - MAX_ARBITRATION_PS2 ;
    mArbitrationPhaseSegment2 = MAX_DATA_PS2 ;
  }
//--- Set RJW to PS2
  mArbitrationSJW = mArbitrationPhaseSegment2 ;
//-------------------------- Final check of the configuration
  const uint32_t W = bestArbitrationTQCount * mDesiredArbitrationBitRate * mBitRatePrescaler ;
  const uint64_t diff = (FDCAN_ROOT_CLOCK_FREQUENCY > W) ? (FDCAN_ROOT_CLOCK_FREQUENCY - W) : (W - FDCAN_ROOT_CLOCK_FREQUENCY) ;
  const uint64_t ppm = uint64_t (1000 * 1000) ;
  mBitSettingOk = (diff * ppm) <= (uint64_t (W) * inTolerancePPM) ;
} ;

//------------------------------------------------------------------------------

uint32_t ACANFD_STM32_Settings::actualArbitrationBitRate (void) const {
  const uint32_t FDCAN_ROOT_CLOCK_FREQUENCY = fdcanClock () ;
  const uint32_t TQCount = 1 /* Sync Seg */ + mArbitrationPhaseSegment1 + mArbitrationPhaseSegment2 ;
  return FDCAN_ROOT_CLOCK_FREQUENCY / (mBitRatePrescaler * TQCount) ;
}

//------------------------------------------------------------------------------

uint32_t ACANFD_STM32_Settings::actualDataBitRate (void) const {
  const uint32_t FDCAN_ROOT_CLOCK_FREQUENCY = fdcanClock () ;
  const uint32_t TQCount = 1 /* Sync Seg */ + mDataPhaseSegment1 + mDataPhaseSegment2 ;
  return FDCAN_ROOT_CLOCK_FREQUENCY / (mBitRatePrescaler * TQCount) ;
}

//------------------------------------------------------------------------------

bool ACANFD_STM32_Settings::exactArbitrationBitRate (void) const {
  const uint32_t FDCAN_ROOT_CLOCK_FREQUENCY = fdcanClock () ;
  const uint32_t TQCount = 1 /* Sync Seg */ + mArbitrationPhaseSegment1 + mArbitrationPhaseSegment2 ;
  return FDCAN_ROOT_CLOCK_FREQUENCY == (mBitRatePrescaler * mDesiredArbitrationBitRate * TQCount) ;
}

//------------------------------------------------------------------------------

bool ACANFD_STM32_Settings::exactDataBitRate (void) const {
  const uint32_t FDCAN_ROOT_CLOCK_FREQUENCY = fdcanClock () ;
  const uint32_t TQCount = 1 /* Sync Seg */ + mDataPhaseSegment1 + mDataPhaseSegment2 ;
  return FDCAN_ROOT_CLOCK_FREQUENCY == (mBitRatePrescaler * mDesiredArbitrationBitRate * TQCount * uint32_t (mDataBitRateFactor)) ;
}

//------------------------------------------------------------------------------

uint32_t ACANFD_STM32_Settings::ppmFromWishedBitRate (void) const {
  const uint32_t FDCAN_ROOT_CLOCK_FREQUENCY = fdcanClock () ;
  const uint32_t TQCount = 1 /* Sync Seg */ + mArbitrationPhaseSegment1 + mArbitrationPhaseSegment2 ;
  const uint32_t W = TQCount * mDesiredArbitrationBitRate * mBitRatePrescaler ;
  const uint64_t diff = (FDCAN_ROOT_CLOCK_FREQUENCY > W) ? (FDCAN_ROOT_CLOCK_FREQUENCY - W) : (W - FDCAN_ROOT_CLOCK_FREQUENCY) ;
  const uint64_t ppm = uint64_t (1000 * 1000) ;
  return uint32_t ((diff * ppm) / W) ;
}

//------------------------------------------------------------------------------

float ACANFD_STM32_Settings::arbitrationSamplePointFromBitStart (void) const {
  const uint32_t TQCount = 1 /* Sync Seg */ + mArbitrationPhaseSegment1 + mArbitrationPhaseSegment2 ;
  const uint32_t samplePoint = 1 /* Sync Seg */ + mArbitrationPhaseSegment1 ;
  const float partPerCent = 100.0 ;
  return (float (samplePoint) * partPerCent) / float (TQCount) ;
}

//------------------------------------------------------------------------------

float ACANFD_STM32_Settings::dataSamplePointFromBitStart (void) const {
  const uint32_t TQCount = 1 /* Sync Seg */ + mDataPhaseSegment1 + mDataPhaseSegment2 ;
  const uint32_t samplePoint = 1 /* Sync Seg */ + mDataPhaseSegment1 ;
  const float partPerCent = 100.0 ;
  return (float (samplePoint) * partPerCent) / float (TQCount) ;
}

//------------------------------------------------------------------------------

uint32_t ACANFD_STM32_Settings::checkBitSettingConsistency (void) const {
  uint32_t errorCode = 0 ; // Means no error
  if (mBitRatePrescaler == 0) {
    errorCode |= kBitRatePrescalerIsZero ;
  }else if (mBitRatePrescaler > MAX_BRP) {
    errorCode |= kBitRatePrescalerIsGreaterThan32 ;
  }
  if (mArbitrationPhaseSegment1 < MIN_ARBITRATION_PS1) {
    errorCode |= kArbitrationPhaseSegment1IsZero ;
  }else if (mArbitrationPhaseSegment1 > MAX_ARBITRATION_PS1) {
    errorCode |= kArbitrationPhaseSegment1IsGreaterThan256 ;
  }
  if (mArbitrationPhaseSegment2 < MIN_ARBITRATION_PS2) {
    errorCode |= kArbitrationPhaseSegment2IsLowerThan2 ;
  }else if (mArbitrationPhaseSegment2 > MAX_ARBITRATION_PS2) {
    errorCode |= kArbitrationPhaseSegment2IsGreaterThan128 ;
  }
  if (mArbitrationSJW == 0) {
    errorCode |= kArbitrationSJWIsZero ;
  }else if (mArbitrationSJW > MAX_ARBITRATION_SJW) {
    errorCode |= kArbitrationSJWIsGreaterThan128 ;
  }
  if (mArbitrationSJW > mArbitrationPhaseSegment2) {
    errorCode |= kArbitrationSJWIsGreaterThanPhaseSegment2 ;
  }
  if (mDataPhaseSegment1 < MIN_DATA_PS1) {
    errorCode |= kDataPhaseSegment1IsZero ;
  }else if (mDataPhaseSegment1 > MAX_DATA_PS1) {
    errorCode |= kDataPhaseSegment1IsGreaterThan32 ;
  }
  if (mDataPhaseSegment2 < MIN_DATA_PS2) {
    errorCode |= kDataPhaseSegment2IsLowerThan2 ;
  }else if (mDataPhaseSegment2 > MAX_DATA_PS2) {
    errorCode |= kDataPhaseSegment2IsGreaterThan16 ;
  }
  if (mDataSJW == 0) {
    errorCode |= kDataSJWIsZero ;
  }else if (mDataSJW > MAX_DATA_SJW) {
    errorCode |= kDataSJWIsGreaterThan16 ;
  }
  if (mDataSJW > mDataPhaseSegment2) {
    errorCode |= kDataSJWIsGreaterThanPhaseSegment2 ;
  }
  if (mTransceiverDelayCompensation > MAX_TRANSCEIVER_DELAY_COMPENSATION) {
    errorCode |= kTransceiverDelayCompensationIsGreaterThan127 ;
  }
  return errorCode ;
}

//------------------------------------------------------------------------------

